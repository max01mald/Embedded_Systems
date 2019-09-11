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

//slave select
int ss = 2;

//master out slave in
int mosi = 3;

//master in slave out
int miso = 4;

//serial clock
int sck = 5;

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
  DDRB |= ((1 << mosi) | (1 << sck) | (1 << ss)); 

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
void seChar(char c)
{
  while((UCSR0A & (1<<UDRE0))==0){}
  UDR0 = c;
  
}

//UART send Message
void seMess(char* msg, int size)
{
  for(int i=0; i<size; i++)
  {
    while((UCSR0A & (1<<UDRE0))==0){}
    
    UDR0 = msg[i];
  }
}

//Transmitting and receiving function
uint8_t transceiver (uint8_t data)
{
  //Load byte of data into data transfer register 
  SPDR = data; 
  
  // Wait until interrupt flag is asserted. 
  //Is asserted when transmission is complete
  while ((SPSR & (1 << SPIF))==0){}
  
  //returns the contents of the data register
  return (SPDR);
}

//Reset the array segments of the IR char array to NULL
void reset(uint8_t* msg)
{
  msg[0] = '\0';
  msg[1] = '\0';
  msg[2] = '\0';
  
}

//Used to communicate to slave to obtain IR Data
void IR_ON()
{
  //Send variable
  //To hold Send 8 bit char
  uint8_t sd;
  
  //Receive variable
  //To hold Receive 8 bit char
  uint8_t rc;
  
  //3 8 bit char array to hold IR sensor data
  uint8_t IR[3];
  
  //4 8 bit char array to hold message intended for Serial Monitor output
  uint8_t msg[4] = "IR: ";
  
  //int to hold count position for IR array
  int i = 0;
  
  while(i<3)
  {
    //Master receiving IR data from Slave
    //Master sending empty 8 bit message
    rc = transceiver(sd);

    //if the received data is a digit in the range of 0-9
    if((int)rc>47 && (int)rc<58)
    {
      //Place received data into the appropriate array segment
      IR[i] = rc;
      //increment the index
      i++;
    }
    
  }

  //Send a Informative message to the Serial Monitor
  seMess(msg, 4);
  seMess(IR, 3);
  seChar('\n');
  //Reset the IR array positions to NULL
  reset(IR);

  
}

//Main program
int main(void) 
{
  //Initiate the UART and SPI registers
  init();
  mstr_en();

  //Receive Variable
  //Used to store the incoming byte information from slave
  uint8_t rc;

  //Index
  //Used to cycle through char Array
  int i=0;

  //Turn On IR Function Variable
  //When True access IR Function portion of the program
  int on = 0;

  //Message Variable
  //Used to Store Message for User Action through Serial Monitor
  uint8_t msg[28] = "Enter 1 to Toggle IR Sensor";

  //Print Message
  seMess(msg, 28);
  seChar('\n');
  
  while(1)
  {
    //Receive Char from Serial Monitor
    rc = reChar();

    //If one is entered toggle boolean to true
    if(rc == '1')
    {
      on = 1;
    }

    while(on)
    {
      //Run Function
      IR_ON();
      //Delay to let IR_Sensor Work
      _delay_ms(100);
      //Display only one reading then exit
      on = 0;
      //Display Message demanding user interaction
      seMess(msg, 28);
      seChar('\n');
    } 
  }
  
  return 0;
}
