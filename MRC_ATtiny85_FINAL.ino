#include <TinyWireM.h>
#include <Tiny4kOLED.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
volatile int revs = 0;
volatile int cheddar = 0;
unsigned long bit;
void setup() {
  pinMode(0,OUTPUT); // Sets pin 0 as an output for the pin-change interrupt
  pinMode(1,INPUT);
  digitalWrite(1, LOW);
  pinMode(3,INPUT); 
  digitalWrite(3, LOW);
  pinMode(2,OUTPUT); //SCL for OLED
  pinMode(4,INPUT); // removing floating variables by setting unused pins to input to save power 
  pinMode(5,INPUT);
  sbi(GIMSK,PCIE); // turns on pin change interrupt
  sbi(PCMSK,PCINT3); // defines pin 3 (IR Sensor) as affected by interrupt
  sbi(PCMSK,PCINT1); // defines pin 1 (Hall-Effect Sensor) as affected by interrupt
  oled.begin();              
  oled.clear();
  oled.setFont(FONT8X16);
}
void loop() {//Main loop checks if IR sensor is triggered to either increase or display count
bit=pulseIn(3,HIGH,50000UL);
if(bit<4000){
    revs++;
    if(revs>=20000){ 
    cheddar++; //if revolution number is at 10k, this variable goes up by 1
    revs=0;    //Resets counter variable to 0
    }
}
if(bit>4000){
     oled.on(); 
     oled.clear();
     oled.print(long((revs)/2)); //Displays revoltutions <10k
     oled.setCursor(0, 2);
     oled.print(long(cheddar)); //Displays multiples of 10k to the right of <10k count
     delay(4000);    
     }
digitalWrite(3,LOW);
system_sleep();          
}
void system_sleep() {
  oled.off();
  cbi(ADCSRA, ADEN); // turns ADC off
  sleep_bod_disable(); 
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sets the sleep mode
  sleep_mode(); // turns sleep mode on
  sbi(ADCSRA,ADEN); // turns ADC back on
}
ISR(PCINT0_vect){//Occurs whenever the HE or IR sensors sense pin-change
}
