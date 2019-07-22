#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
//#define FIREBASE_HOST "notifire-9c622.firebaseio.com"
//#define FIREBASE_AUTH "CurrPh0jzAuBSV0J5zcUma6wei0gszFgBpHE9F3Q"
#define FIREBASE_HOST "notifire-pkm19.firebaseio.com"
#define FIREBASE_AUTH "nXkM7UOze1aNpsloPvC7HzpUEw0DJ6sUSCmdojN6"
#define WIFI_SSID "Tai kucing"
#define WIFI_PASSWORD "12345678"

String nama = "alat/notifire_1";

void setup() {
  pinMode(D6, OUTPUT);
  
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
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  if(Firebase.getInt(nama+"/situasi")==1){
    digitalWrite(D6, HIGH);
    Serial.println(Firebase.getInt(nama+"/situasi"));
    delay(10);
  }else if(Firebase.getInt(nama+"/situasi")==2){
    digitalWrite(D6, LOW);
    Serial.println(Firebase.getInt(nama+"/situasi"));
    delay(10);
  }else if(Firebase.getInt(nama+"/situasi")==0){
    digitalWrite(D6, HIGH);
    Serial.println(Firebase.getInt(nama+"/situasi"));
    delay(10);
  }
}
