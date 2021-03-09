const int BUFFER_SIZE = 14;
char buf[BUFFER_SIZE];

void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
}

void loop() {
  // check if data is available
  if (Serial.available() > 0) {
    // read the incoming bytes:
    int rlen = Serial.readBytesUntil('\n', buf, BUFFER_SIZE);

    // prints the received data
    Serial.println(" ");
    Serial.print("I received: ");
    for(int i = 0; i < rlen; i++)
      Serial.print(buf[i]);
  }
}
