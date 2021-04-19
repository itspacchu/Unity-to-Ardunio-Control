#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

String readString;
int readIndex = 0;
String ba;
String br;
String ar;
String OldMsg;
int QuadParity = 0;

Servo BaseAng;
Servo BaseRot;
Servo ArmRot;//
LiquidCrystal_I2C lcd(0x27, 16, 2);

int baseang = 0;
int baserot = 0;
int armrot = 0;

void setup() {
  lcd.begin();
  lcd.backlight();
  lcd.print("IM ALIVVVVV");
  lcd.println("$:XX:XX:XX:$");
  BaseAng.attach(10);
  BaseRot.attach(6);
  ArmRot.attach(11);
  ResetAllServos();
  Serial.begin(9600);
}

void loop() { 
  if(Serial.available()){
    char buf = Serial.read();
    if(buf == ':'){
      QuadParity++;
      readIndex ++;
    }else{
      if((buf > 47 && buf < 59) || buf == 10 || buf == 32){
        switch(readIndex){
        case 1:
          ba += buf;
          break;
         case 2:
          br += buf;
          break;
         case 3:
          ar += buf;
          break;
         case 4:  
              
          baseang = ba.toInt();
          baserot = br.toInt();
          armrot = ar.toInt();
          if(QuadParity == 4 && ValidData(baseang) && ValidData(baserot) && ValidData(armrot)){
            Serial.println("🌾");
            UpdateServos();
            UpdateLCD();
            QuadParity = 0;
          }else{
            Serial.println("Quad parity Failed Corrupted Data");
            lcd.clear();
            ClearBuffers();
            QuadParity = 0;
            readIndex = 0;
            buf = ' ';
          }
          ClearBuffers();
          readIndex = 0;
          break;
        }
      }else{
        Serial.println((int)buf);
        Serial.println("Not a Valid Data");
        lcd.clear();
        Serial.flush();
      }
      
    } 
   }
 }



void UpdateServos(){
    BaseAng.write(baseang);
    BaseRot.write(baserot);
    ArmRot.write(armrot);
}

void UpdateLCD(){
  lcd.clear();
  lcd.print(baseang);
  lcd.print(":");
  lcd.print(baserot);
  lcd.print(":");
  lcd.print(armrot); 
}

void ClearBuffers(){
  ba = ' ';
  br = ' ';
  ar = ' ';
}


void ResetAllServos(){
  baseang = BaseAng.read();
  while(baseang > 10){
    baseang--;
    BaseAng.write(baseang);
  }
  delay(2);
  baserot = BaseRot.read();
  while(baserot > 10){
    baserot--;
    BaseRot.write(baserot);
  }
  delay(2);
  armrot = ArmRot.read();
  while(armrot > 10){
    armrot--;
    ArmRot.write(armrot);
  }
  delay(2);


}

//https://arduino.stackexchange.com/questions/1013/how-do-i-split-an-incoming-string
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void debug(){
    
  for(baseang = 0;baseang < 180;++baseang){
    BaseAng.write(baseang);
    delay(10);
    BaseRot.write(baseang);
    delay(10);
    ArmRot.write(baseang);
    delay(10);
  }

   for(baseang = 180;baseang > 0;baseang--){
    BaseAng.write(baseang);
    delay(10);
    BaseRot.write(baseang);
    delay(10);
    ArmRot.write(baseang);
    delay(10);
  }
}


bool ValidData(int val){
  if(val < 181 && val > -1){
    return true;
  }else{
    return false;
  }
}
