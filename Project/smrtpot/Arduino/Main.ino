//Definitions
#define F_CPU 16000000
#define USART_BAUDRATE 9600
#define BAUD_PRESCALE ((F_CPU/(USART_BAUDRATE*16UL))-1)

//Libraries
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>
#include <string.h>

//Global Variables used to set
//pin locations on the board

//master in slave out
int miso = 4;

//Infrared A0 pin
int IR = 0;
int LED = 6;

//Slave address 0x02 and Read bit 0x01
uint8_t SLA_R = 0x03;

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
  //SPCR |= (1 << SPIE);

  //Set Pin A0 PC0 to output
  DDRC |= (1<<0);
  DDRC |= (1<<1);
  DDRC |= (1<<2);

  //setting channel and setting reference voltage to 5v
  ADMUX |= 0x00;
  
  //ADEN enables EDC
  //Others set to one sets the Prescaeler to 128 to achieve ADC in range of 50kHz - 200 kHz
  ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);

  //ADCSRA |= (1<<ADIE);

  DDRD |= (1<<LED);
  
   //TWI Communication enable
  //PreScaler 1 
  TWSR = 0x00;  
  //Set Frequency to 400hz 
  TWBR = 0x0C;  
  //Enable TWI Communication
  TWCR = ( 1 << TWEN );
  
  TCCR2A = (1 << WGM20) | (1 << WGM21) | (1 << COM2B1); //set fast pwm with wgm20 & wgm21, non-inverting mode
  TCCR2B = (1 << CS10);  //using prescale /8

  DDRD |= (1 << 3); //set PD3 to output for OC2B pwm
  DDRB |= (1 << 0) | (1 << 1); //set PB0 & PB1 to output 
  PORTB |= (1 << 0) | (1 << 1); //set PB0 and PB1 High to enable output of H bridge
  //connect motro to pin 2 &|| 6 of the H bridge

  //SET OCR2B to 255 for on, 0 to off
  //EX: OCR2B = 255;

}

/**** START I2C FUNCTIONS ****/
//Receive Status from TWSR Register
uint8_t TWSR_status (void)
{
  //Mask the bottom 3 bits to return a Context Specific Status
  uint8_t status;
  status = (TWSR & 0xF8); 
  return status;
}

// Sends a start Condition over the bus to the slave
void START()
{
  // TWINT Set the Interrupt flag 
  // TWSTA Set the Start condition
  // TWEN set TWI enable
  TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
  
  // Wait for TWI Interrupt Flag to be set
  // indicating the end of a process
  while((TWCR & (1<<TWINT))==0) {}
}

// Used to convey the end of a communication cycle
void STOP()
{
  // Set TWI interrupt flag, TWI enable, TWI Stop
  TWCR |= (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
  
  // Wait for stop Transaction bit
  while((TWCR & (1<<TWSTO)));
}

// Used to deliver the address and read bit to the slave
void load_addr()
{
  //load slave address
  TWDR = SLA_R;
  
  //Enable TWI Interrupt flag and TWI enable
  TWCR = (1<<TWINT) | (1<<TWEN);
  
  //Wait for TWI Interrupt Flag to be set
  //indicating the end of a process
  while((TWCR & (1<<TWINT))==0){}
}

//Writes data to Slave
void load_data(uint8_t data)
{
  //8-bit data is loaded into data register
  TWDR = data;
  
  //Enable TWI Interrupt flag and TWI enable
  TWCR = (1<<TWINT) | (1<<TWEN);
  
  //Wait for TWI Interrupt Flag to be set
  //indicating the end of a process
  while((TWCR & (1<<TWINT))==0){}
  
}

//Receive Data From Slave with ACK Signal
char rec_data_ack()
{
  //Set TWI Flag, Enable TWI Communication, Accept ACK signal
  TWCR = ( 1 << TWINT )|( 1 << TWEN )|(1 << TWEA);
  while (( TWCR & ( 1 << TWINT )) == 0 );
  //return the contents of the data register
  return TWDR;
}

//Receive From slave whit Non ACK condition
char rec_data()
{
  TWCR = ( 1 << TWINT )|( 1 << TWEN );
  while (( TWCR & ( 1 << TWINT )) == 0 );
  //return the contents of the data register
  return TWDR;
}

//Reset Character Array for Temp Sensor
void reset_data(uint8_t* msg)
{
  msg[0] = '\0';
  msg[1] = '\0';
  msg[2] = '\0';
}

// Temp Sensor Function creates communication with the Slave
// Return the value of the Sensor
void Temp_ON(uint8_t *Temp_data)
{
  //used for the while loop to fill the char array for IR data
  int i=0;

  //Receive variable Holds the contents of the Slave Transmission
  uint8_t rc;

  //Variable to hold status from TWSR Register
  uint8_t status;

  //Sends Start Condition
  START();

  //Send the Address to the slave
  load_addr();

  //Receive the Status from the TWSR register
  status = TWSR_status();
        
  if (status == 0x40)
  {
    while(i<3)
    {
      //Receive from the Slave With ACK Signal
      rc = rec_data_ack();

      //If the character is a digit accept
      if((int)rc>47 && (int)rc<58)
      {
        //Store it in the Char Array and increment the index
        Temp_data[i] = rc;
        i++;
      }
    }
    i=0;
    //Receive final NACK Signal and Stop the Transmission
    rec_data();
    STOP();
 }

 //Display the Message IR Data and then clear the array
 seMess(msg, 4);
 seMess(Temp_data,3);
 seChar('\n');
 reset_data(Temp_data);
}
/**** END I2C FUNCTIONS ****/

/**** START UART FUNCTIONS ****/
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

//UART send Message
void seMess(uint8_t* msg, int size)
{
  for(int i=0; i<size; i++)
  {
    while((UCSR0A & (1<<UDRE0))==0){}
    
    UDR0 = msg[i];
  }
}
/**** END UART FUNCTIONS ****/

/**** START SPI FUNCTIONS ****/
//Receiving function
uint8_t receiver ()
{
  // Wait until interrupt flag is asserted 
  //Is asserted when transmission is complete
  while ((SPSR & (1 << SPIF))==0){}
 
  //returns the contents of the data register  
  return (SPDR);
}

//transmiting function
void transmit (uint8_t data)
{
  //Load byte of data into data transfer register 
  SPDR = data; 
  
  // Wait until interrupt flag is asserted. 
  //Is asserted when transmission is complete
  while ((SPSR & (1 << SPIF))==0){}
}

//transmiting function
void transmiting (uint8_t* data,int n)
{
  //Load byte of data into data transfer register 
  int i = 0;

  if(i == 0)
    transmit('0');
    
  while(i<n)
  {
    SPDR = toHex(data[i]); 
  
  // Wait until interrupt flag is asserted. 
  //Is asserted when transmission is complete
    while ((SPSR & (1 << SPIF))==0){}
    i++;
  }
}
/**** END SPI FUNCTIONS ****/

/**** START ADC FUNCTIONS ****/
void reset(uint8_t* msg)
{
  msg[0] = '0';
  msg[1] = '0';
  msg[2] = '0';
}

uint8_t toHex(uint8_t num)
{
  if(num == '0')
    return 0x0;
  else if(num == '1')
    return 0x1;
  else if(num == '2')
    return 0x2;
  else if(num == '3')
    return 0x3;
  else if(num == '4')
    return 0x4;
  else if(num == '5')
    return 0x5;
  else if(num == '6')
    return 0x6;
  else if(num == '7')
    return 0x7;
  else if(num == '8')
    return 0x8;
  else if(num == '9')
    return 0x9;

  return num;
    
}

// setting specific ANALOG channel to read from
void setADMUX(int num)
{
  if(num == 0)
  {
    ADMUX = 0x40;
  }
  else if(num == 1)
  {
    ADMUX = 0x41; 
  }
  else if(num == 2)
  {
    ADMUX = 0x42;
  }
  else if(num == 3)
  {
    ADMUX = 0x43;
  }
  else if(num == 4)
  {
    ADMUX = 0x44;
  }
  else if(num == 5)
  {
    ADMUX = 0x45;
  }
  
}


void Sensor(uint8_t* msg, int num)
{
  // select ANALOG channel to read from
  setADMUX(num);
  
  ADCSRA|=(1<<ADSC);

  // waiting for conversion to complete
  
  while(!(ADCSRA & (1<<ADIF))){};

  // conversion is complete
  ADCSRA|=(1<<ADIF);

  // Separates the 100, 10 and 1
  //into three categories 
  int hun = (int)ADC/100;
  int ten = (int)(ADC/10)%10;
  int one = (int)ADC%10;
  
  // store the int as an 8-bit char into 
  // the passed array
  msg[0] = hun + '0';
  msg[1] = ten + '0';
  msg[2] = one + '0';
}
/**** END ADC FUNCTIONS ****/

/**** MAIN ****/
int main(void) 
{
  //Enables UART, IR and Slave
  init();

  // these arrays hold 3 digit values for each sensor
  uint8_t mois[3];
  uint8_t mois1[3];
  uint8_t mois2[3];
  uint8_t ultraS[3];
  uint8_t temp[3];
  uint8_t lght[3];
  
  // received command from BBB
  uint8_t rc;
  
  while(1)
  { 
    // non blocking awaiting command   
    rc = SPDR;
  
    // set to [0,0,0]
    reset(mois);
    reset(mois1);
    reset(mois2);
    reset(ultraS);
    reset(temp);
    reset(lght);
    
    
    Sensor(mois,0);
    Sensor(mois1,1);
    Sensor(mois2,2);
    //TEMP_ON(temp); // not ready yet
    // Sensor(ultraS,4); // not ready yet
    Sensor(lght,5);
    
    // if received certain command from BBB
    if (rc == '0') {
       seChar(rc);
      transmiting(mois,3);
    }
    else if(rc == '1') {
       seChar(rc);
      transmiting(mois1,3);
    }
    else if(rc == '2') {
      seChar(rc);
      transmiting(mois2,3);
    }
    else if(rc == '3') {
      seChar(rc);
      transmiting(temp,3);
    }
    else if(rc == '4') {
       seChar(rc);
      transmiting(ultraS,3);
    }
    else if(rc == '5') {
      seChar(rc);
      transmiting(lght,3);
    }
    else if(rc == '6')
    {
	  0CR2B = 255;
      transmiting(OCR2B, 1);
    }
    else if(rc == '7')
    {
	OCR2B = 0;
      transmiting(OCR2B, 1);
    }

    if((int)lght[0] < 53) {
      PORTD |= (1<<LED);
    }
    else {
      PORTD &= ~(1<<LED);
    }
    
  }
  
  return 0;
}
