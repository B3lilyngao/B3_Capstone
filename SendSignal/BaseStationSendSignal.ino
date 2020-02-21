// If written by GUI in serial:
// "f" move forward, i.e. send 11-001-000
// "b" move backward, i.e. send 11-010-000
// "l" move left, i.e. send 11-011-000
// "r" move right, i.e. send 11-100-000
// "e" return along escape path, i.e. send 11-101-000
// "t" transmit sensor data, i.e. send 11-110-000

// Write to serial the distance sensor values

// All transmissions are 8-bit, starting with header 11, to be 11xxxxxx
const int TxPeriod = 10;
const int headerLength = 8;
bool header[headerLength] = {1,0,1,1,0,0,1,0};
char a; // Serial read value
int Rx(11), Tx(12); // Antennas


void setup() {
  Serial.begin(9600);
  pinMode(Rx, INPUT); // Rx pin
  pinMode(Tx, OUTPUT); // Tx pin
  pinMode(LED_BUILTIN, OUTPUT);
  int  a =1;
}

void loop() {
  while (Serial.available()){
    char a = Serial.read(); // Read incoming character
    
    switch (a) { // Based on GUI input, transmit signal to robot
      case ('F'):
        transmitSignal("001000"); // 001
        break;
      case ('B'):
        transmitSignal("010000"); // 010
        break;
      case ('L'):
        transmitSignal("011000"); // 011
        break;
      case ('R'):
        transmitSignal("100000"); // 100
        break;
      case ('X'):
        transmitSignal("101000"); // 101
        break;
      case ('Z'):
        transmitSignal("110000"); // 110
    }
  }
}

// transmitSignal -- Transmits signal (sig) to robot
void transmitSignal(String sig){
  for(int i = 0; i < headerLength; i++){
    digitalWrite(Tx, header[i]);
    digitalWrite(LED_BUILTIN, header[i]);
    delay(TxPeriod);
    }
  for (int i = 0; i < sig.length(); i++){
    int b = sig[i];
    b-=48;
    Serial.print(b);
    digitalWrite(Tx, b); // Iterate through the signified message and transmit each digit
    digitalWrite(LED_BUILTIN, b);cdxzxszxs]
    delay(TxPeriod);
  }
}
