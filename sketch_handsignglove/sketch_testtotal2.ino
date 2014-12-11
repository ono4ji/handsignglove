#include <SoftwareSerial.h>
 
#define LED_PORT 13
#define BT_RX 11
#define BT_TX 10
// TX(RBT-001) <-> 11(Arduino)
 
#define PORATE 9600
SoftwareSerial btSerial(BT_RX, BT_TX);

// 1:oya, 2:hito, 3:naka, 4:oyaM, 5:hitoM
byte seq[10];
int seqIndex = 0;
byte prevSeq = B00000;

// same flag with seq
#define HISTORY_NUM 20
byte history[HISTORY_NUM];
 
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

  // visual
  pinMode(13, OUTPUT);
}

int count = 0;
void loop()
{
  char c;
  
  // Read from software serial port (with Bluetooth)
  if (btSerial.available()){
    c = btSerial.read();
    Serial.write( c );
  }
  
  // read mage
  boolean mage1 = readMageSensor(A3, 0.45);
  boolean mage2 = readMageSensor(A4, 2.2);
  boolean mage3 = readMageSensor(A5, 2.2);

  // read hall 1
  boolean hall1 = digitalRead(6) > 0;
  
  byte current = B00000;
  if(mage1){
    current = current | B00001;
  }
  if(mage2){
    current = current | B00010;
  }
  if(mage3){
    current = current | B00100;
  }
  if(hall1){
    current = current | B01000;
  }
  
  history[count] = current;
  count++;
  // average check
  if(count == HISTORY_NUM){
    int temp[256];
    int i;
    for(i=0; i<256; i++){
      temp[i] = 0;
    }
    for(i=0; i< HISTORY_NUM; i++){
      temp[history[i]] += 1;
    }
    for(i=0; i<256; i++){
      if(temp[i] > HISTORY_NUM * 0.8){
        // ok add seq
        addSeq((byte)i);
        break;
      }
    }
    count = 0;
  }

//  Serial.println("m1:" + String(mage1));
//  Serial.println("m2:" + String(mage2));
//  Serial.println(hall1);
//  Serial.println(current);

  //test
  if (Serial.available()){
    c = Serial.read();
 
    if(c == 'z'){
      Serial.write(c);
      sendKey((byte)0x1D);
    }
    if(c == 'e'){
      Serial.write(c);
      sendKey((byte)0x00);
    }
    if(c == 'g'){
      Serial.println("output force");
      doSeq();
      prevSeq = B00000;
    }    
    if(c == 'f'){
      Serial.println("output force here");
      seq[0] = 2;
      seqIndex = 1;
      doSeq();
      prevSeq = B00000;
    }    
  }


  delay(15);
}

void addSeq(byte current){ 
  if(current == prevSeq){
    //Serial.println("no change");
    return;
  }
  prevSeq = current;
  ledFlash();

  if(current == B00000){
    Serial.println("pop:" + String(current));
    // reset
    if(seqIndex > 0){
      doSeq();
    }else{
      Serial.println("no seq");
    }
    return;
  }else{
    Serial.println("addseq:" + String(current));
    seq[seqIndex] = current;
    seqIndex++;
  }
}

void doSeq(){
  String s = "";
  int i;
  for(i=0; i<seqIndex; i++){
    s += String(seq[i]);
  }
  Serial.println("seq is " + s);
  
  if(s == "2"){
    Serial.println("right");
    sendKey((byte) 0x4F);
  }
  if(s == "1"){
    Serial.println("left");
    sendKey((byte) 0x50);
  }
  if(s == "32"){
    Serial.println("presen start(shift + F5)");
    sendKeyModifier((byte) 0x3E, (byte) 0x02);
  }
  if(s == "762"){
    Serial.println("close(alt + F4)");
    sendKeyModifier((byte) 0x3D, (byte) 0x04);
  }
  if(s == "267"){
    Serial.println("hello,world!");
    sendKey((byte) 0x0B);
    sendKey((byte) 0x08);
    sendKey((byte) 0x0F);
    sendKey((byte) 0x0F);
    sendKey((byte) 0x12);
    sendKey((byte) 0x36);//,
    sendKey((byte) 0x1A);
    sendKey((byte) 0x12);
    sendKey((byte) 0x15);
    sendKey((byte) 0x0F);
    sendKey((byte) 0x07);
    sendKeyModifier((byte) 0x1E, (byte) 0x02);
  }
  if(s == "24"){
    Serial.println("esc");
    sendKey((byte) 0x29);
  }
  if(s == "21"){
    Serial.println("enter");
    sendKey((byte) 0x28);
  }
  seqIndex = 0;
}

boolean readMageSensor(int port, float limit){
  int mageValue = analogRead(port);
  float voltage = mageValue * (5.0 / 1023.0);
  //Serial.print(voltage);
  return voltage > limit;
}

void sendKeyModifier(byte key, byte modifier){
  sendKeyCode(key, modifier);
  delay(100);
  sendKeyCode((byte)0x00, (byte)0x00);
}

void sendKey(byte key){
  sendKeyModifier(key, (byte)0x00);
}
void sendKeyCode(byte key, byte modifier){
    btSerial.write(0xFD); // Raw Report Mode
    btSerial.write(0x09); // Length
    btSerial.write(0x01); // Descriptor 0x01=Keyboard

    btSerial.write(modifier);
    btSerial.write((byte)0x00);
    btSerial.write(key);
    btSerial.write((byte)0x00);
    btSerial.write((byte)0x00);
    btSerial.write((byte)0x00);
    btSerial.write((byte)0x00);
    btSerial.write((byte)0x00);
}

void ledFlash(){
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(10);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(10);              // wait for a second
}  
