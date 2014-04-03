void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

void loop() {
  for(int i = 0; i < 8; i++)
  {
    unsigned char c = 0;
    bitSet(c,i);
    Serial.write(c);
  }
  
  delay(2000);
}
