void setup()
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // initialize the input pins
  pinMode(2, INPUT_PULLUP); // bit 0
  pinMode(3, INPUT_PULLUP); // bit 1
  pinMode(4, INPUT_PULLUP); // bit 2
  pinMode(5, INPUT_PULLUP); // bit 3
  pinMode(6, INPUT_PULLUP); // bit 4
  pinMode(7, INPUT_PULLUP); // bit 5
  pinMode(8, INPUT_PULLUP); // bit 6
  pinMode(9, INPUT_PULLUP); // bit 7 or parity
  pinMode(10, INPUT_PULLUP); // clock
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
    bit[i] = !state;
  }
  
  for(int i = 0; i < 8; i++)
  {
    Serial.print(bit[i]);
  }
  
  Serial.println();
  while(!wait)
  {
    if(digitalRead(10) == HIGH)
    {
      wait = true;
    }
  }
}
