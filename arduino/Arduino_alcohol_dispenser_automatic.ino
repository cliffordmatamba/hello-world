
#include <SD.h>                 
#define SD_ChipSelectPin 10     
#include <TMRpcm.h>      
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
#define I2C_ADDRESS 0x3C  // 0X3C+SA0 - 0x3C or 0x3D
#define RST_PIN -1  // Define proper RST_PIN if required.
SSD1306AsciiAvrI2c oled;

TMRpcm tmrpcm;            // create an object for music player

const int trigPin = 17;  //ultrasonic
const int echoPin = 16;  //ultrasonic
long duration;
int distance;
int step1_judge = 0;
#define pump 2

void setup(){
  //--------------------------------------- oled
  #if RST_PIN >= 0
    oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
  #else // RST_PIN >= 0
    oled.begin(&Adafruit128x64, I2C_ADDRESS);
  #endif // RST_PIN >= 0
  // Call oled.setI2cClock(frequency) to change from the default frequency.
    oled.setFont(Adafruit5x7);
  //-------------------------------------- oled
  
  tmrpcm.speakerPin = 9; //pin 9 for output audio
  Serial.begin(9600);
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("SD fail");
    oled.clear();
    oled.set1X();
    oled.println("SD card fail");
    return;   // don't do anything more if not
  }
  else{   
    Serial.println("SD ok");
    oled.clear();
    oled.println("SD card ok");
  }
  delay(1000);
  tmrpcm.volume(1);  //set volume for audio
  oled_print();
    
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input     
  pinMode(pump, OUTPUT); // for pump
}

void loop(){
  //------------reading distance
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH, 23529); //23529us for timeout 4.0m

  // Calculating the distance
  distance= duration*0.034/2;

  //speak_out(temp);
  Serial.print("distance is ");
  Serial.println(distance);
  if ((distance<14)&&(distance>0)) step1_judge++;
  else step1_judge=0;
  
  if (step1_judge>1){
    step1_judge=0;
    oled.clear();
    oled.set2X();
    oled.println("");
    oled.println("THANK YOU!");
  
    tmrpcm.play("en.wav");
    digitalWrite(pump, HIGH);
    delay(800);
    digitalWrite(pump, LOW);
    delay(10000); //wait for welcome voice complete
    
  }
  delay(300);
  oled_print();
}

void oled_print(){
  oled.clear();
  oled.set1X();
  oled.println("");
  oled.println("   Please keep hand");
  oled.println("");
  oled.set2X();
  oled.println("   CLEAN");
}
