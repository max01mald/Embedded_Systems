//definitions
#define F_CPU 16000000
#define USART_BAUDRATE 9600
#define BAUD_PRESCALE ((F_CPU/(USART_BAUDRATE*16UL))-1)

//libraries
#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>
#include <string.h>
#include <time.h>

//Gloabal variables
//To hold value for Pin PB1
int PWM = 1;

//To hold value for Pin PD6
int PWM2 = 6;

//To hold value for the second LED's brightness
int bright = 255;

//To hold value for the second LED's interval
//Value to count to, dictating the length of the interval
//Set to -1 for default frequency
int G_count = -1;

//Value counting up to the end of an interval
//Set to -1 for default frequency
int g_count = -1;

//initiation function for UART communication
void init()
{
  //Set up BAUD rate
  UBRR0H = (BAUD_PRESCALE >> 8);
  UBRR0L = BAUD_PRESCALE;
  
  //enable Send and Recieve UART
  UCSR0B |= (1<<RXEN0) | (1<< TXEN0);

  //Set Pin PB1 to output
  DDRB |= (1<<PWM);

  //Set Pin PD6 to output
  DDRD |= (1<<PWM2);

  //Clear all interrupts
  cli();

  //PWM 1
  //Set the TOP of the phase correct PWM for Mode 10
  //TOP = 15625
  ICR1H = 0x3D;
  ICR1L = 0x09;

  //Compare Register Value
  //Compare = 7812
  //50% duty cycle
  OCR1AH = 0x1E;
  OCR1AL = 0x84;
  
  //PWM Phase Correct Mode 10 With ICR1 as TOP
  TCCR1A |= (1<<WGM11);
  TCCR1B |= (1<<WGM13);

  //Pre-Scaler is 1024
  TCCR1B |= (1<<CS12) | (1<<CS10);

  //Enable Interrupt when Output compare A equals TCNT1 
  TIMSK1 |= (1<<OCIE1A);

  //PWM2

  //Mode 3 with a 255 TOP
  TCCR0A |= (1<<COM0A1) | (1<<WGM01) | (1<<WGM00);
  
  //Pre-Scaler of 64
  TCCR0B |= (1<<CS01) | (1<<CS00);

  //Set the compare value to the set Brightness
  //Default value is 255 maximum value
  OCR0A = bright;
  
  //Set all the Interrupts
  sei();

}

//Interrupt handler
ISR(TIMER1_COMPA_vect)
{ 
  //First LED
  //Showing default frequency and brightness for comparison
  
  //If the Pin PB1 is ON 
  if(PORTB & (1<<PWM))
  {
    //Turn it OFF
    PORTB &= ~(1<<PWM);
  }
  else
  {
    //Turn it ON
    PORTB |= (1<<PWM);
  }

  //Second LED
  //Variable LED with alternating 
  //Brightness (0<b<255)
  //Interval (1>hz)

  //If default is OFF
  if(G_count != -1)
  {
    //Increment counting value
    g_count ++;

    //If incremented value reaches the set inteval term
    if(g_count == G_count)
    {
      //If LED is OFF
      if(OCR0A == 0)
      {
        //Turn LED to brightness
        OCR0A = bright;
      }
      else
      {
        //Turn LED OFF
        OCR0A = 0;
      }
      //Reset the incrementing Count
      g_count = 0; 
    }
  }
  //IF Default ON
  else
  {
    //IF LED OFF
    if(OCR0A == 0)
    {     
      //Turn LED to Brightness
      OCR0A = bright;
    }
    else
    {
      //Turn LED OFF
      OCR0A = 0;
    }
  }
}

//Send Message (array of characters) Function
//accepts char pointer (point to the beginning of the array) 
//accepts int to describe the length of the character array
void seMess(uint8_t* msg, int size)
{
  //loops through each char in the passed array
  for(int i=0; i<size; i++)
  {
    //waits for the data register is empty
    while((UCSR0A & (1<<UDRE0))==0){}
    
    //pass the specified character into the data register
    UDR0 = msg[i];
  }
}

//Receive Character Function
uint8_t reChar()
{
  //Wait until the message has been received (RXC0 will be 0)  
  while((UCSR0A & (1<<RXC0))==0){}
  
  //Return the byte in the data register
  return UDR0;
}

void print_num(int num)
{
  uint8_t tmp[4];

  tmp[0] = num/100 + '0';
  tmp[1] = (num%100)/10 + '0';
  tmp[2] = num%10 + '0';
  tmp[3] = '\n';

  seMess(tmp,4);
  
}

void reset(uint8_t* tmp, int length)
{
  for(int i=0; i<length; i++)
  {
    tmp[i] = '\0';
  }
}

//Interprets the Serial Monitor Input
void Toggle(uint8_t* hold, int length)
{
  //Variable indicating that a number has begun to be read
  int strt = 0;
  
  //Variable controlling the Loop involved in recording numbers
  int on = 1;

  //String to hold the number;
  uint8_t tmp[3];

  //Local Brightness number
  int br = 0;

  //Interval number
  int intv = 0;

  //Index to hold position of the number
  int count = 0;

  //Loops through the String
  for(int i=0; i<length; i++)
  {
    //If br has been recognized
    if(hold[i] == 'b' && hold[i+1] == 'r')
    {
       //Start Reading for Numbers
       while(on)
       {
          //If what is being read is in the numerical range 0-9
          if((int)hold[i] >= 48 && (int)hold[i] <= 57 )
          {
            //Number has started
            strt = 1;
            
            //Fill the Number Container with the current digit
            //At the index position count
            tmp[count] = hold[i];

            //Increment index
            count ++;
          }
          //If something other than a number is read
          else if(strt)
          {
            //Fill the local brightness Variable 
            //With Value from the Serial Monitor
            br = atoi(tmp);

            //Reset Tmp variable
            reset(tmp, 3);
            //Reset Index
            count = 0;
            //Reset number Read 
            strt = 0;
            //Reset number Loop
            on = 0;
          }
          //Continue reading the String after number
          i++;
       }
       //Turn on Looping when a number is read 
       on = 1;
    }

    //If in is recognized 
    //Do the exact same thing as for brightness
    if(hold[i] == 'i' && hold[i+1] == 'n')
    {
       while(on)
       {
          if((int)hold[i] >= 48 && (int)hold[i] <= 57 )
          {
            strt = 1;
            tmp[count] = hold[i];
            count ++;
          }
          else if(strt)
          {
            intv = atoi(tmp);
            reset(tmp, 3);
            count = 0;
            on = 0;
            strt = 0;
          }
          i++;
       }
       on = 1;
    }  
  }

  //If a brightness value was read
  if(br != 0)
  {
    //If the brightness is not in the proper range
    if(br > 255 || br<0)
    {
      //Send message and set brightness to default
      uint8_t msg[30] = "Brightness Value is too high reset to default\n";
      seMess(msg, 30);
      br = 255;
    }

    //Send message indicating new brightness
    uint8_t msg[30] = "Brightness Value is now ";
    seMess(msg, 30);

    //Print the number and a new line
    print_num(br);
    
    
    //Brightness of the toggle LED is changed
    //Set value of Glabal brightness to value of local brightness
    bright = br;

    //Set compare register of timer 0 to chosen brightness
    OCR0A = bright;
  }

  //If Interval value is valid (positiv)
  if(intv > 0)
  {
    //If the Interval is 1hz or greater
    if(intv >= 1)
    {
      //Send Message and number
      uint8_t msg[30] = "Interval Value is now ";
      seMess(msg, 30);
      print_num(intv);

      //Set Global counters to appropriate values
      G_count = intv;
      g_count = 0;
    }
    else
    {
      //Sends a message and leaves the interval
      uint8_t msg[30] = "Sorry Interval is too low\n";
      seMess(msg,30);
    }
  }
  
}

//Function to print User prompt
void Messages()
{
  uint8_t msg[90] = "Please enter br: followed by a value, not exceeding 255, to change Brightness\n";
  uint8_t msg2[90] = "Please enter in: followed by a value, not lower than 1 hz, to change Interrupt Interval\n";
  uint8_t msg3[85] = "Both can be entered simultanously and out of order\n";

  seMess(msg, 90);
  seMess(msg2, 90);
  seMess(msg3, 85);
}

//Main Function
int main()
{
  //Initialize the Pins, Timer and UART
  init();

  //Variable to Store Receive value
  uint8_t rc;

  //Set index for the input string to 0
  int i = 0;

  //Variable to indicate a read message
  int on = 0;

  //Variable to hold Serial Monitor string
  uint8_t hold[50];

  //Printing Messages for user prompt
  Messages();

  //Make sure hold is empty
  reset(hold,50);
 
  while(1)
  {
    //Start reading from Monitor
    rc = reChar();

    //Read until a line break
    while((int)rc != 10)
    {
      //If the value is a number or a letter 
      if((int)rc >= 48 && (int)rc <= 90 || (int)rc)
      {
        //Store the character to the string
        hold[i] = rc;
        
        //Set the Read variable to true to call Toggle function 
        on = 1;

        //increment index of string
        i += 1;
      }
      rc = reChar();
    }

    //If a message has been read
    if(on)
    {
      //Display what has been entered by the user
      uint8_t msg[20] = "You have entered: ";

      seMess(msg, 20);
      seMess(hold,50);
      seMess("\n",1);

      //Call the Toggle Function
      Toggle(hold, i);

      //Reset the input string variable
      reset(hold,50);

      //Reset index and Read boolean
      i = 0;
      on = 0;

      //promt the user
      Messages();
    }
    
  }
  return 0;
}
