#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
#define LED_PIN 8

int ReceivedMessage[1] = {000}; 

int ch_width_1 = 0;
int ch_width_2 = 0;
int ch_width_3 = 0;
int ch_width_4 = 0;

Servo ch1;
Servo ch2;
Servo ch3;
Servo ch4;

struct Signal {
byte throttle;      
byte pitch;
byte roll;
byte yaw;

};
Signal data;
const uint64_t pipeIn = 0xE9E8F0F0E1LL;
RF24 radio(9, 10); 
void ResetData()
{

data.roll = 127;   
data.pitch = 127;  
data.throttle = 12;
data.yaw = 127;   
}
void setup()
{
  
  ch1.attach(2);
  ch2.attach(3);
  ch3.attach(4);
  ch4.attach(5);
 
  
  ResetData();
  radio.begin();
  radio.openReadingPipe(1,pipeIn);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.startListening();
  pinMode(2,OUTPUT);
  pinMode(LED_PIN, OUTPUT); 
}
unsigned long lastRecvTime = 0;
void recvData()
{
while ( radio.available() ) {
radio.read(&data, sizeof(Signal));
lastRecvTime = millis();   // receive the data | data alınıyor
}
}
void loop()
{
  while (radio.available())
  {
    radio.read(ReceivedMessage, 1); // Read information from the NRF24L01

    if (ReceivedMessage[0] == 111) // Indicates switch is pressed
    {
      digitalWrite(LED_PIN, HIGH);
      digitalWrite(2,127);
    }
    else
    {
       digitalWrite(LED_PIN, LOW);
    }
    delay(2);
   }
recvData();
unsigned long now = millis();
if ( now - lastRecvTime > 1000 ) {
ResetData(); // Signal lost.. Reset data | Sinyal kayıpsa data resetleniyor
}
ch_width_4 = map(data.yaw,      0, 255, 1000, 2000);     
ch_width_2 = map(data.pitch,    0, 255, 1000, 2000);     
ch_width_3 = map(data.throttle, 0, 255, 1000, 2000);     
ch_width_1 = map(data.roll,     0, 255, 1000, 2000);     


ch1.writeMicroseconds(ch_width_1);
ch2.writeMicroseconds(ch_width_2);
ch3.writeMicroseconds(ch_width_3);
ch4.writeMicroseconds(ch_width_4);
}
