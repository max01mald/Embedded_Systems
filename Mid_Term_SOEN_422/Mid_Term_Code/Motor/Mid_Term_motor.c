//Includes 
#include <TimerOne.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <util/delay.h>

//Definitions 
#define F_CPU 16000000

//Global Variables
//Pin PB0 or D8
int oneA = 0;

//Pin PB1 or D9
int twoA = 1;

//Pin PD6 or D6
int PWM = 6;

//Setup of the Nano Pins
void setup()
{
  //pullup resistor at A0
  DDRB |= (0<<1);
  PORTB |= (1<<1);

  //Turn the Pins to Output
  DDRB |= (1<<oneA);
  DDRB |= (1<<twoA);
  DDRD |= (1<<PWM);

  //Turn Pin D8 to High
  PORTB |= (1<<oneA);

  //Turn Pin D9 to Low
  PORTB &= ~(1<<twoA);

  //Turn Pin D6 to High
  PORTD |= (1<<PWM);

  //Fast PWM with 255 (0xFF) TOP
  //Compare Match Enabled
  TCCR0A |= (1<<COM0A1) | (1<<WGM01) | (1<<WGM00);

  //Pre-Scaler of 64
  TCCR0B |= (1<<CS01) | (1<<CS00);

}

//Main to show the variation in current
//The Inversion of Direction of Flow of Current
int main(void)
{
  setup();
  
  while(true)
  {
    //5v is set on the Output Pin
    OCR0A = 255;

    _delay_ms(3000);

    //5/2v is set on the Output Pin
    OCR0A = 122;

    _delay_ms(3000);

    //5/4v is set on the Output Pin
    OCR0A = 61;

     _delay_ms(3000);

    //Turn the volatage to 0v on the Output Pin
    OCR0A = 0;

    _delay_ms(3000);

    //Reverse the Flow of volatge on the H-bridge
    //Switch the active LED (Change direction of Motor)
    //If Pin D8 is on
    if(PORTB & (1<<oneA))
    {
      //Turn Pin D8 to Low and D9 to High
      PORTB &= ~(1<<oneA);
      PORTB |= (1<<twoA);
    }
    else
    {
      //Trun Pin D8 to High and D9 to Low
      PORTB |= (1<<oneA);
      PORTB &= ~(1<<twoA);
    }
    
  }
  return 0;
}
