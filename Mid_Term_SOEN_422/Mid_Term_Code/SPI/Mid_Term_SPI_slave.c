//Definitions
#define F_CPU 16000000
#define USART_BAUDRATE 9600
#define BAUD_PRESCALE ((F_CPU/(USART_BAUDRATE*16UL))-1)

//Libraries
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <string.h>

//Global Variables used to set
//pin locations on the board

//master in slave out
int miso = 4;

//Infrared A0 pin
int IR = 0;

//Initialize UART and IR Sensor
void init()
{
  //Set up BAUD rate
  UBRR0H = (BAUD_PRESCALE >> 8);
  UBRR0L = BAUD_PRESCALE;
  
  //enable Send and Recieve
  UCSR0B |= (1<<RXEN0) | (1<< TXEN0);
  
  //Initialization of a Slave nano
  // Set MISO as output. MOSI, SCK and SS are set to inputs 
  DDRB |= (1 << miso); 

  // Set device as SPI slave just by enabling SPI
  SPCR |= (1 << SPE); 

  //Set Pin A0 PC0 to output
  DDRC |= (1<<0);

  cli();
  sei();
}

//Receiver funtion UART
char reChar()
{
  //Waits for transmission to be over
  while((UCSR0A & (1<<RXC0))==0){}
  
  //returns byte to Main
  return UDR0;
}

//Transmit function UART
void seChar(char c)
{
  //Wait for Tranmission to be done
  while((UCSR0A & (1<<UDRE0))==0){}
  
  //stores the data byte in transmission register
  UDR0 = c;
  
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

//Transmitting function
void transmit (uint8_t data)
{
  //Load byte of data into data transfer register 
  SPDR = data; 
  
  // Wait until interrupt flag is asserted. 
  //Is asserted when transmission is complete
  while ((SPSR & (1 << SPIF))==0){}
}

ISR(SPI_STC_vect)
{
 //Receive variable to hold received value   
 uint8_t rc;

 //Reception of char from the master
 rc = receiver();

 //If what is received is a 1
 if((int)rc == 49)
  {
    //if LED is ON
    if(PORTC & (1<<0))
    {
      //Transmit byte representing Off
      //Turn the light Off
      transmit('0');
      PORTC &= ~(1<<0);
    }
    else
    {
      //Transmit byte representing On
      //Turn the light On
      transmit('1');
      PORTC |= (1<<0);
    }
  }
}

int main(void) 
{
  //Enables UART, IR and Slave
  init();

  while(1)
  {
    
  }
  
  

  return 0;
}
