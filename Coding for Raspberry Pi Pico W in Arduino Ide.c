#include <Wire.h>   
#include <WiFi.h>  
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h> 
LiquidCrystal_I2C lcd(0x27,16,2); 
unsigned int t;  
unsigned int c1,c2,c3,c4,c5,ct,cin,a1,a2,a3,a4,a5;
const char* ssid = "electronics"; // your network SSID (name)
const char* password = "wifihotspot"; // your network password
const char* api_key = "thingspeak api"; // your network password
const char* server = "184.106.153.149"; 

#define ir1 14
#define ir2 22

#define s1 6
#define s2 7
#define s3 10
#define s4 11
#define s5 9

bool stp1,stp2,stp3,stp4,stp5;
bool s1d,s2d,firstTime;

void setup()  
{ 
  Wire.begin(); 
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
  pinMode(s1, INPUT_PULLUP);
  pinMode(s2, INPUT_PULLUP);
  pinMode(s3, INPUT_PULLUP);
  pinMode(s4, INPUT_PULLUP);
  pinMode(s5, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);  
  lcd.print(" BUS PASSENGER  ");  
  lcd.setCursor(0,1);  
  lcd.print("COUNTING SYSTEM");  
  delay(1000);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println();
  digitalWrite(LED_BUILTIN, HIGH);    
  delay(500);                       
  digitalWrite(LED_BUILTIN, LOW);     
  delay(500);  
  lcd.clear();  
  stp1=1;
  stp2=0;
  stp3=0;
  stp4=0;
  stp5=0;
 while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  }  
  void loop()   
  { 
    busStopSEL();
    lcd.setCursor(0,0);  
    lcd.print("T:");
    lcdINT3(ct);
    lcd.print(" ");
    lcd.print("IN:");
    lcdINT2(cin);
    lcd.print(" ");
    lcdINT3(t);
    lcd.print(" ");
    lcd.setCursor(0,1); 
    lcdINT2(c1);
    lcd.print(" ");
    lcdINT2(c2);
    lcd.print(" ");
    lcdINT2(c3);
    lcd.print(" ");
    lcdINT2(c4);
    lcd.print(" ");
    lcdINT2(c5);

    if(digitalRead(ir1) == 0)
    {
      while(digitalRead(ir1) == 0);
      s1d=1;
      if(s2d==1)
      {
        s1d=0;
        if(cin>=1)cin--;
        s2d=0;
      }
    }
    if(digitalRead(ir2) == 0)
    {
      while(digitalRead(ir2) == 0);
      s2d=1;
      if(s1d==1)
      {
        s2d=0;
        ct++;
        cin++;
        if(stp1){
          c1++;
          a1=cin;}
        if(stp2){
          c2++;
          a2=cin;}
        if(stp3){
          c3++;
          a3=cin;}
        if(stp4){
          c4++;
          a4=cin;}
        if(stp5){
          c5++;
          a5=cin;}
        s1d=0;
      }
    }
    t++;
    if(t>999)
    {
      t=0;
      lcd.clear(); 
      lcd.setCursor(0,0);  
      lcd.print("uploading1.."); 
     SEND_status1();
    lcd.clear(); 
    }
  }

void SEND_status1()
{
  lcd.setCursor(0, 1);
  lcd.print("Uploading  ");

  WiFiClient client;

  // connect to the ThingSpeak server
  if (client.connect(server, 80)) {
    Serial.println("Connected to ThingSpeak");

    // create a HTTP POST request
    String post_data = "api_key=" + String(api_key) + "&field1=" + String(c1) + "&field2=" + String(c2)+"&field3=" + String(c3) + "&field4=" + String(c4)+"&field5=" + String(c5) + "&field6=" + String(cin)+"&field7=" + String(ct) ;
     client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(post_data.length());
    client.print("\n\n");
    client.print(post_data);
   
   

    // wait for the response
    int timeout = millis() + 5000;
    while (client.available() == 0) {
      if (timeout - millis() < 0) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }

    // read and print the response
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }

    Serial.println();
    Serial.println("Closing connection");
  }}
 
void busStopSEL()
{
  if(digitalRead(s1)==0)
    {
        lcd.clear(); 
        stp1=1;
        stp2=0;
        stp3=0;
        stp4=0;
        stp5=0;
        lcd.setCursor(0,0);  
        lcd.print("STOP 1"); 
        delay(1000);
        lcd.clear();  
    }
    if(digitalRead(s2)==0)
    {
        lcd.clear(); 
        stp1=0;
        stp2=1;
        stp3=0;
        stp4=0;
        stp5=0;
        lcd.setCursor(0,0);  
        lcd.print("STOP 2"); 
        delay(1000);
        lcd.clear();  
    }
    if(digitalRead(s3)==0)
    {
        lcd.clear(); 
        stp1=0;
        stp2=0;
        stp3=1;
        stp4=0;
        stp5=0;
        lcd.setCursor(0,0);  
        lcd.print("STOP 3"); 
        delay(1000);
        lcd.clear();  
    }
    if(digitalRead(s4)==0)
    {
        lcd.clear(); 
        stp1=0;
        stp2=0;
        stp3=0;
        stp4=1;
        stp5=0;
        lcd.setCursor(0,0);  
        lcd.print("STOP 4"); 
        delay(1000);
        lcd.clear();  
    }
    if(digitalRead(s5)==0)
    {
        lcd.clear(); 
        stp1=0;
        stp2=0;
        stp3=0;
        stp4=0;
        stp5=1;
        lcd.setCursor(0,0);  
        lcd.print("STOP 5"); 
        delay(1000);
        lcd.clear();  
    }
}
void lcdINT2(unsigned int val)
{
  lcd.print(val/10);
  lcd.print(val%10); 
}
void lcdINT3(unsigned int val)
{
  lcd.print(val/100);
  lcd.print((val%100)/10); 
  lcd.print((val%100)%10); 
}
