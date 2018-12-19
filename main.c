#include <msp430.h>


int IB = 0;
int F = 0;
char temp;

int CUART()
{
    P4SEL |= BIT4+BIT5;             //sets uart pins as 4.4 and 4.5
    UCA1CTL1 |= UCSWRST;            // **Put state machine in reset**
    UCA1CTL1 |=UCSSEL_2;            //SMCLK
    UCA1BR0= 9;                     // 1MHz 115200 (see User's Guide)
    UCA1BR1= 0;                     // 1MHz 115200
    UCA1MCTL |= UCBRS_1 +UCBRF_0;   // Modulation UCBRSx=1, UCBRFx=0
    UCA1CTL1 &= ~UCSWRST;           // **Initialize USCI state machine**
}


int CPINS()
{
    P3DIR &=~BIT7;
    P3OUT |= BIT7;
    P3REN |= BIT7;
    P8DIR &=~BIT2;
    P8OUT |= BIT2;
    P8REN |= BIT2;
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
    TA1CCR0 = 120000;                           // PWM Period
    TA1CCR1 = 0;                                // CCR1 PWM duty cycle
    TA1CCTL1 = OUTMOD_7;
    TA1CTL = TASSEL_2 + MC_1 + ID_2;            // SMCLK, up mode, clear TAR
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
    char topic[] = {'$', 'F', 'i', 'r', 'e', '\n'};                                     //subscribes to topic name
    int strCounter = 0;                                                                 //sets counter size
    int strLen = 5;                                                                     //sets topic length size starting from 0
    while (strCounter<=strLen)                                                          //compares length of topic and counter
    {
        while(!(UCA1IFG&UCTXIFG));
        temp = topic[strCounter];                                                       //sends topic name as goes through loop
        UCA1TXBUF = temp;                                                               //sends topic name as goes through loop
        strCounter++;                                                                   //adds to counter size
    }
}


int sendNotice()
{
    char notice[] = {'#', 'F', 'i', 'r', 'e', ' ', 'F', 'i', 'r', 'e', '\n'};           //sends topic name to send a message to
    int noticeLen = 10;                                                                 //sets topic+message length size starting from 0
    int noticeCounter = 0;                                                              //sets counter size
    while (noticeCounter<=noticeLen)                                                    //compares length of message and counter
        {
            while(!(UCA1IFG&UCTXIFG));
            temp = notice[noticeCounter];                                               //sends message as goes through loop
            UCA1TXBUF = temp;                                                           //sends message as goes through loop
            noticeCounter++;                                                            //adds to counter size
        }
}


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                                                           // stop watchdog timer
    CPINS();                                                                            // calls the configure pins function
    CPWM();                                                                             // calls the configure pwm function
    CUART();                                                                            // calls the configure uart function
    Ctopic();                                                                           // calls the configure subcribe to topic function
    while (1)
    {
        _BIS_SR(LPM0_bits + GIE);                                                       //sets low power mode and global interrupts
    }
}

#pragma vector = TIMER0_A0_VECTOR                   // Detects interrupt for CCR0 on Timer1
__interrupt void Timer_A0(void)
{
    if(P3IN & BIT7)                                //checks value of pin 3.7
    {
        IB = 1;                                    //sets value of "In Building" to 1
    }
    else
    {
        IB = 0;                                    //if pin 3.7 is not driven high sets value to 0.
       TA1CCR1 = 0;                                //keeps pwm off
    }
    if(P8IN & BIT2)                                //checks value of pin 8.2
    {
        F = 0;                                     //sets value of "fire" to 0
        TA1CCR1 = 0;                               //keeps pwm off
    }
    else
    {
        F = 1;                                     //sets value of Fire to 1 if fire and in building are 1
        sendNotice();                              // sends "Fire" message
    }
    if(IB == 1 && F == 1)
        {
        // = 'PEOPLE NEED TO BE RESCUED!!!';
                TA1CCR1 = 100000;                   //sets pwm to a buzzer to audibly alert the person in building if there is motion from 3.7 and fire from 8.2
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

