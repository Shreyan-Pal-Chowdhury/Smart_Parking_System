#include <Servo.h> //includes the servo library
#include <Wire.h> 
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd1(0x27, 20, 4);

Servo In_servo;
Servo Out_servo;

#define Entry_out 3
#define Entry_in 4
#define Exit_in 5
#define Exit_out 6

#define car_1 A0
#define car_2 2

int flag_entry = 0;
int flag_exit = 0;
int readsuccess;

String data[2] = {"71748330", "031FA2A7"};
String StrUID;
char str[32] = "";
byte readcard[4];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();
  pinMode(Entry_out, INPUT_PULLUP);
  pinMode(Entry_in, INPUT_PULLUP);
  pinMode(Exit_in, INPUT_PULLUP);
  pinMode(Exit_out, INPUT_PULLUP);

  pinMode(car_1, INPUT_PULLUP);
  pinMode(car_2, INPUT_PULLUP);

  In_servo.attach(7);
  Out_servo.attach(8);
  In_servo.write(180);
  Out_servo.write(180);

  lcd1.init();  
  lcd1.backlight();
  //lcd2.init();  
  //lcd2.backlight();
  lcd1.setCursor(0,1);
  lcd1.print("     WELCOME TO     ");
  lcd1.setCursor(0,2);
  lcd1.print("     Parking 2.0    ");
  delay(2000);
  lcd1.clear();
  lcd1.setCursor(0,1);
  lcd1.print("The New Horizon  ");
  lcd1.setCursor(0,2);
  lcd1.print("  of Parking  ");
  delay(2000);
  lcd1.clear();
  //lcd2.setCursor(0,1);
  //lcd2.print(" Tax displayed ");
}

void loop() {
  
  // put your main code here, to run repeatedly:
  int total = digitalRead(car_1) + digitalRead(car_2);
  Serial.println(total);
  Serial.println(digitalRead(Entry_out) + digitalRead(Entry_in) + digitalRead(Exit_in) + digitalRead(Exit_out));
  Serial.println(digitalRead(car_1));
  delay(500);
  lcd1.setCursor (0,0);
  lcd1.print("   Have Slot: "); 
  lcd1.print(total);
  lcd1.print("    "); 
  
  lcd1.setCursor(0,1);
  lcd1.print("Parking1:");
  
  lcd1.setCursor(9,1);
  lcd1.print("Parking2:");
  
  lcd1.setCursor (0,2);
  if(digitalRead(car_1)==0){lcd1.print("S1:Fill ");}
       else{lcd1.print("S1:Empty");}
  
  lcd1.setCursor (8,2);
  if(digitalRead(car_2)==0){lcd1.print("S2:Fill ");}
       else{lcd1.print("S2:Empty");}
  
  if (digitalRead(Entry_out) == 0 && getid()==1)
  {
    flag_entry = 1;
  }
  else if (digitalRead(Entry_in) == 0 && flag_entry == 1)
  {
    flag_entry = 0;
  }
  if (flag_entry == 1 && total >0){
    In_servo.write(90);
  }
  else{
    In_servo.write(180);
    flag_entry = 0;
  }
  if (digitalRead(Exit_in) == 0)
  {
    flag_exit = 1;
  }
  if (digitalRead(Exit_out) == 0 && flag_exit == 1)
  {
    flag_exit = 0;
  }
  if(flag_exit==1){
    Out_servo.write(90);
  }
  else{
    Out_servo.write(180);
  }
}

int getid(){  
  if(!mfrc522.PICC_IsNewCardPresent()){
    Serial.println("Faliure");
    return 0;
  }
  if(!mfrc522.PICC_ReadCardSerial()){
    Serial.println("Faliure");
    return 0;
  }
 
  
  Serial.println("THE UID OF THE SCANNED CARD IS:");
  
  for(int i=0;i<4;i++){
    readcard[i]=mfrc522.uid.uidByte[i]; //storing the UID of the tag in readcard
    array_to_string(readcard, 4, str);
    StrUID = str;
  }
  mfrc522.PICC_HaltA();
  for(int i=0;i<2;i++){
    if(StrUID == data[i]){
      Serial.println("Success");
      return 1;
    }
    else{
      Serial.println("Faliure, not present");
    }
  }
  Serial.println(StrUID);
}

void array_to_string(byte array[], unsigned int len, char buffer[])
{
    for (unsigned int i = 0; i < len; i++)
    {
        byte nib1 = (array[i] >> 4) & 0x0F;
        byte nib2 = (array[i] >> 0) & 0x0F;
        buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
        buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
    }
    buffer[len*2] = '\0';
}
