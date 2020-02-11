// If written by GUI in serial:
// "f" move forward, i.e. send 11-001-000
// "b" move backward, i.e. send 11-010-000
// "l" move left, i.e. send 11-011-000
// "r" move right, i.e. send 11-100-000
// "e" return along escape path, i.e. send 11-101-000
// "t" transmit sensor data, i.e. send 11-110-000

// Write to serial the distance sensor values

// All transmissions are 8-bit, starting with header 11, to be 11xxxxxx

char a; // Serial read value
int Rx(11), Tx(12); // Antennas


void setup() {
  Serial.begin(9600);
  pinMode(Rx, INPUT); // Rx pin
  pinMode(Tx, OUTPUT); // Tx pin

}

void loop() {
  while (Serial.available()){
    a = Serial.read(); // Read incoming character

    switch (a) { // Based on GUI input, transmit signal to robot
      case ('f'):
        transmitSignal("11001000"); // 001
        displaySensorData();
        break;
      case ('b'):
        transmitSignal("11010000"); // 010
        displaySensorData();
        break;
      case ('l'):
        transmitSignal("11011000"); // 011
        displaySensorData();
        break;
      case ('r'):
        transmitSignal("11100000"); // 100
        displaySensorData();
        break;
      case ('e'):
        transmitSignal("11101000"); // 101
        displaySensorData();
        break;
      case ('t'):
        transmitSignal("11110000"); // 110
    }
    int distancesReceived = 0;
    while (distancesReceived < 5){ // Waiting for 5 different sensor values
      if (digitalRead(Rx)){
        clockTick();
        if (digitalRead(Rx)){ // If receive header of 11
            clockTick();
            displaySensorData(); // Read rest of 11xxxxxx message
            distancesReceived++;
        }
      }
    }
    transmitSignal("11"); // Sends ACK to robot to acknowledge having gotten the distances
  }
}

// transmitSignal -- Transmits signal (sig) to robot
void transmitSignal(String sig){
  for (int i = 0; i < sig.length(); i++){
    digitalWrite(Tx, sig.charAt(i)); // Iterate through the signified message and transmit each digit
    clockTick();
  }
}

// displaySensorData - Reads sensor data returned from robot and prints it to serial
void displaySensorData(){
  for (int i = 0; i < 6; i++){
    Serial.write(digitalRead(Rx)); // Write digits of sensor values to Serial Port, to be parsed by the GUI
    clockTick();
  }
}

// clockTick - One tick of the clock, the decided-upon duration between the transmission of bits
void clockTick(){
  delay(10); //10 ms
}

// checkTimeout - Check to see if, based on the time the message was sent, it has timed out before sending another message
/*bool checkTimeout(unsigned long startTime){
  unsigned long currTime = millis();
  if (currTime - startTime < 30000){ // Timeout of 30 seconds
    return true;
  }
  else return false; // *What happens when it times out?
}*/
