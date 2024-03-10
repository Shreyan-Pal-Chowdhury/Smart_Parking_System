#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.
LiquidCrystal_I2C lcd(0x27, 16, 2);

int readsuccess;
byte readcard[4];
char str[32] = "";
String StrUID;
String a[10];
int timer[10];
int len=0;
int print_flag=0;
void setup() {
  Serial.begin(9600); // Initialize serial communications with the PC
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Tax calculating..."); 
  delay(1000);
  lcd.clear();
  Serial.println("CLEARDATA");
  Serial.println("Scan PICC to see UID...");
  Serial.println("");
}
// --------------------------------------------------------------------
void loop() {
  readsuccess = getid();
 
  if(readsuccess){
    Serial.println( (String) "DATA,DATE,TIME," + StrUID );
    for(int i=0; i<len; i++){
      if(a[i]==StrUID){
       int out=millis()/1000;
        int in = timer[i];
        swap(i);
        len--;
        print_flag=1;
        Serial.println((String)"The time is: "+ "Out Time: "+ out+"  ,In Time: "+in+"= "+(out-in)+"The fair is:"+((out-in)*5));
        lcd.setCursor(0,0);
        lcd.print((String)"Your fair: "+((out-in)*5));
        lcd.setCursor(0,1);
        lcd.print("Please pay");
        delay(3000);
        lcd.clear();
      }
    }
    if(print_flag==0){
      a[len]=StrUID;
      timer[len]=millis()/1000;
      len++;
    }
    print_flag=0;
    for(int i=0; i<len; i++){
      Serial.println(a[i]+" "+timer[i]);
    }
    
  }

}
int swap(int i){
  for(int j=i; j<len; j++){
    a[j]=a[j+1];
    timer[j]=timer[j+1];
  }
}
// --------------------------------------------------------------------
int getid(){  
  if(!mfrc522.PICC_IsNewCardPresent()){
    return 0;
  }
  if(!mfrc522.PICC_ReadCardSerial()){
    return 0;
  }
 
  
  Serial.println("THE UID OF THE SCANNED CARD IS:");
  
  for(int i=0;i<4;i++){
    readcard[i]=mfrc522.uid.uidByte[i]; //storing the UID of the tag in readcard
    array_to_string(readcard, 4, str);
    StrUID = str;
  }
  mfrc522.PICC_HaltA();
  return 1;
}
// --------------------------------------------------------------------
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
