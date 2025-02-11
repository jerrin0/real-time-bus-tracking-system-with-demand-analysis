#include <SoftwareSerial.h>
#include<TinyGPS++.h>

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;
bool first_ser_init,firstTime;
unsigned int sendT;
long int lan2,log2;
double lan1,log1;


TinyGPSPlus gps;
SoftwareSerial GPS(RXPin, TXPin);

void setup(){
  Serial.begin(9600);
  GPS.begin(GPSBaud);
  init_wifi();

}
void loop(){
  if (GPS.available() > 0){
    gps.encode(GPS.read());
    if (gps.location.isUpdated()){
      Serial.print("Latitude= "); 
      lan1=gps.location.lat();
      lan2=lan1*10000000; // need to convert back to original form in your application
      Serial.print(gps.location.lat(), 7);
      Serial.print(" Longitude= "); 
      log1=gps.location.lng();
      log2=log1*10000000; // need to convert back to original form in your application
      Serial.println(gps.location.lng(), 7);}
      sendT++;}
 
  if(sendT>1000)
  {
    
    sendT=0;
    SEND_status();
   
  }
}
void enter_wifi()
{
  Serial.write(0x0d);
  Serial.write(0x0a);
  delay(200);
}
void init_wifi()
{
  Serial.print("AT");
  enter_wifi();
  delay(500);
  Serial.print("ATE0");
  enter_wifi();
  delay(500);
  Serial.print("AT+GMR");
  enter_wifi();
}

void refresh_WiFi_List()
{
  Serial.print("AT+CWLAP");
  enter_wifi();
  delay(500);
}

void connect_WiFi()
{
  Serial.print("AT+CWJAP=");
  Serial.write('"');
  Serial.print("electronics"); 
  Serial.write('"');
  Serial.write(',');
  Serial.write('"');
  Serial.print("wifihotspot"); 
  Serial.write('"');
  enter_wifi();
}

void get_IP()
{
  Serial.print("AT+CIFSR");
  enter_wifi();

}
void SEND_status()
{
  
  if(first_ser_init==0)
  {
    Serial.print("AT+CIPMUX=1");
    enter_wifi();
    first_ser_init=1;
  }
  Serial.print("AT+CIPSTART=4");
  Serial.write(',');
  Serial.write('"');
  Serial.print("TCP");
  Serial.write('"');
  Serial.write(',');
  Serial.write('"');
  Serial.print("184.106.153.149");
  Serial.write('"');
  Serial.write(',');
  Serial.print("80");
  enter_wifi();
  Serial.print("AT+CIPSEND=4");
  Serial.write(',');
  Serial.print("80");
  enter_wifi();
  delay(2000);
  Serial.print("GET /update?key=U7Q0IJ71LOY211HP&field6=");
  Serial.print(lan2);
  Serial.print("&field7=");
  Serial.print(log2);
  enter_wifi();
  delay(2000);
  Serial.print("GET /update?key=U7Q0IJ71LOY211HP&field6=");
  Serial.print(lan2);
  Serial.print("&field7=");
  Serial.print(log2);
  enter_wifi();
  delay(2000);
}

 