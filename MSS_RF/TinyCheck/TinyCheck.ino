#include <SoftwareSerial.h>

SoftwareSerial mser(-1,4);

void setup() {
  // put your setup code here, to run once:
pinMode(3,OUTPUT);
pinMode(4,OUTPUT);
mser.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(3,1);
delay(100);
digitalWrite(3,0);
delay(100);
mser.println("working...");


}
