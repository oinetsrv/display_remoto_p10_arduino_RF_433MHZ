// Mapeamento de Hardware

#define bt1 15
#define led1 13

// variables will change:
void readKey();
unsigned int duty1 = 1;
static boolean flag2 = 0;

void setup() {
  Serial.begin(9600); //inicia com. serial 115200 bits/s
  pinMode(led1, OUTPUT);
  pinMode(bt1, INPUT);
  digitalWrite(led1, LOW);
}

////////////////////////////

void loop() {

  readKey();

  delay(10);

} //end loop

void readKey() {
  static boolean flag1 = 0;

  if (digitalRead(bt1)){

    if (flag2 == 1)
    {
          digitalWrite(led1, flag2);
          Serial.println("Led1   ligado!");
    }
    flag1 = 0x01;
    flag2 = 0x00;
    
    delay(10);
  } 
 
  if (!digitalRead(bt1) && flag1) {
    flag1 = 0x00;
    flag2 = 0x01;
    digitalWrite(led1, flag1);
    Serial.println("Led1 desligado");
    delay(10);
  }
 
}