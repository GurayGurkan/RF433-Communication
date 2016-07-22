#include <VirtualWire.h>

const unsigned char seg7[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67};
int data;
unsigned short int n=0;
unsigned short int GND_1=6;
unsigned short int GND_2=5;
unsigned short int RXpin=2;
unsigned short int signalLED=3;
char d[]={0,0};


void setup()
{
  vw_set_ptt_inverted(true); 
 vw_setup(2000);	 // Bits per sec
 vw_set_rx_pin(RXpin);
 vw_rx_start();       // Start the receiver PLL running
 //Serial.begin(2400);
 DDRB |= 0x3F; // Digital pin 8-13 cikis >>> 7 seg.: a-b-c-d-e-f
 PORTB |=0x20; // Digital pin 8-13 Logic 1 (+3.3V) 
 pinMode(7,OUTPUT); // Digital pin 7 cikis >>> 7 seg.: g
 
 pinMode(GND_1,OUTPUT);
 pinMode(GND_2,OUTPUT);
 digitalWrite(7,'HIGH'); // Digital pin 7 Logic 1.
 digitalWrite(GND_1,HIGH);
 digitalWrite(GND_2,HIGH);
 pinMode(2,OUTPUT);
 pinMode(signalLED,OUTPUT);
 

}

void segment7(short int s){
PORTB = (seg7[s] & 0x3F);
digitalWrite(7,((seg7[s] & 0x40) >>6)) ;
}



void display_BCD(unsigned short v)
{
short i;
  for (i = 0; i < 2; i++)
  {
     d[i] = v % 10;
     v = v/10;
  }
}


void loop()
{
  uint8_t buf[2];
  uint8_t buflen=2;

    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
      digitalWrite(signalLED,HIGH);
      digitalWrite(GND_1,LOW);
      segment7(buf[1]);
      delay(25);
      digitalWrite(GND_1,HIGH);
      digitalWrite(GND_2,LOW);
      segment7(buf[0]);
      delay(25);
      digitalWrite(GND_2,HIGH);
      digitalWrite(signalLED,LOW);
    }
    delay(10);
}

