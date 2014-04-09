/* This software is written for Arduino boards and turn it into a
   Control unit replacement for FACIT Paper tape reader. In the current
   state only reading is supported. */

void setup()
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
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
  bool wait = true;
  while(wait)
  {
    if(digitalRead(10) == LOW)
    {
      wait = false;
    }
  }
  
  int bit[8];
  for(int i = 0; i < 8; i++)
  {
    bool state = digitalRead(i+2); 
    bit[i] = state;
  }
  
  for(int i = 0; i < 8; i++)
  {
    Serial.print(bit[i]);
  }
  
  Serial.println();
  delay(3);
  while(!wait)
  {
    if(digitalRead(10) == HIGH)
    {
      wait = true;
    }
  }
}
