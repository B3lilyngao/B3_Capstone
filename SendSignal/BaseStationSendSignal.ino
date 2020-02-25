int Rx(11), Tx(12); // Antennas
// All messages are 8-bits with header bits attached to the front
const int period = 10;
unsigned long currentTime;
unsigned long previousTime = millis();

const int headerLength = 8;
const int TxLength = 8;
bool header[headerLength] = {1,0,1,1,0,0,1,0};

const int RxLength = 56 + headerLength;
bool RxSignal [RxLength];

int distances[7]={0,0,0,0,0,0,0};

void setup() {
  Serial.begin(9600);
  pinMode(Rx, INPUT); // Rx pin
  pinMode(Tx, OUTPUT); // Tx pin
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {

  //Read Rx input once every period
  if(currentTime >= (previousTime + period)){
    previousTime = currentTime;
    appendSignal(digitalRead(Rx));
    
    if(checkHeader()){
      Serial.println(" -header ");
      getDistances();
      }
  }

  
  while (Serial.available()){
    char a = Serial.read(); // Read incoming character
    // Based on GUI input, transmit signal to robot
    switch (a) { 
      case ('F'): //Move forward
        transmitSignal(4);
        break;
      case ('B'): //Move backward
        transmitSignal(5);
        break;
      case ('L'): //Turn Left
        transmitSignal(6);
        break;
      case ('R'): //Turn Right
        transmitSignal(7);
        break;
      case ('X'): //Escape
        transmitSignal(8);
        break;
      case ('D'): //Get distances
        transmitSignal(9);
    }
  }
}

// transmitSignal -- Transmits signal (sig) to robot
void transmitSignal(int sig){
  bool sigBinary[TxLength];
  //Convert sig to binary
  for(int i = (TxLength - 1); i >=0 ; i--){
    sigBinary[i]=sig%2;
    sig = sig/2;
    }
  
  //Send header first
  for(int i = 0; i < headerLength; i++){
    digitalWrite(Tx, header[i]);
    digitalWrite(LED_BUILTIN, header[i]);
    delay(period);
    }

  //Convert sigBinary bits after header is sent
  Serial.println("Sent:");
  for (int i = 0; i < TxLength ; i++){
    bool b = sigBinary[i];
    Serial.print(b);
    digitalWrite(Tx, b);
    delay(period);
  }
  digitalWrite(Tx,0);
  return;
}

//appendSignal - Add new to array of recorded Rx inputs
void appendSignal(bool RxNewBit){
  for(int i=0; i < (RxLength - 1); i++){
    RxSignal[i] = RxSignal[i+1];
  }
  RxSignal[RxLength - 1] = RxNewBit;
  return;
}

//Convert binary to decimal, Rx bits between start and stop index
int RxInteger(int startIndex, int stopIndex){
  int decimal = 0;
  for(int i = startIndex; i <= stopIndex; i++){
    decimal = (decimal*2) + RxSignal[i + startIndex];
    }
  return decimal;
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

//getDistances
void getDistances(){
  for(int i=0;i<7;i++){
  int startIndex = headerLength + (i*8);
  int stopIndex = startIndex + 7;
  distances[i]= RxInteger(startIndex,stopIndex);
  Serial.println("Distance ");
  Serial.print(i);
  Serial.print(" = ");
  Serial.print(distances[i]);
  }
}

