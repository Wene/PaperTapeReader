/* This software is written for Arduino boards and turn it into a
   Control unit replacement for FACIT Paper tape reader. In the current
   state only reading is supported. */

char mode = 'e';
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
    
    command = Serial.read();
    mode = command;
    
    Serial.println();
    
    if(mode == 'd' || mode == 'D')
    {
      //debug mode selected
      mode = 'd';
      Serial.println("Enter debug mode. Press a key to exit.");
    }
    else if(mode == 'b' || mode == 'B')
    {
      //binary mode selscted
      mode = 'b';
      Serial.println("Enter binary mode. Press a key to exit.");
    }
    else if(mode == 'a' || mode == 'A')
    {
      //ASCII mode selected
      mode = 'a';
      Serial.println("Enter ASCII mode. Press a key to exit.");
    }
    else
    {
      //selection error
      mode = 'e';
      Serial.println("Error - could not interpret your selection. Try again.");
    }
    
    Serial.println();
  }
  
  //read loop - read until a key is pressed
  while(!Serial.available() && mode != 'e')
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
      
      //debug mode
      if(mode == 'd')
      {
	//print binary code
        for(int i = 0; i < 8; i++)
        {
          Serial.print(bit[i]);
        }
        Serial.print("\t");
        
	//combine byte and print it in Hex and Dec
	unsigned char byte;
	for(int i = 0; i < 8; i++)
	{
	  bitWrite(byte, i, bit[i]);
	}
	Serial.print(byte, HEX);
	Serial.print("\t");
	Serial.print(byte, DEC);
        Serial.println();
      }
      
      //ASCII mode
      else if(mode == 'a')
      {
	//combine ASCII character
	char byte;
	for(int i = 0; i < 7; i++)
	{
	  bitWrite(byte, i, bit[i]);
	}
	
	//ignore zeroes
	if(byte)
	{
	
	  //check parity and count errors
	  int bitCount = 0;
	  for(int i = 0; i < 8; i++)
	  {
	    bitCount += bit[i];
	  }
	  errorCount += bitCount % 2;
	
	  //print the character
	  Serial.write(byte);
	}
      }
      
      //binary mode
      else if(mode == 'b')
      {
	unsigned char byte;
	for(int i = 0; i < 8; i++)
	{
	  bitWrite(byte, i, bit[i]);
	}
	Serial.write(byte);
      }
  
      delay(3);
    }
  }
}
