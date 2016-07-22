

#include <VirtualWire.h>

const unsigned char seg7[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67};
unsigned short int n=0;
unsigned short int GND_1=6;
unsigned short int GND_2=5;
unsigned short int FWDpin =4;
unsigned short int REVpin =3;
unsigned short int TXpin =2;
unsigned short int pressed=0;
long darbeperyot=0;
char d[]={0,0};


void setup()
{
 Serial.begin(2400);

 
 vw_set_ptt_inverted(true); 
 vw_set_tx_pin(TXpin); //without PWM
 vw_setup(2000);	 // Bits per sec
 DDRB |= 0x3F; // Digital pin 8-13 cikis >>> 7 seg.: a-b-c-d-e-f
 PORTB |=0x20; // Digital pin 8-13 Logic 1 (+3.3V) 
 pinMode(7,OUTPUT); // Digital pin 7 cikis >>> 7 seg.: g 
 
 pinMode(GND_1,OUTPUT);
 pinMode(GND_2,OUTPUT);

 digitalWrite(7,'HIGH'); // Digital pin 7 Logic 1.
 
 digitalWrite(GND_1,HIGH);
 digitalWrite(GND_2,HIGH);
 
pinMode(FWDpin,INPUT); 
pinMode(REVpin,INPUT);

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
  
  display_BCD(n);
  digitalWrite(GND_1,LOW);
  segment7(d[1]);
  delay(25);
  digitalWrite(GND_1,HIGH);
  digitalWrite(GND_2,LOW);
  segment7(d[0]);
  delay(25);
  digitalWrite(GND_2,HIGH);
  if (digitalRead(FWDpin)==HIGH && ((millis()-darbeperyot)>500) && pressed==0)
   {
    if (n>98) 
    n=0;
    else
    n++;  
    
    pressed=1;
    darbeperyot = millis();
    vw_send((uint8_t *)d,2);
    vw_wait_tx(); 
   } //endif FWDpin
  
   
    if (digitalRead(REVpin)==HIGH && ((millis()-darbeperyot)>500) && pressed==0)
   {
    if (n<1)
    n=0;
    else
     n--;
    darbeperyot = millis();
    vw_send((uint8_t *)d,2);
    vw_wait_tx(); 
   }  
   pressed=0;
}

