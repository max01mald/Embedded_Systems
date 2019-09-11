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
  
  //IR Sensor set up
  //Set data direction to input
  DDRC &= ~(1<<IR);

  //choosing channel one
  uint8_t ch=ch&0b00000001;

  //setting channel and setting reference voltage to 5v
  ADMUX |= ch | (1<<REFS0);

  //ADEN enables EDC
  //Others set to one sets the Prescaeler to 128 to achieve ADC in range of 50kHz - 200 kHz
  ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);

}

//Initialization of a Slave nano
void slv_en (void)
{
  // Set MISO as output. MOSI, SCK and SS are set to inputs 
  DDRB |= (1 << miso); 

  // Set device as SPI slave just by enabling SPI
  SPCR |= (1 << SPE); 
}

//Transmitting and receiving function
uint8_t transceiver (uint8_t data)
{
  //Load byte of data into data transfer register 
  SPDR = data; 
  
  // Wait until interrupt flag is asserted 
  //Is asserted when transmission is complete
  while ((SPSR & (1 << SPIF))==0){}
 
  //returns the contents of the data register  
  return (SPDR);
}

//IR_Sensor function accepts a pointer to a 8-bit char
void IR_Sensor(uint8_t* msg)
{
  //ADC setting up conversion
  ADCSRA|=(1<<ADSC);

  //waiting for conversion to complete
  while(!(ADCSRA & (1<<ADIF))){};

  //conversion is complete
  ADCSRA|=(1<<ADIF);

  //Separates the 100, 10 and 1
  //into three categories 
  int hun = (int)ADC/100;
  int ten = (int)(ADC/10)%10;
  int one = (int)ADC%10;
  
  //store the int as an 8-bit char into 
  //the passed array
  msg[0] = hun + '0';
  msg[1] = ten + '0';
  msg[2] = one + '0';
  
}

int main(void) 
{
  //Enables UART, IR and Slave
  init();
  slv_en();

  //8-bit char to Store Send infromation
  uint8_t sd;
  
  //8-bit char to store Receive information
  uint8_t rc;
  
  //8-bit char array to store IR_Sensor data
  uint8_t IR_data[3];
   
  while(true)
  {
    //Receives Data from IR_Sensor and breaks it down
    //and stores it in the 3-char array
    IR_Sensor(IR_data);
    
    //Delay to let IR Sensor work
    _delay_ms(100);
  
  //Cycles through the three positions
  //of the array to send to Master
    for(int i=0; i<3;i++)
    {
      rc = transceiver(IR_data[i]);
      _delay_ms(100);
    }

    //Let the IR Sensor work
    _delay_ms(100);
  }

  return 0;
}
