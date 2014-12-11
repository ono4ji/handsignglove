#include <SoftwareSerial.h>
 
#define LED_PORT 13
#define BT_RX 11
#define BT_TX 10
// TX(RBT-001) <-> 11(Arduino)
 
#define PORATE 9600
 
SoftwareSerial btSerial(BT_RX, BT_TX);
 
void setup()
{
  ////////////// arduino
  // Initialize LED port 
  pinMode(LED_PORT, OUTPUT);
  digitalWrite(LED_PORT, LOW);
  // Initialize internal serial port (with PC)
  Serial.begin(PORATE);

  ///////////// rn42
  // Initialize software serial port (with Bluetooth)
  btSerial.begin(PORATE);
  
  //sensor
  pinMode(6, INPUT);

}
void loop()
{
  char c;
  
  // Read from software serial port (with Bluetooth)
  if (btSerial.available()){
    c = btSerial.read();
    Serial.write( c );
  }
  
  // read mage
  int sensorValue = analogRead(A1);
  float voltage = sensorValue * (5.0 / 1023.0);
  if(voltage < 0.4){
    Serial.println("mage1:" + String(voltage));
    sendKeyCode((byte)0x04);
    delay(300);
    sendKeyCode((byte)0x00);    
    delay(300);
  }
  
  // read hall 1
  int hallValue1 = digitalRead(6);
  if(hallValue1 > 0){
    Serial.println("hall1:" + String(hallValue1));
    sendKeyCode((byte)0x05);
    delay(300);
    sendKeyCode((byte)0x00);    
    delay(300);    
  }


  delay(5);
}
void sendKeyCode(byte key){
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
}
