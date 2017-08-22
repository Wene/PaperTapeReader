/* This software is written for Arduino boards and turn it into a
   Control unit replacement for FACIT Paper tape reader. In the current
   state four read modes are available: debug, binary, ASCII and Baudot. */

enum readMode {error, debug, binary, ascii, baudot};
enum baudotMode {letters, figures};

readMode mode = error;
unsigned int errorCount = 0;
baudotMode bMode = letters;

unsigned char baudot2ascii(unsigned char inchar)
{
  //remove the last 3 bits, which are allways 1 by reading 5 bit code
  inchar -= 224;
  
  unsigned char outchar = 0;
  
  if(bMode == letters)
  {
    switch(inchar)
    {
      case 1:   outchar = 't';   break;
      case 2:   outchar = 13;    break;  // CR
      case 3:   outchar = 'o';   break;
      case 4:   outchar = ' ';   break;  // space
      case 5:   outchar = 'h';   break;
      case 6:   outchar = 'n';   break;
      case 7:   outchar = 'm';   break;
      case 8:   outchar = 10;    break;  // LF
      case 9:   outchar = 'l';   break;
      case 10:  outchar = 'r';   break;
      case 11:  outchar = 'g';   break;
      case 12:  outchar = 'i';   break;
      case 13:  outchar = 'p';   break;
      case 14:  outchar = 'c';   break;
      case 15:  outchar = 'v';   break;
      case 16:  outchar = 'e';   break;
      case 17:  outchar = 'z';   break;
      case 18:  outchar = 'd';   break;
      case 19:  outchar = 'b';   break;
      case 20:  outchar = 's';   break;
      case 21:  outchar = 'y';   break;
      case 22:  outchar = 'f';   break;
      case 23:  outchar = 'x';   break;
      case 24:  outchar = 'a';   break;
      case 25:  outchar = 'w';   break;
      case 26:  outchar = 'j';   break;
      case 27:  bMode = figures; break;
      case 28:  outchar = 'u';   break;
      case 29:  outchar = 'q';   break;
      case 30:  outchar = 'k';   break;
      case 31:  outchar = 127;   break;  // 31 is shift to letters. Interpret multiple shifts in a row as DEL.
      default:  outchar = 0;     break;
    }
  }
  else if(bMode == figures)
  {
    switch(inchar)
    {
      case 1:   outchar = '5';   break;
      case 2:   outchar = 13;    break;  // CR
      case 3:   outchar = '9';   break;
      case 4:   outchar = ' ';   break;
      case 5:   outchar = ' ';   break;  // unused or #
      case 6:   outchar = ',';   break;
      case 7:   outchar = '.';   break;
      case 8:   outchar = 10;    break;  // LF
      case 9:   outchar = ')';   break;
      case 10:  outchar = '4';   break;
      case 11:  outchar = ' ';   break;  // unused or &
      case 12:  outchar = '8';   break;
      case 13:  outchar = '0';   break;
      case 14:  outchar = ':';   break;
      case 15:  outchar = '=';   break;
      case 16:  outchar = '3';   break;
      case 17:  outchar = '+';   break;
      case 18:  outchar = 5;     break;  // Who are You? -> ASCII representation ENQuiry
      case 19:  outchar = '?';   break;
      case 20:  outchar = '\'';  break;
      case 21:  outchar = '6';   break;
      case 22:  outchar = ' ';   break;  // unused or !
      case 23:  outchar = '/';   break;
      case 24:  outchar = '-';   break;
      case 25:  outchar = '2';   break;
      case 26:  outchar = '\a';  break;  // Bell
      case 27:  outchar = 127;   break;  // 27 is shift to figures. Interpret multiple shifts in a row as DEL.
      case 28:  outchar = '7';   break;
      case 29:  outchar = '1';   break;
      case 30:  outchar = '(';   break;
      case 31:  bMode = letters; break;
      default:  outchar = 0;     break;
    }
  }  
  return outchar;
}

void setup()
{
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  
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
  Serial.print("Select mode: (D)ebug, (B)inary, (A)SCII, (5)-Bit Baudot");
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
        Serial.println();
        Serial.println("binary  \thex\tdecimal\tASCII");
        Serial.println("------  \t---\t-------\t-----");
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
      case '5':
        mode = baudot;
        Serial.println("Enter 5-Bit Baudot mode. Press a key to exit.");
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
            if(bit[i])
            {
              Serial.print("o");
            }
            else
            {
              Serial.print(".");
            }
            if(i == 2)
            {
              Serial.print(":");
            }
          }
          
          Serial.print("\t");
        
          //print in Hex and Dec
          //fill with 0 if the value is below 16
          if(byte8 < 16)
          {
            Serial.print(0);
          }
          Serial.print(byte8, HEX);
          Serial.print("\t");
          //fill with at least one 0 if the value is below 100
          if(byte8 < 100)
          {
            Serial.print(0);
          }
          //add another 0 if the value is below 10
          if(byte8 < 10)
          {
            Serial.print(0);
          }
          Serial.print(byte8, DEC);
          Serial.print("\t");
          
          //print the ASCII character only if it is a human readable character
          if(byte8 > 31 && byte8 < 127)
          {
            Serial.write(byte8);
          }
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
          
        case baudot:
          //substitute baudot code by ASCII character and print it
          unsigned char b2ascii = baudot2ascii(byte8);
          if(b2ascii) //only write characters != 0
          {
            Serial.write(b2ascii);
          }
          break;
      }
      
      //avoid double read
      delay(3);
    }
  }
}
