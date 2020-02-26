#include <SharpIR.h>
#define LS1 A0
#define LS2 A1
#define LS3 A2
#define LS4 A3
#define LS5 A4
#define LS6 A5
#define LS7 A6
#define model 1080
SharpIR L1(LS1, model);
SharpIR L2(LS2, model);
SharpIR L3(LS3, model);
SharpIR L4(LS4, model);
SharpIR L5(LS5, model);
SharpIR L6(LS6, model);
SharpIR L7(LS7, model);

// Rx signal is read after reciving the header
const int headerLength = 8;
bool header [headerLength] = {1,0,1,1,0,0,1,0}; 

const int RxLength = 16;
bool RxSignal [RxLength];

const int TxLength = 56;
bool TxSignal [TxLength];
 
unsigned long checkoutTime = millis();
unsigned long currentTime;
unsigned long previousTime = millis();
int period = 10;

// Define wheel pins
int frontR1(2),frontR2(3), frontL1(4),frontL2(5), backR1(6),backR2(7), backL1(8),backL2(9); 
// Define pins for reception and transmission
int Rx(11), Tx(12);
// Array to store previous moves so the robot can backtrack
char pastMoves[20];
int pastMoveCount = 0;

void setup()
{
 pinMode(Rx, INPUT); // Rx pin
 pinMode(Tx, OUTPUT); // Tx pin
 pinMode(frontR1, OUTPUT);
 pinMode(frontR2, OUTPUT);
 pinMode(frontL1, OUTPUT);
 pinMode(frontL2, OUTPUT);
 pinMode(backR1, OUTPUT);
 pinMode(backR2, OUTPUT);
 pinMode(backL1, OUTPUT);
 pinMode(backL2, OUTPUT);

 Serial.begin(9600);
}

void loop()
{
  currentTime = millis();
  //Read Rx input once every period
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

  /*if(currentTime > (checkoutTime + 30000)){
    Escape();
    sendDistance();
    checkoutTime = currentTime;
   }*/

   //Test sending distances
    while (Serial.available()){
     char a = Serial.read(); // Read incoming character
     if (a=='x') { 
        sendDistance();
        PrintTx();
     }
    }
    
}

// performAction - Reads the incoming message and performs the necessary action
void performAction(){
  
  int RxMessage = RxInteger();
  if (RxMessage == 4){ // Forward
    moveForward();
    Serial.println("Forward");
  }
  else if (RxMessage == 5){ // Backwards
    moveBack();
    Serial.println("Backward");
  }
  else if (RxMessage == 6){ // Left
    turnLeft();
    Serial.println("Left");
  }
  else if (RxMessage == 7){ // Right
    turnRight();
    Serial.println("Right");
  }
  else if (RxMessage == 8){ // Escape
    Escape();
    Serial.println("Escape");
  }
  else if (RxMessage == 9){ // Send Distance
    sendDistance();
    Serial.println("Send Distance");
  }
}

//Turn all wheels off
void wheelsOff(){
  digitalWrite(frontR1, LOW);
  digitalWrite(frontR2, LOW);
  digitalWrite(frontL1, LOW);
  digitalWrite(frontL2, LOW);
  digitalWrite(backR1, LOW);
  digitalWrite(backR2, LOW);
  digitalWrite(backL1, LOW);
  digitalWrite(backL2, LOW);
}

//moveForward - Moves the robot forward, saves move, sends distance
void moveForward(){
  digitalWrite(frontR1, HIGH);
  digitalWrite(frontL1, HIGH);
  digitalWrite(backR1, HIGH);
  digitalWrite(backL1, HIGH);
  
  delay(200);
  wheelsOff();
  
  pastMoves[pastMoveCount] = 'b';
  pastMoveCount++;
}

//moveBack - Moves the robot backwards, saves move, sends distance
void  moveBack(){
  digitalWrite(frontR2, HIGH);
  digitalWrite(frontL2, HIGH);
  digitalWrite(backR2, HIGH);
  digitalWrite(backL2, HIGH);
  
  delay(200);
  wheelsOff();
  
  pastMoves[pastMoveCount] = 'f';
  pastMoveCount++;
}

//turnLeft - Turnes the robot left, saves move, sends distance
void turnLeft (){
  digitalWrite(frontR2, HIGH);
  digitalWrite(frontL1, HIGH);
  digitalWrite(backR2, HIGH);
  digitalWrite(backL1, HIGH);

  delay(100);
  wheelsOff();
  
  pastMoves[pastMoveCount] = 'r';
  pastMoveCount++;
}

//turnRight - Turns the robot right, saves move, sends distance
void turnRight (){
  digitalWrite(frontR1, HIGH);
  digitalWrite(frontL2, HIGH);
  digitalWrite(backR1, HIGH);
  digitalWrite(backL2, HIGH);

  delay(100);
  wheelsOff();

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
  int dis[7]= {51,64,77,88,90,120,5};

  /*
  dis[0]= L1.distance();
  dis[1]= L2.distance();
  dis[2]= L3.distance();
  dis[3]= L4.distance();
  dis[4]= L5.distance();
  dis[5]= L6.distance();
  dis[6]= L7.distance();
  */

  //Convert the distances to 8-bit binary numbers and store them in TxSignal Array
  //TxSingal = dis1,dis2,dis3,dis5,dis5,dis6,dis7
  int k = TxLength - 1;
  for(int i=6; i >= 0; i--){
    for(int j=0; j < 8; j++){
      TxSignal[k] = dis[i]%2;
      dis[i] = dis[i]/2;
      k--;
    }
  }
  
  //Send the bits stored in TxSignal
  sendMessage();
 return;
}

// sendMessage - Send an array of bits
void sendMessage(){  
  for(int i =0;i < headerLength; i++){
    digitalWrite(Tx, header[i]);
    delay(period);
    }
  
  for (int i = 0; i < TxLength; i++){
    digitalWrite(Tx, TxSignal[i]);
    delay(period);
    }
    
  digitalWrite(Tx, 0);
  return;
}

//checkHeader - Compare recorded Rx bits to the header
bool checkHeader(){
  for(int i = 0; i < headerLength; i++){
    if(RxSignal[i] != header[i]){
      return false;
    }
  }
  return true;
}

//appendSignal - Add new to array of recorded Rx inputs
void appendSignal(bool RxNewBit){
  for(int i=0; i < (RxLength - 1); i++){
    RxSignal[i] = RxSignal[i+1];
  }
  RxSignal[RxLength - 1] = RxNewBit;
  return;
}

//Decode the integer value of the Rx bits after the header
int RxInteger(){
  int decimal = 0;
  for(int i = headerLength; i < RxLength; i++){
    decimal = (decimal*2) + RxSignal[i];
    }
  return decimal;
}

//(Function For Testing) Print TxSignal
void PrintTx(){
  Serial.println("\nSignal:");
  for(int i=0; i< 7; i++){
    for(int j=0; j < 8; j++){
      int index = (8*i)+j;
      //Serial.print(index);
      Serial.print(TxSignal[index]);
      }
    Serial.print(" ");
  }
  return;
}
