/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/
#define VD_1  13 
#define VD_2  12 
#define VM_1 11                                   // define pino D10 como Rx
#define VM_2 10  
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(VD_1, OUTPUT);
  pinMode(VD_2, OUTPUT);
  pinMode(VM_1, OUTPUT);
  pinMode(VM_2, OUTPUT);
  digitalWrite(VD_1, LOW);
  digitalWrite(VD_2, LOW);
  digitalWrite(VM_1, LOW);
  digitalWrite(VM_2, LOW);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(VD_1, HIGH);  
  digitalWrite(VM_2, HIGH); 
  delay(2000);  
  digitalWrite(VD_1, LOW);  
  digitalWrite(VM_2, LOW);
  delay(2000);  
    digitalWrite(VD_2, HIGH);  
    digitalWrite(VM_1, HIGH); 
    delay(2000);  
    digitalWrite(VD_2, LOW);  
    digitalWrite(VM_1, LOW); 
    delay(2000); 
    digitalWrite(VM_1, HIGH); 
    digitalWrite(VM_2, HIGH);
    delay(2000);
    digitalWrite(VM_1, LOW); 
    digitalWrite(VM_2, LOW);
    delay(2000);
    digitalWrite(VD_1, HIGH);  
    digitalWrite(VD_2, HIGH); 
    delay(2000);
    digitalWrite(VD_1, LOW );  
    digitalWrite(VD_2, LOW);


        
}
