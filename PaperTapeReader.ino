/* This software is written for Arduino boards and turn it into a
   Control unit replacement for FACIT Paper tape reader. In the current
   state three read modes are available: debug, binary and ASCII. */

enum readMode {error, debug, binary, ascii};

readMode mode = error;
unsigned int errorCount = 0;

void setup()
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(19200);
  
  // initialize the input pins
  pinMode(2, INPUT); // bit 0
  pinMode(3, INPUT); // bit 1
  pinMode(4, INPUT); // bit 2
  pinMode(5, INPUT); // bit 3
  pinMode(6, INPUT); // bit 4
  pinMode(7, INPUT); // bit 5
  pinMode(8, INPUT); // bit 6
  pinMode(9, INPUT); // bit 7 or parity
  pinMode(10, INPUT); // clock
}

void loop()
{
  //clear input buffer
  while(Serial.available())
  {
    Serial.read();
    Serial.println();
    Serial.println();
    Serial.println();
  }
  
  //print and reset error count
  if(errorCount)
  {
    Serial.print("Last error count: ");
    Serial.print(errorCount);
    errorCount = 0;
    Serial.println();
  }
  
  //print prompt
  Serial.print("Select mode: (D)ebug, (B)inary, (A)SCII");
  while(!Serial.available())
  {
    delay(10);
  }
  while(Serial.available())
  {
    int command;
    unsigned char letter;
    
    command = Serial.read();
    letter = command;
    
    Serial.println();
    
    switch(letter)
    {
      case 'd':
      case 'D':
        mode = debug;
        Serial.println("Enter debug mode. Press a key to exit.");
        break;
      case 'b':
      case 'B':    
        mode = binary;
        Serial.println("Enter binary mode. Press a key to exit.");
        break;
      case 'a':
      case 'A':
        mode = ascii;
        Serial.println("Enter ASCII mode. Press a key to exit.");
        break;
      default:
        mode = error;
        Serial.println("Error - could not interpret your selection. Try again.");
        break;
    }
    
    Serial.println();   
  }
  
  //read loop - read until a key is pressed
  while(!Serial.available() && mode != error)
  {
    bool clockSignal = false;
    if(digitalRead(10) == HIGH)
    {
      clockSignal = true;
    }
    
    if(clockSignal)
    {
      int bit[8];
      for(int i = 0; i < 8; i++)
      {
        bool state = digitalRead(i+2); 
        bit[i] = state;
      }
      
      //combine bits to a bytes
      unsigned char byte8;
      for(int i = 0; i < 8; i++)
      {
        bitWrite(byte8, i, bit[i]);
      }
      unsigned char byte7;
      for(int i = 0; i < 7; i++)
      {
        bitWrite(byte7, i, bit[i]);
      }
      
      switch(mode)
      {
        case debug:
          //print binary code
          for(int i = 0; i < 8; i++)
          {
            Serial.print(bit[i]);
          }
          
          Serial.print("\t");
        
          //print in Hex and Dec
          Serial.print(byte8, HEX);
          Serial.print("\t");
          Serial.print(byte8, DEC);
          Serial.println();
          break;
        case ascii:
          //ignore zeroes
          if(byte7)
          {

            //check parity and count errors
            int bitCount = 0;
            for(int i = 0; i < 8; i++)
            {
              bitCount += bit[i];
            }
            errorCount += bitCount % 2;

            //print the character
            Serial.write(byte7);
          }
          break;
        case binary:
          Serial.write(byte8);
          break;
      }
      
      delay(3);
    }
  }
}
