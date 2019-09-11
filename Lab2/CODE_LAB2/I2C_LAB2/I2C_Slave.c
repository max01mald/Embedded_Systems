//definitions
#define F_CPU 16000000
#define USART_BAUDRATE 9600
#define BAUD_PRESCALE ((F_CPU/(USART_BAUDRATE*16UL))-1)

//libraries
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <string.h>

//Slave Address with a leftward shift to fill 7 most significant bits
uint8_t SLA_A = 0x02;


void init ( void ) 
{
  //Slave Initialization with Prescaler 1
  TWSR = 0x00;
  //Frequency of 400 hz  
  TWBR = 0x0C;  
  //Load the address into the address register
  TWAR = SLA_A ; 
  //Enable ACK and TWI communication
  TWCR = ( 1 << TWEA ) | ( 1 << TWEN );

  //IR Sensor set up
  //Set data direction to input
  DDRC &= ~(1<<0);

  //choosing channel one
  uint8_t ch=ch&0b00000001;

  //setting channel and setting reference voltage to 5v
  ADMUX |= ch | (1<<REFS0);

  //ADEN enables EDC
  //Others set to one sets the Prescaeler to 128 to achieve ADC in range of 50kHz - 200 kHz
  ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
  
}

//Same as in SPI or Lab 1b
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

//Function to return status from TWSR
uint8_t TWSR_status ( void)
{
  uint8_t status;
  //Mask the least significant bit
  status = (TWSR & 0xF8);
  return status;
}

//Function used to transmit data to master
void load_data ( uint8_t data)
{
  //load data into Data Register
  TWDR = data;
  //Enable Interrupt flag, TWI communication and ACK bit
  TWCR = ( 1 << TWINT ) | ( 1 << TWEN ) | ( 1 << TWEA );
  //Wait for the Interrupt flag to set
  while (( TWCR & ( 1 << TWINT )) == 0 );
}

//Slave Transmit mode constantly in the Main loop
int main(void)
{
  //Initialize the IR Sensor and TWI communication
  init();
  //Index used to cycle through IR_data array
  int i=0;
  //Variable used to hold status from TWSR 
  uint8_t status;
  //Char array to hold 3 digits of the IR Sensor
  uint8_t IR_data[3];

  while(1)
  {
      //Enable ACK Transmission to trigger 0xA8 condition when the 
      //address is received from master
      TWCR |= ( 1 << TWEA ); 

      //Fill the IR_data array with Sensor data
      IR_Sensor(IR_data);
      //Wait to let Sensor work
      _delay_ms(100);

      //Receive status form TWSR
      //Address has been received transmission is enabled
      status = TWSR_status();
      
      if (status == 0xA8)
      {
        //cycle through the IR data array
        while(i<3)
        {
          //Transmit one of the bytes at a time
          load_data(IR_data[i]);
          _delay_ms(100);
          //increment index
          i+=1;
        }
        //reset index
        i=0;

        //Send NACK signal to master signifying end of transmission
        TWCR &= ~( 1 << TWEA );
      }
      else
      {
        //NO IDEA DOES NOT WORK WITHOUT ???
        continue;
      }
      //Delay to let the IR Sensor work
      _delay_ms(100);
   }
}

 
