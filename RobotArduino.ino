// If get wireless signal:
// 11-001-000 move forward, i.e. moveForward()
// 11-010-000 move backward, i.e. moveBackward()
// 11-011-000 move left, i.e. moveLeft()
// 11-100-000 move right, i.e. moveRight()
// 11-101-000 return along escape path, i.e. Escape()
// 11-110-000 transmit sensor data, i.e. sendDistance()

// Transmit distance sensor values once after every move, i.e. sendDistance()

// All transmissions are 8-bit, starting with header 11, to be 11xxxxxx

// *Parshva - Make pins for distance sensors here
int frontRight(2), frontLeft(3), backRight(4), backLeft(5); // Wheels
int Rx(11), Tx(12); // Antennas
char pastMoves[20]; // Assuming a maximum of 20 moves stored
int pastMoveCount = 0;
unsigned long startTime; // Timeout timer variable, to be set in the performAction function

void setup()
{
 pinMode(Rx, INPUT); // Rx pin
 pinMode(Tx, OUTPUT); // Tx pin
 pinMode(frontRight, OUTPUT);
 pinMode(frontLeft, OUTPUT);
 pinMode(backRight, OUTPUT);
 pinMode(backLeft, OUTPUT);
 // *Parshva - Make pins for distance sensors here
}

void loop()
{
  if (digitalRead(Rx)) {
    clockTick();
    if (digitalRead(Rx)){
      clockTick();
      performAction();
    }
  }
  bool ACK = 0;
  while (!checkTimeout(startTime) && !ACK){
    if (digitalRead(Rx)) {
      clockTick();
      if (digitalRead(Rx)){
        ACK = 1;
      }
    }
  }
  if (checkTimeout(startTime) && !ACK){
    Escape();
  }
}

// performAction - Reads the incoming message and performs the necessary action
void performAction(){
  startTime = millis();
  String RxMessage = "";
  for (int i = 0; i < 6; i++){
    RxMessage += digitalRead(Rx);
    clockTick();
  }
  if (RxMessage == "001000"){ // Forward
    moveForward();
  }
  else if (RxMessage == "010000"){ // Backwards
    moveBack();
  }
  else if (RxMessage == "011000"){ // Left
    turnLeft();
  }
  else if (RxMessage == "100000"){ // Right
    turnRight();
  }
  else if (RxMessage == "101000"){ // Escape
    Escape();
  }
  else if (RxMessage == "110000"){ // Send Distance
    sendDistance();
  }
}

//moveForward - Moves the robot forward, saves move, sends distance
void moveForward(){
  digitalWrite(frontRight, HIGH);
  digitalWrite(frontLeft, HIGH);
  
  delay(1000);
  digitalWrite(frontRight, LOW);
  digitalWrite(frontLeft, LOW);
  
  pastMoves[pastMoveCount] = 'b';
  pastMoveCount++;
  sendDistance();
}

//moveBack - Moves the robot backwards, saves move, sends distance
void  moveBack(){
  digitalWrite(backRight, HIGH);
  digitalWrite(backLeft, HIGH);
  
  delay(1000);
  digitalWrite(backRight, LOW);
  digitalWrite(backLeft, LOW);
  
  pastMoves[pastMoveCount] = 'f';
  pastMoveCount++;
  sendDistance();
}

//turnLeft - Turnes the robot left, saves move, sends distance
void turnLeft (){
  digitalWrite(frontRight, HIGH);
  digitalWrite(backLeft, HIGH);

  delay(1000);
  digitalWrite(frontRight, LOW);
  digitalWrite(backLeft, LOW);
  
  pastMoves[pastMoveCount] = 'r';
  pastMoveCount++;
  sendDistance();
}

//turnRight - Turns the robot right, saves move, sends distance
void turnRight (){
  digitalWrite(frontLeft, HIGH);
  digitalWrite(backRight, HIGH);

  delay(1000);
  digitalWrite(frontLeft, LOW);
  digitalWrite(backRight, LOW);

  pastMoves[pastMoveCount] = 'l';
  pastMoveCount++;
  sendDistance();
}

//Escape - Iterates through saved escape route (pastMoves)and moves the robot accordingly
void Escape(){
  for (int i = pastMoveCount; i >= 0; i--){
    char m = pastMoves[i];
    switch (m){
      case ('f'): // Forward
         moveForward();
         break;
       case ('b'): // Backward
         moveBack();
         break;
       case ('l'): // Left
         turnLeft();
         break;
       case ('r'): // Right
         turnRight();
         break;
    }
    // *To Add - Pause, then if NoACK, Continue
  }
  pastMoveCount = 0;
}

// sendDistance - Transmits distance sensor data, is implemented during every move
void sendDistance(){
  // *Parshva - Use this area to send sensor data as a single 6-bit block to return 
  // Implementing how I have this far (which we can change, of course), requires you to:
  // 1 Get sensor data
  // 2 Make a 6-bit block out of it for each of 5 sensors (e.g. 100101)
  // 3 Iterate on that that through the for-loop below
  // There's probably a more graceful and abstracted way of achieving this, but I have yet to think of it.
  for (int i = 0; i < 5; i++){
    digitalWrite(Tx, 1);
    clockTick();
    digitalWrite(Tx, 1);
    clockTick();
    for (int i = 0; i < 6; i++){
      digitalWrite(Tx, 0); // Incrementally write data for sensor 1, 2, 3, etc...
      clockTick();
    }
  }
}

// clockTick - One tick of the clock, the decided-upon duration between the transmission of bits
void clockTick(){
  delay(1); //1 ms
}

// checkTimeout - Check to see if, based on the time the message was sent, it has timed out before sending another message
bool checkTimeout(unsigned long startTime){
  unsigned long currTime = millis();
  if (currTime - startTime < 30000){ // Timeout of 30 seconds
    return true;
  }
  else return false; // When robot times out, it will escape
}
