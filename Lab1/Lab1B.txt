#include <TimerOne.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h> 
#include <stdlib.h> 
 

#define F_CPU 16000000

//Pin variables
int flashLED = 3;
int varyLED = 6;
int IR = 0;

//adjustement variables
long interval = 1000000;

int bright = 255;


void setup()
{
  Serial.begin(9600);

  //pullup resistor
  DDRB = 0;
  PORTB |= (1<<1);
  
  //turn on light at PB2
  DDRB |= (1<<flashLED);
  PORhLED);
  
  //PWM timer0
  //Fast and Non-Inverted PWM
  TCCR0A |= (1<<COM0A1) | (1<<WGM01) | (1<<WGM00); 
  //Prescaler set to clkIO/64
  TCCR0B |= (1<<CS01) | (1<<CS00); 
  TCNT0 = 0;
  
  //compare register value set to 0
  OCR0A = 0;
  
  //turn on PD6
  DDRD |= (1<<varyLED);
  PORTD |= (1<<varyLED);

  //settup for IR_Sensor
  //Setting PIN 14 to Input
  DDRC |= (0<<IR);

  //choosing channel one
  uint8_t ch=ch&0b00000001;

  //setting channel and setting reference voltage to 5v
  ADMUX |= ch | (1<<REFS0);

  //ADEN enables EDC
  //Others set to one sets the Prescaeler to 128 to achieve ADC in range of 50kHz - 200 kHz
  ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);

  //Creates timer1 interrupt with interval and routine
  Timer1.initialize(interval);
  Timer1.attachInterrupt(IR_Sensor);
  

};

void IR_Sensor()
{
  //ADC setting up conversion
  ADCSRA|=(1<<ADSC);

  //waiting for conversion to complete
  while(!(ADCSRA & (1<<ADIF))){};

  //conversion is complete
  ADCSRA|=(1<<ADIF);

  //print information to Monitor
  Serial.print("IR: ");
  Serial.println(ADC);
}

void blink()
{
  //test if PIN B2 is set to High
  if(PINB & (1<<flashLED))
  {
    //close PIN B2
    PORTB &= ~(1<<flashLED);
  }
  else
  {
    //open PIN B2
    PORTB |= (1<<flashLED);
  }
  
};

void toggle_LED()
{
  //Increments the value of the register by a quarter of 255
  OCR0A += 64;
  _delay_ms(1000);

 //prints information to monitor
 Serial.print("Brightness: ");
 Serial.println(OCR0A,DEC);
}

void loop()
{
  blink();
  toggle_LED();

  Serial.println("Please enter br: followed by a value not exceeding 255 to change Brightness");
  Serial.println("Please enter in: followed by a value to change Interrupt Interval");
  Serial.println("Both can be entered simultanously and out of order, but must be seperated by a comma");
  
  if(Serial.available())
  {
    String hold;
    String tmp;
    int pos1;
    int pos2;
    bool loop = true;

    //reads Serial String
    hold = Serial.readString();

    Serial.println("Enetered " + hold);

    //loops through the string
    for(int i=0; i<hold.length();i++)
    {
      //if br: is found enters the block
      if(hold.substring(i,i+3).equals("br:"))
      {
        //stores the beginning of the value
        pos1 = i+3;
        
        while(loop)
        {
          //searches for the end which is a coma, space or new line 
          tmp = hold.substring(i+3, i+4);
          
          if(tmp.equals(",") || tmp.equals(" ") || tmp.equals("\n"))
          {
            pos2 = i+3;
            tmp = hold.substring(pos1, pos2);
            
            //Value is held in tmp and converted into an integer
            bright = tmp.toInt();

            //tests if the value is safe i.e. does not exceed 255
            if(bright-255 > 0)
            {
              Serial.println("Brightness Value to high please choose another");
              bright = 255;
            }

            //prints new brightness
            Serial.print("New Brightness ");
            Serial.println(bright);

            //recallabrates i and set loop to false to exit while
            i--;
            loop = false;
          }
         i++;
        }
      }
      //same logic and structure as above
      else if(hold.substring(i,i+3).equals("in:"))
      {
        pos1 = i+3;
        
        while(loop)
        {
          tmp = hold.substring(i+3, i+4);
          
          if(tmp.equals(",") || tmp.equals(" ") || tmp.equals("\n"))
          {
            pos2 = i+3;
            tmp = hold.substring(pos1, pos2);
            interval = tmp.toInt();

            Serial.print("New Interval ");
            Serial.println(interval);
            
            i--;
            loop = false;
          }
         i++;
        }
      }
      loop = true;
    }

    //Brightness of the toggle LED is changed
    OCR0A = bright;

    //Interrupt interval is changed
    Timer1.setPeriod(interval);
  }
  
};