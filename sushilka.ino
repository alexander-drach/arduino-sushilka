#define CLK 2 // энкодер вращение
#define DT 3 // энкодер вращение
#define SW 4 // энкодер кнопка

#define DHTPIN 8 // пин датчика влажности/температуры
#define DHTTYPE DHT11 // тип датчика DHT 11

#include <GyverEncoder.h>          // Добавляем необходимые библиотеки
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#define DHTPIN 8                   // пин для сигнала поступающего с датчика

Encoder enc(CLK, DT, SW);          // Инициализация энкодера
DHT dht(DHTPIN, DHTTYPE);          // Инициация датчика влажности/температуры

int count = 0;                     // значение счетчика
int temp;
int cooler;

byte degree[8] =                    // кодируем символ градуса
{
  B00111,
  B00101,
  B00111,
  B00000,
  B00000,
  B00000,
  B00000,
};    
 
LiquidCrystal_I2C lcd(0x27, 20, 4);   // Задаем адрес и размерность дисплея

volatile int counter = 0;   // счётчик
volatile bool encFlag = 0;  // флаг поворота

void setup() {
  Serial.begin(9600);

  attachInterrupt(0, encIsr, CHANGE);
  attachInterrupt(1, encIsr, CHANGE);

  enc.setType(TYPE2);
  enc.setTickMode(AUTO);

  dht.begin();

  lcd.init();                       // Инициализация lcd             
  lcd.backlight();                  // Включаем подсветку
  lcd.createChar(1, degree);        // Создаем символ под номером 
  
  lcd.setCursor(0, 0);              // Устанавливаем курсор в начало 1 строки
  lcd.print("Program: ");           // Выводим текст "Programm"
  lcd.setCursor(0, 2);              // Устанавливаем курсор в начало 1 строки
  lcd.print("Temp =     \1C ");     // Выводим текст, \1 - значок градуса
  lcd.setCursor(0, 3);              // Устанавливаем курсор в начало 1 строки
  lcd.print("Hum  =      % ");      // Выводим текст
  lcd.setCursor(9, 0);              // Устанавливаем курсор в начало 1 строкe, 10 символ
  lcd.print("LOW");                 // Выводим дефолтное название программы - LOW
  lcd.setCursor(0, 1);              // Устанавливаем курсор в начало 1 строкe, 10 символ
  lcd.print("Time: ");              // Выводим дефолтное название программы - LOW
}

void loop() {
  // enc.tick();

  // if (encFlag) {
    //Serial.println(counter);
    // encFlag = 0;
  // }

  // Считывание данных температуры занимает около 250 milliseconds!
  // Показания датчика также могут быть "устаревшими" на 2 секунды (это очень медленный датчик)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();  

  // if (enc.isTurn()) {
    //if (count < 0) count = 0;
    //if (count >= 2) count = 2;

    //if (enc.isRight()) count++;
    //if (enc.isLeft()) count--;    
  // }

  // if (enc.isClick()) Serial.println("click");
  // if (enc.isHolded()) Serial.println("holded");  

  if (count == 0) {
    String str = "LOW     ";
    temp = 0;
    cooler = 33;
    startProgramm(temp, cooler, str);

  } else if (count == 1) {
    String str = "MIDDLE";
    temp = 40;
    cooler = 66;
    startProgramm(temp, cooler, str);

  } else if (count == 2) {
    String str = "HIGH  ";
    temp = 60;
    cooler = 99;
    startProgramm(temp, cooler, str);
  }

  outPutTempHum(t,h);
}

void startProgramm(int temp, int cooller, String str) {
  lcd.setCursor(9, 0);              // Устанавливаем курсор в начало 1 строкe, 10 символ
  lcd.print(str);                  // Выводим название программы
}

void outPutTempHum(float t, float h) {
  lcd.setCursor(7, 2);              // Устанавливаем курсор в начало 3 строкe, 8 символ
  lcd.print(t,1);                   // Выводим значение температуры
  lcd.setCursor(7, 3);              // Устанавливаем курсор в начало 4 строкe, 8 символ
  lcd.print(h,1);                   // Выводим значение влажности
}

// volatile byte reset = 0, last = 0;
// void encIsr() {  
//   byte state = (PIND & 0b1100) >> 2;  // D2 + D3
//   Serial.println(state);
//   if (reset && state == 0b11) {
//     int prevCount = counter;
//     if (last == 0b10) counter++;
//     else if (last == 0b01) counter--;
//     if (prevCount != counter) encFlag = 1;    
//     reset = 0;
//   }
//   if (!state) reset = 1;
//   last = state;
// }
