#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL3FkG5FL9P"
#define BLYNK_TEMPLATE_NAME "Smart Parking 2"

#include<ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
//#include <SimpleTimer.h>


char auth[] = "3aJ3FLaetuzlb0EupOuBTA3roX-pa2N_";

// Your WiFi credentials.
// Set password to “” for open networks.
char ssid[] = "OPPO F21s Pro 5G";
char pass[] = "a3t42xwy";
//BlynkTimer timer;
SimpleTimer timer;

String myString; // complete message from arduino, which consistors of snesors data
char rdata; // received charactors

int firstVal, secondVal; // sensors
int led1,led2,led3,led4;
// This function sends Arduino’s up time every second to Virtual Pin (1).
// In the app, Widget’s reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
// You can send any value at any time.
// Please don’t send more that 10 values per second.
Blynk.virtualWrite(V1, millis() / 1000);

}

void setup()
{
// Debug console
Serial.begin(9600);

Blynk.begin(auth, ssid, pass);

timer.setInterval(1000L,sensorvalue1);
timer.setInterval(1000L,sensorvalue2);
timer.setInterval(1000L,sensorvalue3);
timer.setInterval(1000L,sensorvalue4);


}

void loop()
{
if (Serial.available() == 0 )
{
Blynk.run();
timer.run(); // Initiates BlynkTimer
}

if (Serial.available() > 0 )
{
rdata = Serial.read();
myString = myString+ rdata;
// Serial.print(rdata);
if( rdata == '\n')
{
Serial.println("This is from nodemcu: ");
Serial.println(myString);
// Serial.println(“fahad”);
// new code
String l = getValue(myString, ',', 0);
String m = getValue(myString, ',', 1);
String n = getValue(myString, ',', 2);
String o = getValue(myString, ',', 3);

// these leds represents the leds used in Blynk application
led1 = l.toInt();
led2 = m.toInt();
led3 = n.toInt();
led4 = o.toInt();


myString = "";
// end new code
}
}

}

void sensorvalue1()
{
int sdata = led1;
// You can send any value at any time.
// Please don’t send more that 10 values per second.
Blynk.virtualWrite(V10, sdata);

}
void sensorvalue2()
{
int sdata = led2;
// You can send any value at any time.
// Please don’t send more that 10 values per second.
Blynk.virtualWrite(V11, sdata);

}

void sensorvalue3()
{
int sdata = led3;
// You can send any value at any time.
// Please don’t send more that 10 values per second.
Blynk.virtualWrite(V12, sdata);

}

void sensorvalue4()
{
int sdata = led4;
// You can send any value at any time.
// Please don’t send more that 10 values per second.
Blynk.virtualWrite(V13, sdata);

}


String getValue(String data, char separator, int index)
{
int found = 0;
int strIndex[] = { 0, -1 };
int maxIndex = data.length() -1;

for (int i = 0; i <= maxIndex && found <= index; i++) {
if (data.charAt(i) == separator || i == maxIndex) {
found++;
strIndex[0] = strIndex[1] + 1;
strIndex[1] = (i == maxIndex) ? i+1 : i;
}
}
return found > index ? data.substring(strIndex[0], 
strIndex[1]) : "";
}
