// TF-Mini or TF-Luna
#define TFMINI_BAUDRATE 115200 // bauds
#define TFMINI_DATARATE 10.0f // ms
#define led PB12
int distance = 0;
int strength = 0;
float temp = 0;

//Micros para o motor de passo
unsigned long microsTarefa1 = micros();
unsigned long millisTarefa2 = micros();


// define números de pinos do driver no arduino
#define stepPin PA8
#define dirPin PB15


void setup(){
  // Stepper
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  // Serial ports
  pinMode(led, OUTPUT);
  Serial3.begin(TFMINI_BAUDRATE); // TF mini
  flushSerial3();
}

void loop(){
  rodarMotorDePasso();
  /*getTFminiData(&distance, &strength, &temp);
  if(distance < 100){
    digitalWrite(led, HIGH);
  }
  else{
    digitalWrite(led, LOW);
  }*/
}

// Flushes the INPUT serial buffer
void flushSerial3(){
  while(Serial3.available()){Serial3.read();}
}
void getTFminiData(int* distance, int* strength, float* temp) {
  static char i = 0;
  char j = 0;
  int checksum = 0;
  static int rx[9];
  while(1){
  if(Serial3.available())
  { 
      rx[i] = Serial3.read();
//      Serial.println(rx[i],HEX);
      if(rx[0] != 0x59) {
        i = 0;
      } else if(i == 1 && rx[1] != 0x59) {
        i = 0;
      } else if(i == 8) {
        for(j = 0; j < 8; j++) {
          checksum += rx[j];
        }
        if(rx[8] == (checksum % 256)) {
          *distance = rx[2] + rx[3] * 256;
          *strength = rx[4] + rx[5] * 256;
          *temp = (rx[6] + rx[7] * 256.0f)/8 - 256;
        }
        i = 0;
        break;
      } else
      {
        i++;
      }
    }
  }
  flushSerial3();
}

// permite que o motor se mova em uma direção particular
void rodarMotorDePasso(){
    digitalWrite(dirPin, HIGH);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
}
