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

int lowTemp = 40;
int middleTemp = 60;
int highTemp = 80;

int lowCooler = 40;
int middleCooler = 60;
int highCooler = 80;

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
 
LiquidCrystal_I2C lcd(0x27,16,2);   // Задаем адрес и размерность дисплея

void setup() {
  Serial.begin(9600);

  enc.setType(TYPE2);
  enc.setTickMode(AUTO);

  dht.begin();

  lcd.init();                       // Инициализация lcd             
  lcd.backlight();                  // Включаем подсветку
  lcd.createChar(1, degree);        // Создаем символ под номером 1
}

void loop() {
  enc.tick();  

  if (enc.isTurn()) {
    if (enc.isRight()) count++;
    if (enc.isLeft()) count--;
    Serial.println(count);
  }

  if (enc.isClick()) Serial.println("click");

  //float h = dht.readHumidity(); //Измеряем влажность
  //float t = dht.readTemperature(); //Измеряем температуру
  //if (isnan(h) || isnan(t)) {  // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу
    //Serial.println("Ошибка считывания");
    //return;
  //}

  //Serial.print("Влажность: ");
  //Serial.print(h);
  //Serial.print(" %\t");
  //Serial.print("Температура: ");
  //Serial.print(t);
  //Serial.println(" *C "); //Вывод показателей на экран 

  // Считывание данных температуры занимает около 250 milliseconds!
  // Показания датчика также могут быть "устаревшими" на 2 секунды (это очень медленный датчик)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);
  
  // Выводим показания влажности и температуры
  lcd.setCursor(0, 0);            // Устанавливаем курсор в начало 1 строки
  lcd.print("Hum  =     % ");     // Выводим текст
  lcd.setCursor(7, 0);            // Устанавливаем курсор на 7 символ
  lcd.print(h, 1);                // Выводим на экран значение влажности
  lcd.setCursor(0, 1);            // Устанавливаем курсор в начало 2 строки
  lcd.print("Temp =     \1C ");   // Выводим текст, \1 - значок градуса
  lcd.setCursor(7, 1);            // Устанавливаем курсор на 7 символ 
  lcd.print(t,1);                 // Выводим значение температуры
}
