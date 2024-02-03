#define CLK 2
#define DT 3
#define SW 4

#include <GyverEncoder.h>

Encoder enc(CLK, DT, SW);

int val = 0;

void setup() {
  Serial.begin(9600);
  enc.setType(TYPE2);
  enc.setTickMode(AUTO);
}

void loop() {
  enc.tick();  

  if (enc.isTurn()) {
    if (enc.isRight()) val++;
    if (enc.isLeft()) val--;
    Serial.println(val);
  }

  if (enc.isClick()) Serial.println("click"); 
}
