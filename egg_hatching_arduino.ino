#include<SoftwareSerial.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>

#define DHTTYPE    DHT11     // DHT 11
#define DHTPIN 5
DHT_Unified dht(DHTPIN, DHTTYPE);
SoftwareSerial esp8266(3,4);
LiquidCrystal_I2C lcd(0x27, 16, 2);

int datecount,today,hatchday;

void setup() {
  esp8266.begin(9600);
  Serial.begin(115200);
  dht.begin();
  lcd.init();
  lcd.backlight();
  sensor_t sensor;

  pinMode(13,OUTPUT);//intake fan
  pinMode(12,OUTPUT);//outtake fan
  pinMode(11,OUTPUT);//Bulb 1
  pinMode(6,OUTPUT);//Bulb2

  pinMode(10,OUTPUT);// PIR Pullup
  pinMode(9,OUTPUT);//Door Pull UP

  pinMode(8,INPUT);//PIR Sensor
  pinMode(7,INPUT);//Door Sensor
  
  digitalWrite(13,LOW);
}

void loop() {
  
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  int temperature= event.temperature;

 

  if (temperature  >= 36){digitalWrite(12,LOW); digitalWrite(11,HIGH); digitalWrite(6,HIGH);}
  else if (temperature >=35 && temperature  <= 36){digitalWrite(12,HIGH); digitalWrite(11,HIGH); digitalWrite(6,LOW);}
  else if (temperature < 35) {digitalWrite(12,HIGH); digitalWrite(11,LOW); digitalWrite(6,LOW);}

  if (digitalRead(8) ==  LOW){ digitalWrite(10,1);}
  else if (digitalRead(8) ==  HIGH){ digitalWrite(10,0);}

  if (digitalRead(7) == HIGH ) {digitalWrite(9,1); }
  else if (digitalRead(7) == LOW){digitalWrite(9,0); }


  if  (esp8266.available()>0) {
    Serial.println("ESP Available");
 char inputString[100];
    esp8266.readBytesUntil('\n', inputString, sizeof(inputString));

    char *token = strtok(inputString, " \n");
    if (token != NULL) {
      datecount = atoi(token);
      token = strtok(NULL, " \n");
    }
    if (token != NULL) {
      today = atoi(token);
      token = strtok(NULL, " \n");
    }
    if (token != NULL) {
      hatchday = atoi(token);
    }

    Serial.println("Date count"+String(datecount));
    Serial.println("Today"+String(today));
    Serial.println("Hatch day"+String(hatchday));
    lcd.setCursor(0,0);
    lcd.print("Days Count:"+String(datecount));
     lcd.setCursor(0,1);
    lcd.print("Today:"+String(today));
    lcd.setCursor(8,1);
    lcd.print("Hatch:"+String(hatchday));

      
      }
  
}
