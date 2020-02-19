// If get wireless signal:
// 11-001-000 move forward, i.e. moveForward()
// 11-010-000 move backward, i.e. moveBackward()
// 11-011-000 move left, i.e. moveLeft()
// 11-100-000 move right, i.e. moveRight()
// 11-101-000 return along escape path, i.e. Escape()
// 11-110-000 transmit sensor data, i.e. sendDistance()

// Transmit distance sensor values once after every move, i.e. sendDistance()

// All transmissions are 8-bit, starting with header
const int headerLength = 8;
bool header [headerLength] = {1,0,1,1,0,0,1,0}; 

const int RxLength = 14;
bool RxSignal [RxLength];

unsigned long checkoutTime = millis();
unsigned long currentTime;
unsigned long previousTime = millis();
int period = 10;


// *Parshva - Make pins for distance sensors here
int frontRight(2), frontLeft(4), backRight(6), backLeft(8); // Wheels
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

 Serial.begin(9600);

 
}

void loop()
{
  currentTime = millis();
  
  if(currentTime >= (previousTime + period)){
    previousTime = currentTime;
    appendSignal(digitalRead(Rx));
    
    if(checkHeader()){
      Serial.println(" -header ");
      performAction();
      checkoutTime = currentTime;
      sendDistance();
      }
  }

  if(currentTime > (checkoutTime + 30000)){
    //Escape();
    sendDistance();
    checkoutTime = currentTime;
   }
}

// performAction - Reads the incoming message and performs the necessary action
void performAction(){
  
  int RxMessage = RxInteger();
  if (RxMessage == 8){ // Forward
    moveForward();
    Serial.println("Forward");
  }
  else if (RxMessage == 16){ // Backwards
    moveBack();
    Serial.println("Backward");
  }
  else if (RxMessage == 24){ // Left
    turnLeft();
    Serial.println("Left");
  }
  else if (RxMessage == 32){ // Right
    turnRight();
    Serial.println("Right");
  }
  else if (RxMessage == 40){ // Escape
    Escape();
    Serial.println("Escape");
  }
  else if (RxMessage == 48){ // Send Distance
  }
}

//moveForward - Moves the robot forward, saves move, sends distance
void moveForward(){
  digitalWrite(frontRight, HIGH);
  digitalWrite(frontLeft, HIGH);
  
  delay(500);
  digitalWrite(frontRight, LOW);
  digitalWrite(frontLeft, LOW);
  
  pastMoves[pastMoveCount] = 'b';
  pastMoveCount++;
}

//moveBack - Moves the robot backwards, saves move, sends distance
void  moveBack(){
  digitalWrite(backRight, HIGH);
  digitalWrite(backLeft, HIGH);
  
  delay(500);
  digitalWrite(backRight, LOW);
  digitalWrite(backLeft, LOW);
  
  pastMoves[pastMoveCount] = 'f';
  pastMoveCount++;
}

//turnLeft - Turnes the robot left, saves move, sends distance
void turnLeft (){
  digitalWrite(frontRight, HIGH);
  digitalWrite(backLeft, HIGH);

  delay(500);
  digitalWrite(frontRight, LOW);
  digitalWrite(backLeft, LOW);
  
  pastMoves[pastMoveCount] = 'r';
  pastMoveCount++;
}

//turnRight - Turns the robot right, saves move, sends distance
void turnRight (){
  digitalWrite(frontLeft, HIGH);
  digitalWrite(backRight, HIGH);

  delay(100);
  digitalWrite(frontLeft, LOW);
  digitalWrite(backRight, LOW);

  pastMoves[pastMoveCount] = 'l';
  pastMoveCount++;
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

//sendDistance - measure distance using sensor inputs
void sendDistance(){
 return;
}

// sendMessage - Send an array of bits
void sendMessage(int *message, int messageLength){
  for (int i = 0; i < messageLength; i++){
    digitalWrite(Tx, message[i]);
    delay(period);
    }
    digitalWrite(Tx, 0);
    return;
}

//checkHeader - Compare recorded bits to the header
bool checkHeader(){
  for(int i = 0; i < headerLength; i++){
    if(RxSignal[i] != header[i]){
      return false;
    }
  }
  return true;
}

//appendSignal - Add new to array of recorded inputs
void appendSignal(bool RxNewBit){
  for(int i=0; i < (RxLength - 1); i++){
    RxSignal[i] = RxSignal[i+1];
  }
  RxSignal[RxLength - 1] = RxNewBit;
  return;
}

int RxInteger(){
  int decimal = 0;
  for(int i = headerLength; i < RxLength; i++){
    decimal = (decimal*2) + RxSignal[i];
    }
  return decimal;
}
