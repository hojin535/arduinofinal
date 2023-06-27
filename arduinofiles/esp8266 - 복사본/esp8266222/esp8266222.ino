#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <time.h> 

String state="";
String preSky="";
String khaiGrade="";
String realTime="";
String settingTime="";
#define FIREBASE_HOST "aruino-51411-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "vh5vkyg52ndZNyzuVMV5WNj73BHGdIWung6gEVk9"
//#define WIFI_SSID "U+office_2.4G"
//#define WIFI_PASSWORD "officenet1"  
#define WIFI_SSID "U+NetE5E3"
#define WIFI_PASSWORD "4000004829"  
boolean first= true;
boolean updateData=true;
int timezone = 3; 
int dst = 0; 
 
unsigned long previousMillis = 0;
const long interval = 20000; 
unsigned long oneSeconds=0;
unsigned long fiveSeconds=0;
void setup() {
  Serial.begin(9600);
 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
//현재시간 받아올 서버+ 한국설정 
configTime(3 * 3600 * 3, 0, "kr.pool.ntp.org", "kr.pool.ntp.org"); 
   Serial.println("\nWaiting for time"); 
   while (!time(nullptr)) { 
     Serial.print("."); 
     delay(1000); 
   }
   Serial.println(""); 
}


void loop() {
  unsigned long currentMillis = millis();
   //현재시간 받아오기
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    time_t now = time(nullptr);
    Serial.print("realTime "); 
    Serial.print(String(ctime(&now)).substring(11,13)); 
    Serial.println(String(ctime(&now)).substring(14,16));
  }
  
  //1초마다 현재 창문상태 받아오기
  if(currentMillis-oneSeconds>=1000){
    oneSeconds=currentMillis;
  state=Firebase.getString("state");
  Serial.println("state "+state);
  }
  

    //5초마다 현재 환기시간 받아오기
  if(currentMillis-fiveSeconds>=5000){
    fiveSeconds=currentMillis;

  settingTime=Firebase.getString("settingTime");
  Serial.println("settingTime "+settingTime);

  }

  
   time_t now = time(nullptr);

  //해당 시간에 한번만 값을 받아오기 위해서 
    if(String(ctime(&now)).substring(14,16)=="45"){
    updateData=true;
  }
   // 예측날씨는 45분발표
  if(String(ctime(&now)).substring(14,16)=="47"){
   while(updateData){
    preSky=Firebase.getString("preSky");
  Serial.println("preSky "+preSky);

    if (Firebase.failed()) {
      Serial.print("setting failed:");
      Serial.println(Firebase.error());  
      return;
         }
    updateData=false;     
  }
  }

    //해당 시간에 한번만 값을 받아오기 위해서 
    if(String(ctime(&now)).substring(14,16)=="20"){
    updateData=true;
  }
   // 미세먼지는 20분쯤 업데이트
  if(String(ctime(&now)).substring(14,16)=="22"){
   while(updateData){

  khaiGrade=Firebase.getString("khaiGrade");
  Serial.println("khaiGrade "+khaiGrade);

    if (Firebase.failed()) {
      Serial.print("setting failed:");
      Serial.println(Firebase.error());  
      return;
         }
    updateData=false;     
  }
  }
  
 //처음 아두이노 연결 후 데이터 받아오기

while(first){

    state = Firebase.getString("state");
  preSky=Firebase.getString("preSky");
  khaiGrade=Firebase.getString("khaiGrade");
  realTime=Firebase.getString("realTime");
  settingTime=Firebase.getString("settingTime");

  Serial.println("state "+state);
  Serial.println("preSky "+preSky);
  Serial.println("khaiGrade "+khaiGrade);
  Serial.println("settingTime "+settingTime);
    if (Firebase.failed()) {
      Serial.print("setting failed:");
      Serial.println(Firebase.error());  
      return;
  }
  first=false;
}

 }
