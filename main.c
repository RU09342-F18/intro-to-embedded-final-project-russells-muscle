#include <msp430.h>


int IB = 0;
int F = 0;


int CUART()
{
    P4SEL |= BIT4+BIT5;
        UCA1CTL1 |= UCSWRST;
        UCA1CTL1 |=UCSSEL_2;
        UCA1BR0= 0x03;
        UCA1BR1= 0;
        UCA1MCTL |= UCBRS_0 +UCBRF_13 + UCOS16;
        UCA1CTL1 &= ~UCSWRST;
}


int CPINS()
{
    P3DIR &=~BIT7;
    P3OUT |= BIT7;
    P3REN |= BIT7;
    P8DIR &=~BIT2;
    P8OUT |= BIT2;
    P8REN |= BIT2;
   /* P3IE |= BIT7;
    P3IES |= BIT7;
    P3IFG &=~BIT7;
    P8IE |= BIT2;
    P8IES |= BIT2;
    P8IFG &=~BIT2; */
}


int CPWM()
{
    P2DIR |= BIT0; //Speaker
    P2SEL |= BIT0;
    P2OUT &= ~BIT0;
    P1DIR |= BIT0; //Red
    P1OUT &=~BIT0;
    P4DIR |= BIT7; //Green
    P4OUT &= ~BIT7;
    TA0CTL = TASSEL_2 + MC_1 + ID_3;
    TA0CCR0 = 10000000;
    TA0CCTL0 = CCIE;
    TA1CCR0 = 120000;                          // PWM Period
    TA1CCR1 = 0;                            // CCR1 PWM duty cycle
    TA1CCTL1 = OUTMOD_7;
    TA1CTL = TASSEL_2 + MC_1 + ID_2;         // SMCLK, up mode, clear TAR
    TA1CCTL0 |= CCIE;
    TA0CCTL0 |= CCIE;
}


int ToggleRED()
{
    P1OUT ^= BIT0;
}


int ToggleGreen()
{
    P4OUT ^= BIT7;
}


int OFFRED()
{
    P1OUT &= ~BIT0;
}


int OFFGreen()
{
    P4OUT &= ~BIT7;
}


int Ctopic()
{
    char topic[] = {'$', 'F', 'i', 'r', 'e', ' ', 10};
    int strCounter = 0;
    int strLen = 6;
    while (strCounter<=strLen)
    {
        while(!(UCTXIFG));
        UCA1TXBUF = topic[strCounter];
        strCounter++;
    }
}


int sendNotice()
{
    char notice[] = {'#', 'F', 'i', 'r', 'e', ' ', 'i', 'n', ' ', 'd', 'i', 's', ' ', 'b', 'i', 't', 'c', 'h', ' ' , 10};
    int noticeLen = sizeof(notice);
    int noticeCounter;
    while (noticeCounter<=noticeLen)
        {
            while(!(UCTXIFG));
            UCA1TXBUF = notice[noticeCounter];
            noticeCounter++;
        }
}


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    CPINS();
    CPWM();
    CUART();
    Ctopic();
    while (1)
    {
        _BIS_SR(LPM0_bits + GIE);
    }
}


/*#pragma vector=PORT1_VECTOR    //Motion Sensor
__interrupt void Port_1(void)
{
    if (IB == 0)
    {
       IB = 1;
       P1OUT |= BIT0;
       P4OUT &= ~BIT7;
    }
    P1IFG &=~BIT2;
}
#pragma vector=PORT2_VECTOR    //Fire Sensor
__interrupt void Port_2(void)
{
    F = 1;
        P4OUT |= BIT7;
        P1OUT &= ~BIT0;
        P1IFG &=~BIT3;
}*/


#pragma vector = TIMER0_A0_VECTOR                   // Detects interrupt for CCR0 on Timer1
__interrupt void Timer_A0(void)
{
    if(P3IN & BIT7)
    {
        IB = 1;
    }
    else
    {
        IB = 0;
       TA1CCR1 = 0;
    }
    if(P8IN & BIT2)
    {
        F = 0;
        TA1CCR1 = 0;
    }
    else
    {
       // UCA1TXBUF = 'Someone is in the building';
        F = 1;
        sendNotice();
    }
    if(IB == 1 && F == 1)
        {
        // = 'PEOPLE NEED TO BE RESCUED!!!';
                TA1CCR1 = 100000;
        }
}


#pragma vector = TIMER1_A0_VECTOR                   // Detects interrupt for CCR0 on Timer1
__interrupt void Timer_A1(void)
{
    if(P8IN & BIT2)
       {
        if(P3IN & BIT7){
            ToggleRED();
             ToggleGreen();
                      }
                      else
                      {
                          OFFRED();
                          OFFGreen();
                      }
       }
    else
       {
       ToggleRED();
        OFFGreen();
       }
}
