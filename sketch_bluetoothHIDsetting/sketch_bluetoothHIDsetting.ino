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
  //if (btSerial.available()){
  //  c = btSerial.read();
  //  Serial.write( c );
  //}
  char str[512];
  if(btSerial.available()){
    int i = 0;
    while(1){
      if(btSerial.available()){
        c = btSerial.read();
        str[i] = c;
        //Serial.print(str[i]);
        i++;
      }else{
        str[i] = '\0';
        break; 
      }
      delay(1);
    }
    Serial.print("str=");
    Serial.println(str);
  }
 
  // Read from internal serial port (with PC)
  if (Serial.available()){
    c = Serial.read();
 
    if(c == 's'){
        Serial.write("$$$\n");
        btSerial.print("$$$");
    }
    if(c == 'e'){
        Serial.write("-\n");
        btSerial.print("-");
        btSerial.print('\r');
    }
    if(c == 'm'){
//    SM,0    //スレーブモードにする
//    SM,6    //pairing modeにする
        Serial.write("SM,6\n");
        btSerial.print("SM,6");
        btSerial.print('\r');
    }
    if(c == 'p'){
//S~,6    //プロファイルをHIDに変更
        Serial.write("S~,6\n");
        btSerial.print("S~,6");
        btSerial.print('\r');
    }
    if(c == 'r'){
//R,1     //モジュールをリブート
        Serial.write("R,1\n");
        btSerial.print("R,1");
        btSerial.print('\r');
    }
    if(c == 'd'){
//current
        Serial.write("D\n");
        btSerial.print("D");
        btSerial.print('\r');
    }
    if(c == 'h'){
//help
        Serial.write("H\n");
        btSerial.print("H");
        btSerial.print('\r');
    }
    if(c == 'k'){
    //SH,0200 //Descriptor typeをKeyboardにする（デフォルトだから変更しなくてもよい）
        Serial.write("SH,0220\n");
        //btSerial.print("SH,0220");// mouse
        btSerial.print("SH,0200");// keyboard
        btSerial.print('\r');
    }
    if(c == 'b'){
        Serial.write("SU,9600\n");
        btSerial.print("SU,9600");
        btSerial.print('\r');
    }
    if(c == 'p'){
    //SR,5CF93899A91A
        Serial.write("SR,5CF93899A91A\n");
        btSerial.print("SR,001BDC056605");
//        btSerial.print("SR,5CF93899A91A");
        btSerial.print('\r');
    }
  }
  delay(10);
}
