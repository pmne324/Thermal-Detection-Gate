/*
Author : Porya M.Nilaore
Email : pmne324@gmail.com
github : github.com/pmne324/gate
From : AUTSpace Lab.
*/

#define  gatewide  80
#define  washbox  20
#define  washdelay  2000
#define  pass  1000

//Define Arduino NANO pin number
const int sw     =   2;    // Gate open switch - pull down
const int hkey   =   3;    // Horizontal chek key - pull down
const int vkey   =   4;    // Vertical chek key - pull down
const int motor  =   5;    // Gate motor
const int washer =   6;    // Washer pump
const int GateEchoPin =  7;   // Echo Pin of Ultrasonic Sensor
const int GatePingPin =  8;   // Trigger Pin of Ultrasonic Sensor
const int WashEchoPin =  9;   // Echo Pin of Ultrasonic Sensor
const int WashPingPin =  10;  // Trigger Pin of Ultrasonic Sensor
const int buzzer      =  12;  // Buzzer pin for statrtup and alert

int  angle;
long duration, distance;
int  door, dist, gatecheck, initcheck, washcheck;
int  varh, varv, varsw;
  
void setup() {
  //test
  Serial.begin(9600); // Starting Serial Terminal
  
  //Arduino Nano Configuration
  pinMode(sw,   INPUT);
  pinMode(hkey, INPUT);
  pinMode(vkey, INPUT);
  pinMode(motor,  OUTPUT);
  pinMode(washer, OUTPUT);
  pinMode(GatePingPin, OUTPUT);
  pinMode(GateEchoPin, INPUT);
  pinMode(WashPingPin, OUTPUT);
  pinMode(WashEchoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  
initializing:  
  //check for gate closed
  gatecheck = gateultson();
  if (gatecheck <= gatewide){
    initcheck = 1;
  }else{
    initcheck = 0;
  }
  varsw = digitalRead(sw);
  varh = digitalRead(hkey);
  varv = digitalRead(vkey);
  Serial.println(varsw);
  Serial.println(varh);
  Serial.println(varv);
  Serial.println(initcheck);
  if (varh == 1){
    Serial.println("Door is Close!");
    digitalWrite(motor, LOW);
  }else if (varh == 0){
    if (initcheck == 1)
    {
      digitalWrite(buzzer, HIGH);
      Serial.println("Someone is in gate!");
      delay(3000);
      digitalWrite(buzzer, LOW);
      goto initializing;
    }else{
      Serial.println("Door is Open!");
      do
      {
        digitalWrite(motor, HIGH);
        Serial.println("Motor is Online");
        varh = digitalRead(hkey); 
        delay(10);
      }while(varh == 0);
      digitalWrite(motor, LOW);  
      Serial.println("Door Closed!");
    }
  }
}

void loop() {
//Gate Control
  //Open Gate
  varsw = digitalRead(sw);
  varh = digitalRead(hkey);
  Serial.println(varsw);
  Serial.println(varh);
  if (varsw == 1 && varh == 1){
    do{
      digitalWrite(motor, HIGH);
      varv = digitalRead(vkey);
      Serial.println("Motor Online!");
      Serial.println("Door is opening.");
      Serial.println(varv);
      delay(10);
    }while(varv == 0);
    digitalWrite(motor, LOW);
    Serial.println(varv);
    Serial.println("Motor Offline!");
    Serial.println("Door is close.");
  }else if (varsw == 1 && varh == 0){
    digitalWrite(buzzer, HIGH);
    Serial.println("Wrong Selection");
    Serial.println("Door is Open.");
    delay(500);
    digitalWrite(buzzer, LOW);
  }
  //Close Gate
gateclose:
  gatecheck = gateultson();  
  varv = digitalRead(vkey);  
  if (gatecheck <= gatewide && varv == 1){
    delay(pass);
    gatecheck = gateultson();  
    if (gatecheck <= gatewide){
      digitalWrite(buzzer, HIGH);
      Serial.println("Gate is buzy!");
      Serial.println("Check gate again");
      delay(1000);
      digitalWrite(buzzer, LOW);
      goto gateclose;
    }else{
      do{
        digitalWrite(motor, HIGH);
        Serial.println("Motor is Online!");
        Serial.println("Door is closing.");      
        varh = digitalRead(hkey);
        delay(10);
      }while(varh == 0);      
    }
  }
  
  //Wash Control
  washcheck = 25;
  if (washcheck <= washbox){
    digitalWrite(washer, HIGH);
    Serial.println("Washeris Online");
    Serial.println(washcheck);
    delay(washdelay);
    digitalWrite(washer, LOW);
    Serial.println("Washeris Offline");
    Serial.println(washcheck);
  }
}

long gateultson(){
  digitalWrite(GatePingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(GatePingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(GatePingPin, LOW);
  duration = pulseIn(GateEchoPin, HIGH);
  distance = microsecondsToCentimeters(duration);
  return distance;  
}

long washultson(){
  digitalWrite(WashPingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(WashPingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(WashPingPin, LOW);
  duration = pulseIn(WashEchoPin, HIGH);
  distance = microsecondsToCentimeters(duration);
  return distance;  
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
