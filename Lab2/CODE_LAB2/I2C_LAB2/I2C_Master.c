//definitions
#define F_CPU 16000000
#define USART_BAUDRATE 9600
#define BAUD_PRESCALE ((F_CPU/(USART_BAUDRATE*16UL))-1)

//libraries
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <string.h>

//Slave address 0x02 and Read bit 0x01
uint8_t SLA_R = 0x03;

//Initiates UART and TWI Communication
void init(){
  //Sets UART BAUD RATE to 9600
  //Makes sure to shift the BAUD rate to upper part 
  UBRR0H = (BAUD_PRESCALE >> 8);
  UBRR0L = BAUD_PRESCALE;
  
  //enable Send and Recieve
  UCSR0B |= (1<<RXEN0) | (1<< TXEN0);

  //TWI Communication enable
  //PreScaler 1 
  TWSR = 0x00;  
  //Set Frequency to 400hz 
  TWBR = 0x0C;  
  //Enable TWI Communication
  TWCR = ( 1 << TWEN );
}

//Read Character function
unsigned char reChar()
{
  while ( !(UCSR0A & (1<<RXC0)) ) ;
  return UDR0;
}

//Send Character Function
void seChar(char c)
{
  while( !(UCSR0A & ( 1 << UDRE0)) );
  UDR0 = c;
}

//Send Message
void seMess(uint8_t* msg, int size)
{
  for(int i=0; i<size; i++)
  {
    while( !(UCSR0A & ( 1 << UDRE0)) );
    UDR0 = msg[i];
  }
}

//Receive Status from TWSR Register
uint8_t TWSR_status (void)
{
  //Mask the bottom 3 bits to return a Context Specific Status
  uint8_t status;
  status = (TWSR & 0xF8); 
  return status;
}

//Sends a start Condition over the bus to the slave
void START()
{
  //TWINT Set the Interrupt flag 
  //TWSTA Set the Start condition
  //TWEN set TWI enable
  TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
  
  //Wait for TWI Interrupt Flag to be set
  //indicating the end of a process
  while((TWCR & (1<<TWINT))==0) {}
}

//Used to convey the end of a communication cycle
void STOP()
{
  //Set TWI interrupt flag, TWI enable, TWI Stop
  TWCR |= (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
  
  //Wait for stop Transaction bit
  while((TWCR & (1<<TWSTO)));
}

//Used to deliver the address and read bit to the slave
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

//Reset Character Array for IR Sensor
void reset_data(uint8_t* msg)
{
  msg[0] = '\0';
  msg[1] = '\0';
  msg[2] = '\0';
}

//IR Sensor Function creates communication with the Slave
//Return the value of the Sensor
void IR_ON()
{
  //used for the while loop to fill the char array for IR data
  int i=0;

  //Receive variable Holds the contents of the Slave Transmission
  uint8_t rc;

  //Variable to hold status from TWSR Register
  uint8_t status;

  //Data Array to hold the IR Sensor Data
  uint8_t IR_data[3];

  //Message to display to Serial Monitor
  uint8_t msg[4] = "IR: ";

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
        IR_data[i] = rc;
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
 seMess(IR_data,3);
 seChar('\n');
 reset_data(IR_data);
}

//Main function to Get IR Data On Demand
int main(void)
{
  //Initialize UART and TWI
  init();

  //Receive Character Hold Character From Serial Monitor
  uint8_t rc;

  //Boolean to See if the IR data should be displayed
  bool on = false;

  //Holds Message to prompt user
  uint8_t msg[28] = "Enter 1 to Toggle IR Sensor";

  //Writes the message to the Serial Monitor
  seMess(msg, 28);
  seChar('\n');
    
  while(1)
  {
    //Recieves the Character From the Serial Monitor
    rc = reChar();

    if(rc == '1')
    {
      //Sets the IR Bool variable to true
      on = true;
    }

    //Returns IR data on Command
    while(on)
    {
      //Creates the communication to Slave and returns 
      IR_ON();
      //Let the IR Sensor do Work
      _delay_ms(100);
      //Sets bool to false to display only one IR value
      on = false;
      //Asks user for prompt again
      seMess(msg, 28);
      seChar('\n');
      
    } 
  }
}

 
