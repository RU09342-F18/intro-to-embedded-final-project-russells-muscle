/*
  MQTT-to-UART Bridge
  This code is meant to encapsulate all of the necessary protocols and resources for MQTT and
  leave it on the ESP8266. Connected over a simple UART connection, this can allow your project to quickly
  talk over the internet without the need for complicating your design.

  Commands:
  $Subscribe        SYNTAX: "$Topic\n"
  #Publish          SYNTAX: "#Topic Payload\n"
  !ReceivedMessage  SYNTAX: "!Topic: Message\n"
  ~Unsubscribe      SYNTAX: "~Topic\n"
*/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
// Update these with values suitable for your network.
const char* ssid = "Rowan_IoT";
const char* password =  "";
const char* mqtt_server = "172.24.137.95";
const int mqttPort = 1883;
/* This is for CloudMQTT
const char* mqtt_server = "m13.cloudmqtt.com";
const int mqttPort = 18929;
const char* mqttUser = "ryfxshnb";
const char* mqttPassword = "7_QO8D0Z0-To";
*/
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
char tempmsg[50];
int value = 0;
void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqttPort);
  client.setCallback(callback);
}
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("!");
  Serial.print(topic);
  Serial.print(": ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }
}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    
    //if (client.connect("ESP8266Client", mqttUser, mqttPassword)) {
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      client.publish("TempOut", "Temperature Readings:"); //publish temperature value to "TempOut"
      client.subscribe("inTemp"); //temp sensor data pull
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
long now = millis();
if(now - lastMsg > 20000){
    if(Serial.available()>0){
        char temp[50];    
        String x;
        char t[50];
        x = Serial.readStringUntil('\n');
        x.toCharArray(t,50);
        strcpy(temp, t);
        switch(temp[0]){
           case '$':  
           //subscribe to a topic syntax: $topic           
           for (int i=0; i < sizeof temp; i++){ //remove the delimiter from the array
            temp[i] = temp[i+1];
           }
           client.subscribe(temp);
           break;
           
           case '#':
           //publish to a topic, syntax: #topic words
           char topic[50];
           char message[50];
            for (int i=0; i<50; i++){
              topic[i] = NULL;
              message[i] = NULL;            
           }
           int j;
           int a;
           
           for (int i=0; i < sizeof temp; i++){ //remove the first char from the array
            temp[i] = temp[i+1];
           }
           //Serial.println(temp);
       
           for ( a = 0; a< sizeof temp; a++){ //find space
            if( temp[a] == ' '){
              j = a; 
              break;           
            }          
           }
           //Serial.println(j);
     
           for (int b = 0; b < j; b++){ //grab topic 
            topic[b] = temp[b];
           }
           //Serial.println(topic);
   
           for (int c = j+1, d = 0; c < sizeof temp; c++){ //grab message
            message[d] = temp[c];
            d++;
           }
           //Serial.println(message);
           snprintf(tempmsg, sizeof tempmsg, "%s", message);
           client.publish(topic, tempmsg); //publish message to topic
           break;
           case '~':
           // Unsubscribe from topic. Syntax: ~Topic
           for (int i=0; i < sizeof temp; i++){ //remove the delimiter from the array
            temp[i] = temp[i+1];
           }
           client.unsubscribe(temp);
           break;
        }
        
        //snprintf(tempmsg, sizeof tempmsg, "Temp: %s", temp);
        
       // Serial.print("Publish message: ");
       // Serial.println(tempmsg);
       // client.publish("TempOut", tempmsg);
    }  
}
}
