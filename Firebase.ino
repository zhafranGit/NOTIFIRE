#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
//#define FIREBASE_HOST "notifire-9c622.firebaseio.com"
//#define FIREBASE_AUTH "CurrPh0jzAuBSV0J5zcUma6wei0gszFgBpHE9F3Q"
#define FIREBASE_HOST "notifire-pkm19.firebaseio.com"
#define FIREBASE_AUTH "nXkM7UOze1aNpsloPvC7HzpUEw0DJ6sUSCmdojN6"
#define WIFI_SSID "Tai kucing"
#define WIFI_PASSWORD "12345678"

// inialisasi pin sensor dan alarm/buzzer
const int pinApi = D5;
const int pinAlarm = D6;
const int redLED = D7;
const int smokeA0 = A0;
// inialisasi variabel data
int data;
int dataAsap;
//Nilai threshold
int sensorThres = 330;

String nama = "alat/notifire_1";

void setup() {
  pinMode(pinApi, INPUT); // pin sebagai input
  pinMode(smokeA0, INPUT); // pin sebagai input
  pinMode(pinAlarm, OUTPUT); // pin sebagai output
  pinMode(redLED, OUTPUT);
  
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  tone(pinAlarm, 1000, 200);
  digitalWrite(redLED, HIGH);
       
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
int n = 0;
void loop() {
  if(Firebase.getInt(nama+"/power")==1){
    if(Firebase.getInt(nama+"/status")==0){
      Firebase.setInt(nama+"/situasi", 0);
      Firebase.setInt(nama+"/status", 0);
      Firebase.setString(nama+"/alamat", "");
      Firebase.setFloat(nama+"/lokasi/lat", 0.0);
      Firebase.setFloat(nama+"/lokasi/long", 0.0);
      delay(100);
    }else{
       data = digitalRead(pinApi);
       int analogSensor = analogRead(smokeA0);
      
       // jika data pada sensor API bernilai logic LOW
       if (data == LOW && Firebase.getInt(nama+"/power") == 1){
         // alarm dinyalakan
         digitalWrite(pinAlarm, HIGH);
         digitalWrite(redLED, HIGH);
         Firebase.setInt(nama+"/situasi", 2);
         Serial.println(nama+" => status : kebakaran ("+analogSensor+")");
         delay(100);
       }
      
       // jika data Analog pada sensor Asap lebih besar dari batas
       else if (analogSensor > sensorThres){
          digitalWrite(redLED, HIGH);
          Firebase.setInt(nama+"/situasi", 2);
          digitalWrite(pinAlarm, HIGH);
          Serial.println(nama+" => status : kebakaran ("+analogSensor+")");
          delay(100);
       }
      
       // jika data tidak bernilai logic LOW
       else {
          //Lampu Dimatikan
          digitalWrite(redLED, LOW);
          // alarm dimatikan
          digitalWrite(pinAlarm, LOW);
          Firebase.setInt(nama+"/situasi", 1);
          Serial.println(nama+" => status : aman ("+analogSensor+")");
          delay(100);
       }
    }
  }else{
    if(Firebase.getInt(nama+"/status")==0){
      Firebase.setInt(nama+"/power", 1);
      delay(100);
    }else{
      //Lampu Dimatikan
      digitalWrite(redLED, LOW);
      // alarm dimatikan
      digitalWrite(pinAlarm, LOW);
      Firebase.setInt(nama+"/situasi", 0);
      delay(100);
    }
  }
}
