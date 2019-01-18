#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <MQ135.h>
#include <DHT.h>

/*온습도센서 세팅*/
#define DHTPIN 2 
#define DHTTYPE DHT11 
DHT dht(DHTPIN,DHTTYPE);
#define Humi_LED_pin 10
/*CO2센서 세팅*/
#define ANALOGPIN A0 
#define RZERO 206.85 
MQ135 gasSensor = MQ135(ANALOGPIN);
#define UV_LED_pin 11
/*LCD 세팅*/
LiquidCrystal_I2C lcd(0x27,16,2);
/*모터드라이브 세팅*/
#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7
/*블루투스 세팅*/
#define TXD_pin 9
#define RXD_pin 8
SoftwareSerial BTSerial(TXD_pin,RXD_pin);
char data = 0;
#define BT_LED_pin 12
/*UV*/
#define UV_LED 13


void setup(){
  BTSerial.begin(9600);
  delay(1000);
  pinMode(UV_LED_pin,OUTPUT);// UV 동작확인 LED
  pinMode(Humi_LED_pin,OUTPUT);
  pinMode(BT_LED_pin,OUTPUT);
  pinMode(UV_LED, OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  float rzero = gasSensor.getRZero();
  lcd.begin();
  lcd.backlight();
}
void onHumi(int h);
void onUV(float h);
void loop(){
    if(BTSerial.available())
    data =  BTSerial.read();
    if(data == '1'){
    //습도 센서
    int humidity = dht.readHumidity();  // 변수 h에 습도 값을 저장
    // 가스센서 
    float ppm = gasSensor.getPPM();
     // 작동 확인 LED
    onHumi(humidity);
    onUV(ppm);
    // LCD
    lcd.setCursor(0,0);
    lcd.print("Humidity : "); 
    lcd.print(humidity);
    lcd.setCursor(0,1);
    lcd.print("CO2 : ");
    lcd.print(ppm);
    digitalWrite(BT_LED_pin,HIGH);
    delay(1000);
    }
   else{
      lcd.setCursor(0,0);
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.clear();
        digitalWrite(UV_LED_pin,LOW);
        digitalWrite(Humi_LED_pin,LOW);
        digitalWrite(BT_LED_pin,LOW);
        digitalWrite(IN1,LOW);
        digitalWrite(IN3,LOW);
        onHumi(0);
        onUV(0);
    }
    }
void onUV(float h){ 
  if(h>0.30){
  digitalWrite(UV_LED_pin,HIGH);
  digitalWrite(UV_LED,HIGH)  ;}
  else{
  digitalWrite(UV_LED_pin,LOW);
  digitalWrite(UV_LED,LOW);}
}
void onHumi(int h){ 
  if(h>45){
  digitalWrite(Humi_LED_pin,HIGH);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  
  }else{
  digitalWrite(Humi_LED_pin,LOW);
  digitalWrite(IN1,LOW);
  digitalWrite(IN3,LOW);
  
}
}
