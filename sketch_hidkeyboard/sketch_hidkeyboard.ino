#include <SoftwareSerial.h>
 
#define LED_PORT 13
#define BT_RX 11
#define BT_TX 10
// TX(RBT-001) <-> 11(Arduino)
 
#define PWM_WIDTH 500
// roop = 2ms

#define PORATE 9600
 
SoftwareSerial btSerial(BT_RX, BT_TX);
 
int mode = 0;
int flag1 = 0;
int flag2 = 100;


void setup()
{
  // Initialize LED port 
  pinMode(LED_PORT, OUTPUT);
  digitalWrite(LED_PORT, LOW);
  // Initialize internal serial port (with PC)
  Serial.begin(PORATE);
  // Initialize software serial port (with Bluetooth)
  btSerial.begin(PORATE);
}
void loop()
{
  char c;
  
  // Read from software serial port (with Bluetooth)
  if (btSerial.available()){
    c = btSerial.read();
    Serial.write( c );
  }
 
  // Read from internal serial port (with PC)
  if (Serial.available()){
    c = Serial.read();
 
    if(c == 'z'){
        Serial.write(c);
  sendKeyCode((byte)0x1D);
    }
    if(c == 'e'){
      Serial.write(c);
      sendKeyCode((byte)0x00);
    }
  }
  delay(5);
}
void sendKeyCode(byte key){
  /*
  Serial.print("z");
    btSerial.write(0xFE); // Raw Report Mode
    btSerial.write(0x01); // Raw Report Mode
    btSerial.write((byte)0x00); // Raw Report Mode
    btSerial.write(0x04);

delay(100);
    btSerial.write(0xFE);
    btSerial.write((byte)0x00);
 */
    
    btSerial.write(0xFD); // Raw Report Mode
    btSerial.write(0x09); // Length
    btSerial.write(0x01); // Descriptor 0x01=Keyboard

    btSerial.write((byte)0x00);
    btSerial.write((byte)0x00);
    btSerial.write(key);
    btSerial.write((byte)0x00);
    btSerial.write((byte)0x00);
    btSerial.write((byte)0x00);
    btSerial.write((byte)0x00);
    btSerial.write((byte)0x00);
/*
    delay(100);

    btSerial.write(0xFD); // Raw Report Mode
    btSerial.write(0x09); // Length
    btSerial.write(0x01); // Descriptor 0x01=Keyboard

    btSerial.write((byte)0x00);
    btSerial.write((byte)0x00);
    btSerial.write((byte)0x00);
    btSerial.write((byte)0x00);
    btSerial.write((byte)0x00);
    btSerial.write((byte)0x00);
    btSerial.write((byte)0x00);
    btSerial.write((byte)0x00);
 //        btSerial.print(0x00);
*/
}
