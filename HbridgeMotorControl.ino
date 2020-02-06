int w11(2),w12(3),w21(4),w22(5),w31(6),w32(7),w41(8),w42(9);
void setup()
{

  pinMode(w11, OUTPUT);
  pinMode(w12, OUTPUT);
  pinMode(w21, OUTPUT);
  pinMode(w22, OUTPUT);
  pinMode(w31, OUTPUT);
  pinMode(w32, OUTPUT);
  pinMode(w41, OUTPUT);
  pinMode(w42, OUTPUT);
  pinMode(Ain,INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  time=(millis);
}

// -1 Spins wheel backward
//  1 Spins wheel forward
void spin(int w1, int w2, int w3, int w4){
  //Turn wheel 1
  if(w1==1){
    digitalWrite(w11, LOW);
    digitalWrite(w12, HIGH);
  }
  else if(w1==-1){
    digitalWrite(w11, HIGH);
    digitalWrite(w12, LOW);
  }
  else{
    digitalWrite(w11, LOW);
    digitalWrite(w12, LOW);
  }

  //Turn wheel 2
  if(w2==1){
    digitalWrite(w21, LOW);
    digitalWrite(w22, HIGH);
  }
  else if(w2==-1){
    digitalWrite(w21, HIGH);
    digitalWrite(w22, LOW);
  }
  else{
    digitalWrite(w21, LOW);
    digitalWrite(w22, LOW);
  }

  //Turn wheel 3
  if(w3==1){
    digitalWrite(w31, LOW);
    digitalWrite(w32, HIGH);
  }
  else if(w3==-1){
    digitalWrite(w31, HIGH);
    digitalWrite(w32, LOW);
  }
  else{
    digitalWrite(w31, LOW);
    digitalWrite(w32, LOW);
  }

  //Turn wheel 4
  if(w4==1){
    digitalWrite(w41, LOW);
    digitalWrite(w42, HIGH);
  }
  else if(w4==-1){
    digitalWrite(w41, HIGH);
    digitalWrite(w42, LOW);
  }
  else{
    digitalWrite(w41, LOW);
    digitalWrite(w42, LOW);
  } 
}

