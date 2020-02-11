// When robot receives a command from the serial port, it moves accordingly
// These serial port commands are to be later replaced with wireless signals received

char a;
int frontRightF(2), frontLeftF(4), backRightF(5), backLeftF(8); // Pins to move wheels forward
int frontRightB(3), frontLeftB(5), backRightB(7), backLeftB(9); // Pins to move wheels backward
char pastMoves[500]; // Array to save the past moves for the Escape functionality
int moveCounter = 0;
//int Rx(11), Tx(12); // Antennas

void setup()
{
 //pinMode(Rx, INPUT); // Rx pin
 //pinMode(Tx, OUTPUT); // Tx pin
 pinMode(frontRightF, OUTPUT);
 pinMode(frontLeftF, OUTPUT);
 pinMode(backRightF, OUTPUT);
 pinMode(backLeftF, OUTPUT);
 pinMode(frontRightB, OUTPUT);
 pinMode(frontLeftB, OUTPUT);
 pinMode(backRightB, OUTPUT);
 pinMode(backLeftB, OUTPUT);
 Serial.begin(9600); // Open serial Port
}

void loop()
{
  /*if (digitalRead(Rx)){
    // This is where you'll read the wireless signal
  }*/
  
  while (Serial.available()){
    a = Serial.read(); // Read command from the serial port as 'a'
    Serial.write(a);
    
    switch (a){
      case('f'):
        moveForward();
        break;
      case('b'):
        moveBackward();
        break;
      case('l'):
        turnLeft();
        break;
      case('r'):
        turnRight();
        break;
      case('e'):
        incrementEscape();
        break;
    }
    delay(1000);
    stopWheels();
    pastMoves[moveCounter] = a;
    moveCounter++;
  }
}

// moveForward - Moves the robot forward
void moveForward(){
  digitalWrite(frontRightF, HIGH);
  digitalWrite(frontLeftF, HIGH);
  digitalWrite(backRightF, HIGH);
  digitalWrite(backLeftF, HIGH);
}

// moveBackward - Moves the robot backward
void moveBackward(){
  digitalWrite(frontRightB, HIGH);
  digitalWrite(frontLeftB, HIGH);
  digitalWrite(backRightB, HIGH);
  digitalWrite(backLeftB, HIGH);
}

// turnLeft - Turns the robot left
void turnLeft(){
  digitalWrite(frontRightF, HIGH);
  digitalWrite(backRightF, HIGH);
  digitalWrite(frontLeftB, HIGH);
  digitalWrite(backLeftB, HIGH);
}

// turnRight - Turns the robot right
void turnRight(){
  digitalWrite(frontLeftF, HIGH);
  digitalWrite(backLeftF, HIGH);
  digitalWrite(frontRightB, HIGH);
  digitalWrite(backRightB, HIGH);
}

// incrementEscape - Iterated the robot backwards through the inverse of its past moves
// Currently, just escapes straight out to the beginning
// When wireless communication is incorporated, this ideally send distance back after each move and pause
void incrementEscape(){
  for (int i = moveCounter; i > 0; i--){// Iterates for every move saved in the pastMoves array
    char escapeMove = pastMoves[i];
    switch(escapeMove){
      case('f'):
        moveBackward();
        break;
      case('b'):
        moveForward();
        break;
      case('r'):
        turnLeft();
        break;
      case('l'):
        turnRight();
        break;
    }
    delay(1000);
    stopWheels();
    moveCounter--;
  }
}

// stopWheels - Stops all of the wheels from moving
void stopWheels(){
  digitalWrite(frontRightF, LOW);
  digitalWrite(frontLeftF, LOW);
  digitalWrite(backRightF, LOW);
  digitalWrite(backLeftF, LOW);
  digitalWrite(frontRightB, LOW);
  digitalWrite(frontLeftB, LOW);
  digitalWrite(backRightB, LOW);
  digitalWrite(backLeftB, LOW);
}
