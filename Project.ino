#include <ESP8266WiFi.h>
#include <MicroGear.h>
const char* ssid= "$-God";
const char* password = "yubk2033";

#define APPID   "CoachProject"
#define KEY     "dzTzEdCLqzKkc4u"
#define SECRET  "TXfQr0IZ4iLoPcDvCH3f2VmqG"
#define ALIAS   "Board1"
#define TARGET  "CoachProjectIoT"

#define LED1     D0
#define LED2     D4

WiFiClient client;

int timer = 0;
MicroGear microgear(client);

void onMsghandler(char* topic, uint8_t* msg, unsigned int msglen){
  //String iMsg = "";//
  //for (int i=0;i<msglen;i++) iMsg += (char)msg[i];//
  
  Serial.print("Incoming message --> ");
  Serial.print(topic);
  Serial.print(" : ");
  
  char strState[msglen];
  for(int i= 0; i< msglen; i++){
    strState[i]=(char)msg[i];
    Serial.print((char)msg[i]);
  }
  Serial.println();
  String stateStr = String(strState).substring(0, msglen);
  //
  /*
  if(iMsg == "GET") {
    microgear.chat("/led", digitalRead(LED1) == HIGH ? "ON" : "LOW");
  } else if (iMsg == "ON") {
    digitalWrite(LED1, LOW);
  } else {
    digitalWrite(LED1, HIGH);
  }
  */
  //
  String iMsg = "";
  if (iMsg == "CH1ON") {
     digitalWrite(LED1, LOW);
  }
  
  if(stateStr == "1ON")
  {
    digitalWrite(LED1,LOW);
    Serial.println("RUN Pump1");
  }
  else if (stateStr == "1OFF") {
    digitalWrite(LED1,HIGH);
    Serial.println("OFF Pump1");
  }
  if(stateStr == "2ON")
  {
    digitalWrite(LED2,LOW);
    Serial.println("RUN Pump2");
  }
  else if (stateStr == "2OFF") {
    digitalWrite(LED2,HIGH);
    Serial.println("OFF Pump2");
  }
  if(stateStr == "CH12ON")
  {
    digitalWrite(LED1,LOW);
    digitalWrite(LED2,LOW);
    Serial.println("RUN ALL");
  }
  else if(stateStr == "CH12OFF")
  {
    digitalWrite(LED1,HIGH);
    digitalWrite(LED2,HIGH);
    Serial.println("OFF ALL");
  }
  
  if(stateStr == "CH1ON")
  {
    digitalWrite(LED1,LOW); 
    Serial.println("ON Pump 1 (Red)");
    delay(1000*30);
    digitalWrite(LED1,HIGH);
    Serial.println("OFF Pump 1 (Red)");
    microgear.connect(APPID);
    
  }
  /*
  else if(stateStr == "CH1OFF")
  {
    digitalWrite(LED1,HIGH);  
    Serial.println("OFF LED 1");
  }
  */
  
  if(stateStr == "CH2ON")
  {
    digitalWrite(LED2,LOW); 
    Serial.println("ON LED 2");
    delay(1000*120);
    digitalWrite(LED2,HIGH);
    Serial.println("OFF LED 2");
    microgear.connect(APPID);
  }
  /*
  else if(stateStr == "CH2OFF")
  {
    digitalWrite(LED2,HIGH);  
    Serial.println("OFF LED 2");
  }
  */
  
  if(stateStr == "CH3ON")
  {
    digitalWrite(LED1,LOW); 
    digitalWrite(LED2,LOW); 
    Serial.println("ON LED 1 & 2");
    delay(1000*30);
    digitalWrite(LED1,HIGH);
    delay(1000*90);
    digitalWrite(LED2,HIGH);
    Serial.println("OFF LED 1 & 2");
    microgear.connect(APPID);
  }
  /*
  else if(stateStr == "CH3OFF")
  {
    digitalWrite(LED1,HIGH); 
    digitalWrite(LED2,HIGH);  
    Serial.println("OFF LED 1 & 2");
  }
  */
}
void onFoundgear(char* attribute, uint8_t* msg, unsigned int msglen){
  Serial.print("Found new member --> ");
  for(int i=0; i<msglen; i++)
  Serial.print((char)msg[i]);
  Serial.println();
}
void onLostgear(char* attribute, uint8_t* msg, unsigned int msglen){
  Serial.print("Lost member --> ");
  for(int i=0; i<msglen; i++)
  Serial.print((char)msg[i]);
  Serial.println();
}
/*When a microgear is connected, dothis*/
void onConnected(char* attribute, uint8_t* msg, unsigned int msglen){
  Serial.println("Connected to NETPIE...");
  /* Set the alias of this microgearALIAS */
  microgear.setAlias(ALIAS);
  microgear.setName("led");//
}
void setup()
{
  microgear.on(MESSAGE,onMsghandler);
  microgear.on(PRESENT,onFoundgear);
  microgear.on(ABSENT,onLostgear);
  microgear.on(CONNECTED,onConnected);

  Serial.begin(115200);
  Serial.println("Starting...");
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  digitalWrite(LED1,HIGH);
  digitalWrite(LED2,HIGH);

  if(WiFi.begin(ssid, password))
  {
    while(WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
  }
  Serial.println("WiFiconnected");
  microgear.init(KEY,SECRET,ALIAS);
  microgear.connect(APPID);
}
void loop()
{
  
  if(microgear.connected()){
    microgear.loop();
    bool led1 = digitalRead(LED1);
    bool led2 = digitalRead(LED2);
    String status_LED1;
    String status_LED2;
    String status_AllLED;

    if (led1 < 2) {
      if(led1 == LOW){
        status_LED1="CH1ON";
      }
      else {
        status_LED1="CH1OFF";
      }
    }

    if (led2 < 2) {
      if(led2 == LOW) {
        status_LED2="CH2ON";
      }
      else {
        status_LED2="CH2OFF";
      }
    }
    
    if (led1 < 2 && led2 < 2) {
      if(led1 == LOW && led2 == LOW) {
        status_AllLED="CH3ON";
      }
      else {
        status_AllLED="CH3OFF";
      }
    }
    
    
    String data2freeboard = status_LED1 + "," + status_LED2 + "," + status_AllLED;
    microgear.chat(TARGET,data2freeboard);
    Serial.print("Send message to NetPie: ");
    Serial.println(data2freeboard);
  }
  else{
    Serial.println("connection lost, reconnect...");
    if(timer >= 0){
      microgear.connect(APPID);
      timer=0;
    }
    else timer += 1000;
  }
  delay(1000);
}
