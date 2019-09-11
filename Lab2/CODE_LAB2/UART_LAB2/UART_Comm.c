//definition of constants 
//CPU clock frequency 
#define F_CPU 16000000
//Defined BAUD rate
#define USART_BAUDRATE 9600
//BAUD pre-scaler 
#define BAUD_PRESCALE ((F_CPU/(USART_BAUDRATE*16UL))-1)

//Libraries
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <string.h>


void init()
{
  //Set up BAUD rate in the 16 bit UBRR0 register
  //separated into two 8-bit portions UBRR0H and UBRR0L
  UBRR0H = (BAUD_PRESCALE >> 8);
  UBRR0L = BAUD_PRESCALE;
  
  //enable Send (RXEN0 set to 1) and Receive (TXEN0 set to 1)
  UCSR0B |= (1<<RXEN0) | (1<< TXEN0);
  
}

//Receive Character Function
char reChar()
{
  //Wait until the message has been received (RXC0 will be 0)  
  while((UCSR0A & (1<<RXC0))==0){}
  
  //Return the byte in the data register
  return UDR0;
}

//Send Character Function
void seChar(char c)
{
  //Wait until data register is empty (UDRE0 will be 0)
  while((UCSR0A & (1<<UDRE0))==0){}
  
  //fill the data register with the character fed into the function
  UDR0 = c;
  
}

//Send Message (array of characters) Function
//accepts char pointer (point to the beginning of the array) 
//accepts int to describe the length of the character array
void seMess(char* msg, int size)
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


int main(void) {

  //char set to hold received character
  char rc;
  
  //char array with capacity of 255 characters
  //set to hold serial monitor inputs
  char msg [255];
  
  //set to hold greetings string
  // to manipulate input string
  char greet[7] = "Hello ";
  
  //integer used to hold position
  //of the current character in the input string
  int i = 0;
  
  //calls initiation function to activate Send/Receiving
  //BAUD rate
  init();

  while(true)
  {
    //Reads char from input string in serial monitor
    rc = reChar();
    
    //saves character in message array
    msg[i] = rc;
    
    //increment position in the array
    i++;
    
    //if a line break is read in the serial monitor
    if((int)rc==10)
    {
      //Sends the greetings message followed by 
      //the input message to the serial monitor
      seMess(greet, 7);
      seMess(msg, i);
      
      //reduces the i counter back to 0
      //while setting all the used array positions to NULL
      while(i != 0)
      {
        msg[i] = '\0';
        i--;
      }
    }
  }
  return 0;
}

  
