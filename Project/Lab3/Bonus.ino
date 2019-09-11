//Definitions
#define F_CPU 16000000
#define USART_BAUDRATE 9600
#define BAUD_PRESCALE ((F_CPU/(USART_BAUDRATE*16UL))-1)

//Libraries
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <string.h>

//initiation function for UART communication
void init()
{
  //Set up BAUD rate
  UBRR0H = (BAUD_PRESCALE >> 8);
  UBRR0L = BAUD_PRESCALE;
  
  //enable Send and Recieve
  UCSR0B |= (1<<RXEN0) | (1<< TXEN0);
  
}

//initiation of the master nano
void mstr_en (void)
{
  //Set MOSI, SCK and SS as outputs, MISO as input
  DDRB |= ((1 << MOSI) | (1 << SCK) | (1 << SS)); 

  //Set device as SPI master, enable SPI.
  SPCR |= ( 1 << MSTR) | (1 << SPE);
  
}

//UART receive character
char reChar()
{
  while((UCSR0A & (1<<RXC0))==0){}
  return UDR0;
}

//UART send character
void seChar(uint8_t c)
{
  while((UCSR0A & (1<<UDRE0))==0){}
  UDR0 = c;
  
}

//UART send Message
void seMess(uint8_t* msg, int size)
{
  for(int i=0; i<size; i++)
  {
    while((UCSR0A & (1<<UDRE0))==0){}
    
    UDR0 = msg[i];
  }
}

//Transmitting function
void transmit (uint8_t data)
{
  //Load byte of data into data transfer register 
  SPDR = data; 
  
  // Wait until interrupt flag is asserted. 
  //Is asserted when transmission is complete
  while ((SPSR & (1 << SPIF))==0){}
}

//Receiving function
uint8_t receiver ()
{
  // Wait until interrupt flag is asserted 
  //Is asserted when transmission is complete
  while ((SPSR & (1 << SPIF))==0){}
 
  //returns the contents of the data register  
  return (SPDR);
}


//Main program
int main(void) 
{
  //Initiate the UART and SPI registers
  init();
  mstr_en();

  //Information Variable
  //Used to store the input from the Serial Monitor
  uint8_t info;

  //Message designed to ask the user for Input
  uint8_t ask[30] = "Please enter 1 to toggle LED: ";

  //Message designed to let the user know that the transmission has executed
  uint8_t confirm[25] = "Toggle bit has been sent!";
  
  //Reception Varaible
  //Designed to hold transmission from Slave
  uint8_t rec;

  //New Line stores the new line character into a variable
  uint8_t nl = '\n';

  //Displays the Message to the Serial Monitor
  seMess(ask, 30);

  //Receives the Information byte from the Serial Monitor
  info = reChar();
  
  while(1)
  {
    
    if((int)info == 49)
    {
      PORTB |= (1<<ss);
      transmit(info);
      seChar(info);
      seChar('\n'); 
      PORTB &= ~(1<<ss);
    }

    //Receive byte from Serial Monitor
    info = reChar();
  }
  
  return 0;
}
