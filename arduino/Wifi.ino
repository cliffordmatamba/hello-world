#ifdef ESP32
#include <WiFi.h>
#include <HTTPClient.h>
#else
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <MFRC522.h> //library for rfid sensor
#include <Wire.h> 
#endif

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
const char* ssid     = "juxby";
const char* password ="00000011";
const char* serverName = "http://ferroelectrical.co.za/.php";
String apiKeyValue = "tPmAT5Ab3j7F9";
String attendance  = "yes";


//define pins for rfid sensor
#define SS_PIN 10
#define RST_PIN 9


#include <SPI.h>
#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 
#define ONE_WIRE_BUS 4


MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.


const int numOfCards = 2;
byte cards[numOfCards][1] = {{0x69, 0x38, 0xFD, 0x6E},{0x29, 0xCE, 0xE2, 0x6E}}; // array of UIDs of rfid cards

int n = 0;//n is for the total number of students//j is for to detect the card is valid or not
int numCard[numOfCards]; //this array content the details of cards that already detect or not .
String names[numOfCards] = {"faffy mahaso","Amanda makumire"};//student names
long sNumbers[numOfCards] = {16242,16273};//student sNumbers

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  
  bool status = bme.begin(0x76);
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring or change I2C address!");
    while (1);
  }
SPI.begin();  // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card

}

void readRFID(){
  int j = -1;
  byte card_ID[4];//card UID size 4byte
  
  if ( ! mfrc522.PICC_IsNewCardPresent()) {//look for new card
    return;//got to start of loop if there is no card present
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {// Select one of the cards
    return;//if read card serial(0) returns 1, the uid struct contians the ID of the read card.
  }

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    card_ID[i] = mfrc522.uid.uidByte[i];
  }
  
  for (int i = 0; i < numOfCards; i++) {
    if (card_ID[0] == cards[i][0] && card_ID[1] == cards[i][1] && card_ID[2] == cards[i][2] && card_ID[3] == cards[i][3]) {
      j = i;
    }
  }
  
  if(j == -1) {//check the card validity
    invalid();
  }
  else if (numCard[j] == 1) { //to check if the card already detect
    alreadyRead(names[j], sNumbers[j], j);
  }
  else {
    //send data to display and save
    logCardData(names[j], sNumbers[j], j);
  }
  delay(1000);
}

void loop() {
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    
  
    http.begin(serverName);
    
    // content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    


    //HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&name=" + String(mfrc522.PICC_ReadCardSerial())         
                          + "&date=" + date + "&attendance=" + present
                          + "&temp=" + String(bme.readTemperature()) + "&fmask=" + n/a + "";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    
    // You can comment the httpRequestData variable above
    // then, use the httpRequestData variable below (for testing purposes without the  sensors)
    //String httpRequestData = "api_key=tPmAT5Ab3j7F9&name=clifford2%chari&date=06-07-21&attedance=present&temp=36.54&fmask=N/a";

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
     
    
http.addHeader("Content-Type", "type/text");
    //int httpResponseCode = http.POST("Covid testing results!");
    
    
        
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 30 seconds
  delay(30000);  
}














  
  




  

