#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <MFRC522.h>
#include <WiFi101.h>
//   
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
Adafruit_SSD1306 display(128, 32, &Wire, -1);
MFRC522 mfrc522(7, 6);
WiFiClient client;
// your network SSID (name)
// and password (WPA)
char ssid[] = "juxby";        
char pass[] = "00000011";  
// name address 
char server[] = "www.ferroelectrical.co.za";    

int pinRelay=5;
int status = WL_IDLE_STATUS;  
char response[40];
byte cardUID[4] = {0,0,0,0};
int res=0;
int rr;
float temp;
unsigned long millis1=0;
unsigned long millis2=0;
unsigned long millis3=0;

void setup() {
  Serial.begin(9600);
  // relay
  pinMode(pinRelay,OUTPUT);
  digitalWrite(pinRelay,LOW);



  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(5000);
  }
  display.setCursor(30,20);
  display.print(F("ok"));
  display.display();
  printWiFiStatus();
  delay(2000);
  Serial.println("Wait RFID ....");
    
}

void loop() {
   

  switch(res) {
     case 0:  //
        send_display("Wait RFID...."); 
        res=1;
        break;
     case 1:  //  RFID
        //    
        if ( mfrc522.PICC_IsNewCardPresent()) {
           //    .
           if ( mfrc522.PICC_ReadCardSerial()) {   
              //  UID
              Serial.print(F("Card UID:")); 
              display.setCursor(10,20);
              for (byte i = 0; i < 4; i++) {
                 cardUID[i]=mfrc522.uid.uidByte[i];
                 Serial.print(cardUID[i],HEX);
                 display.print(cardUID[i],HEX);
              }
           Serial.println();
           res=2;
           display.display();
           delay(2000);
           Serial.println("send uid to server....");
           }
        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();
        }
        display.display();
        break;
     case 2:  //    UID  
        send_display("Send server..."); 
        rr=send_uid_to_server();
        if(rr==1) {
           res=3;
           display.setCursor(10,0);
           display.print(F("uid ok"));
           millis3=millis();
        }
        else if(rr==2) {
           res=0;
           display.setCursor(10,0);
           display.print(F("incorrect")); 
        }
        else  {
           res=0;
           display.setCursor(10,0);
           display.print(F("error 404")); 
        }
        display.display();
        delay(3000);
        break;
     case 3:  //   
        if(millis()-millis3<10000) {
           send_display("Wait temp....");
           temp=mlx.readObjectTempC();
           display.setCursor(10,20);
           display.print(temp);
           display.print(F(" *C"));
           display.display();
           if(temp>34.0 && temp<42.0) {
              res=4;
              delay(2000);
           }
        }
        else {
          res=0;
        }
        break;
     case 4:  //     
         send_display("Wait temp....");
         send_temp_to_server();
         res=5;
         delay(2000);
         break;
     
  }
}

//
void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
// 
int send_uid_to_server() {
   int ret=3;
   unsigned long previos;
   int x=0;int f=0;
   char s;
   client.stop();
   if (client.connect(server, 80)) {
     
       
     // uid  
     String str="/arduino/post.php?uid=";
     for (byte i = 0; i < 4; i++) {
        str+=String(cardUID[i],HEX);
     }
     Serial.print("str=");Serial.println(str);
     client.println("GET "+str+" HTTP/1.1");
     client.println("Host: ferroelectrical.co.za");
     client.println("User-Agent: ArduinoWiFi/1.1");
     client.println("Connection: close");
     client.println();
     //  
     previos=millis();
     for(int i=0;i<40;i++)
        response[i]=0; 
     
     do{
        if(client.available() > 0) { 
           //    
           s = client.read();
           if(s=='#') 
              f=1;
           if(f==1) {
              response[x]=s;
              //Serial.print(response[x]);
              x++;
           }
           Serial.write(s);
        }
     }
     while((millis() - previos) < 5000);
     if(response[0]='#') {
        ret=command(response);
     }
     Serial.println(response);
     //client.stop();
     return ret;
  } 
  else {
    //  
   Serial.println("connection failed");
   //client.stop();
   return 3;
  }
}
//
void send_temp_to_server() {
   client.stop();
   if (client.connect(server, 80)) {
    
     // uid  
     String str="/firm/get_temp.php?uid=";
     for (byte i = 0; i < 4; i++) {
        str+=String(cardUID[i],HEX);
     }
     str+="&temp="+String(temp);
     Serial.print("str=");Serial.println(str);
     client.println("GET "+str+" HTTP/1.1");
     client.println("Host: ferroelectrical.co.za");
     client.println("User-Agent: ArduinoWiFi/1.1");
     client.println("Connection: close");
     client.println();
     Serial.println(response);
     delay(10);
     //client.stop();
  } 
  else {
    //  
   Serial.println("connection failed");
   //client.stop();
  }
}
//     
int command(char* arr) {
    String str(arr);
    //
    if(str.indexOf("yes")!=-1) {
       return 1;
    }
    else if(str.indexOf("no")!=-1) {
       return 2;
    }
    return 3;
}


