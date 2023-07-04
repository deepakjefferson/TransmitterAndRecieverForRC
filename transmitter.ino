#include "nRF24L01.h" 
#include "RF24.h"
#include "SPI.h"

RF24 radio(9,10); 
int joyX;
int joyY;
struct Signal {
byte throttle;
byte fwd;
byte bwd;
byte left;
byte right;
};
Signal data;
int middle=524;
const uint64_t pipe = 0xE6E6E6E6E6E6;
void setup()
{
  Serial.begin(9600);
  radio.begin(); 
  radio.openWritingPipe(pipe); 
}
int mapJoystickValues(int val)
{
val = constrain(val,0,1023);
if ( val < middle )
val = map(val, 524, 0, 0, 255);
else
val = map(val,524,1023, 0, 255);
return val;
}
void loop()
{
joyX=analogRead(A0);
joyY=analogRead(A1);
if(joyX>middle){
data.left=0;
data.right=mapJoystickValues( joyX);
}
if(joyX<middle){
data.right=0;
data.left=mapJoystickValues( joyX);
}
if(joyY>middle){
  data.fwd=0;
data.bwd=mapJoystickValues( joyY);
}
if(joyY<middle){
  data.bwd=0;
data.fwd=mapJoystickValues( joyY);
}
data.throttle=map(analogRead(A6),0,1023,0,255);
//Serial.print("right:");
//Serial.print(data.right);
//Serial.print("\n");
//Serial.print("left:");
//Serial.print(data.left);
//Serial.print("\n");
//Serial.print("fwd:");
//Serial.print(data.fwd);
//Serial.print("\n");
//Serial.print("bwd:");
//Serial.print(data.bwd);
Serial.print("\n");
Serial.print("throttle:");
Serial.print(data.throttle);
//Serial.print("\n");
//Serial.print("\n");
radio.write(&data, sizeof(Signal));
}
