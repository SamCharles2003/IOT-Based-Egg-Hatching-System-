#define BLYNK_TEMPLATE_ID           "" // Your Credentials
#define BLYNK_TEMPLATE_NAME         ""
#define BLYNK_AUTH_TOKEN            ""

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include<SoftwareSerial.h>

char ssid[] = "";
char pass[] = "";

int previousdate = 0;
int datecount = 0;
int hatchdate;
int hatchmonth;


SoftwareSerial arduino(0,2); // D3, D4
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

WiFiServer server(5050);
String header;

String weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

std::pair<int, int> date();  

void setup() {
  pinMode(16, OUTPUT); //D0
  pinMode(5, OUTPUT); //D1
  pinMode(4, OUTPUT); //D2
  pinMode(13,INPUT); //D7 (DOOR)
  pinMode(15, INPUT); //D8  (PIR)

  arduino.begin(9600);
  Serial.begin(115200);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timeClient.begin();
  timeClient.setTimeOffset(0);

  Blynk.logEvent("ready","Egg hatching is Equipped!");

}

void loop() {
  Blynk.run();
  int today, month;
  std::tie(today, month) = date();  // Use std::tie to destructure the returned values

  if (previousdate != today) {
    datecount = datecount + 1;
    previousdate = today;
  }
  if (today == hatchdate && month == hatchmonth ) {
    Serial.println("Hatching Day!");    
    Blynk.logEvent("day", "Today's the hatching Day");
    datecount = 0;
    previousdate= 0;
  }
  if ( digitalRead(13)== LOW) {Serial.println("Hatch opened");   Blynk.logEvent("open", "Hatch Box was Opened!");}
  if ( digitalRead(15)== HIGH)  {Serial.println("Movement"); Blynk.logEvent("pir", "Anonymous Movemnet detected!");}
  Blynk.virtualWrite(V2, String(datecount));
  Blynk.virtualWrite(V3,String(hatchdate)+ "/"+String(hatchmonth));
  Blynk.virtualWrite(V4,String(today)+ "/"+String(month));
  arduino.print(datecount);
 arduino.print(" ");
arduino.print(String(today) + "/" + String(month));
arduino.print(" ");
arduino.print(String(hatchdate) + "/" + String(hatchmonth));
arduino.print("\n");
  
}

std::pair<int, int> date() {
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  String formattedTime = timeClient.getFormattedTime();
  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();
  int currentSecond = timeClient.getSeconds();
  String weekDay = weekDays[timeClient.getDay()];
  struct tm *ptm = gmtime((time_t *)&epochTime);
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon + 1;

  return std::make_pair(monthDay, currentMonth);
}

BLYNK_WRITE(V0) {
  hatchdate = param.asInt();
}

BLYNK_WRITE(V1) {
  hatchmonth = param.asInt();
}
