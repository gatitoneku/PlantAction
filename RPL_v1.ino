#include <SoftwareSerial.h>

int sensor_temp = A0;
int value_temp;

#define DEBUG FALSE //comment out to remove debug msgs

//*-- Hardware Serial
#define _baudrate 9600

//*-- Software Serial
//
#define _rxpin      2
#define _txpin      3
SoftwareSerial Serial1( _rxpin, _txpin );

//*-- IoT Information
#define SSID "SSID"
#define PASS "PASSWD"
#define IP "184.106.153.149" // ThingSpeak IP Address: 184.106.153.149

// GET /update?key=[THINGSPEAK_KEY]&field1=[data 1]&field2=[data 2]...;
String GET = "GET /update?key=TOI5RNNO2E331QY2";

//TalkBack information
String thingSpeakAPI = "api.thingspeak.com";
String talkBackAPIKey = "IBQ9ORXYKZSS97NT";
String talkBackID = "11796";
String talkBackURL = "GET /talkbacks/" + talkBackID + "/commands/3224092?api_key=" + talkBackAPIKey;
const int checkTalkBackInterval = 15 * 1000; 
int sprayCount = 0;

void setup() {
   Serial1.begin( _baudrate );
   pinMode(13,OUTPUT);
   Serial1.println("AT+RST");
   sendSerial1("AT");
   delay(5000);
   Serial1.println("RECEIVED: OK\nData ready to sent!");
   connectWiFi();
  
}

float sensorRead(){
    float soilMoisture;
    float sensorValue = analogRead(A0);
    sensorValue = constrain(sensorValue, 280, 1023);
    float soil = map(sensorValue, 280, 1023, 1023, 0);
    float soilMoistureRaw = soil*(3.3/1024) ;
  
  //Volumetric Water Content is a piecewise function of the voltage from the sensor
  if (soilMoistureRaw < 1.1) {
    soilMoisture = (10 * soilMoistureRaw) - 1;
  }
  else if (soilMoistureRaw < 1.3) {
    soilMoisture = (25 * soilMoistureRaw) - 17.5;
  }
  else if (soilMoistureRaw < 1.82) {
    soilMoisture = (48.08 * soilMoistureRaw) - 47.5;
  }
  else if (soilMoistureRaw < 2.2) {
    soilMoisture = (26.32 * soilMoistureRaw) - 7.89;
  }
  else {
    soilMoisture = (62.5 * soilMoistureRaw) - 87.5;
  }
   Serial1.print("Current soil moisture is: ");
   Serial1.println(soilMoisture);
   return soilMoisture;
}

void sprayWater(int value_temp)  {
  Serial1.println("NOW SPRAYING...");
   if(value_temp < 30) {
    digitalWrite(13, HIGH);
    sprayCount++;
    delay(3000);
  }
  digitalWrite(13,LOW);
}
void loop() {
  value_temp = sensorRead();      //get sensor reading
  sprayWater(value_temp);         //spray water or not depends on reading
  String temp =String(value_temp);// turn integer to string 
  updateTS(temp);                 //update web
  checkTalkBack();                //check if there is a command to manual spray
  delay(1000); //
}

//----- update the  Thingspeak
void updateTS( String T)
{
  // ESP8266 Client
  String cmd = "AT+CIPSTART=\"TCP\",\"";// Setup TCP connection
  cmd += IP;
  cmd += "\",80";
  sendSerial1(cmd);

  cmd = GET + "&field1=" + T +"\r\n"; //"GET /update?key=TOI5RNNO2E331QY2" + "&field1=" +
                                      //"<data>" 
  Serial1.print( "AT+CIPSEND=" );
  delay(1000);
  Serial1.println( cmd.length() );
  delay(1000);
  Serial1.print(cmd);
  delay(2000);
  if( Serial1.find("OK") )
  {
    Serial1.println( "RECEIVED: OK" );
  }
  else
  {
    Serial1.println( "RECEIVED: Error\nExit2" );
  }
}

void sendSerial1(String cmd)
{
  Serial1.print("SEND: ");
  Serial1.println(cmd);
  delay(5000);
}

boolean connectWiFi()
{
  Serial1.println("AT+CWMODE=1");//WiFi STA mode - if '3' it is both client and AP
  delay(2000);
  //Connect to Router with AT+CWJAP="SSID","Password";
  // Check if connected with AT+CWJAP?
  String cmd="AT+CWJAP=\""; // Join accespoint
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  sendSerial1(cmd);
  delay(1000);
  if(Serial1.find("OK"))
  {
    Serial1.println("RECEIVED: OK");
    return true;
  }
  else
  {
    Serial1.println("RECEIVED: Error");
    return false;
  }

  cmd = "AT+CIPMUX=0";// Set Single connection
  sendSerial1( cmd );
  if( Serial1.find( "Error") )
  {
    Serial1.print( "RECEIVED: Error" );
    return false;
  }
}


void checkTalkBack()
{  
  String talkBackCommand;
  char charIn;
  String cmd;

  // Make a HTTP GET request to the TalkBack API:
  cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  Serial1.print("SEND: ");
  Serial1.println(cmd);
  delay(5000);
   if( Serial1.find( "Error" ) )
  {
    Serial1.print( "RECEIVED: Error\nExit1" );
    //return;
  }
  cmd = talkBackURL + "\r\n";
  Serial1.print("AT+CIPSEND=");
  delay(1000);
  Serial1.println(cmd.length());
  delay(10000);
  
    Serial1.print(cmd);
    digitalWrite(13,LOW);  //TURN OFF FIRST
    if (Serial1.find("TURN_ON")) {  //TURNS ON THE LED AFTER GET  
    Serial.println("executing:");
    Serial.println(talkBackCommand);
    digitalWrite(13, HIGH);
    delay(10000);
    sprayCount++; 
  }

  Serial1.println( "AT+CIPCLOSE" );//close TCP connection 
  Serial1.flush(); 
  delay(1000);
}



