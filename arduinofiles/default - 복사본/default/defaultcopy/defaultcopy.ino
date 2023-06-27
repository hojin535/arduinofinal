#include <SoftwareSerial.h>
#include <TM1637Display.h>
#include <Stepper.h>
const int moveStep = 64;
Stepper stepper(moveStep, 8, 10, 9, 11);
// 빗물 감지센서 핀 번호
const int rainPin = A0;  
 // 빗물 감지 한계값
const int maxRainSensorValue = 500; 
 // "STOP", "OPEN", "CLOSE" 
String state = "STOP";
// "true", "false"
bool check = true; 
 // 시작 시간 저장 변수
unsigned long startMillis;
 // 동작 지속 시간 (5초)
const unsigned long duringTime = 5000;


//esp8266
#define BT_RXD 2
#define BT_TXD 3
//tm1637
#define CLK 4
#define DIO 5
//esp8266
 String now = "";
 String inState="";
String preSky="";
String khaiGrade="";
String realTime="";
String settingTime="";
SoftwareSerial ESP_wifi(BT_RXD, BT_TXD);
 boolean stringComplete = false; 
//tm1637
TM1637Display display(CLK, DIO);
//stopMotor 함수
void stopMotor() { 
  // state를 stop으로
  state = "STOP"; 
   // 센서를 중지
  stepper.step(0);
}
 // openMotor 함수
void openMotor() {
  // 센서를 시계방향으로 진행
  stepper.step(1); 
}
// closeMotor 함수
void closeMotor() { 
  // 센서를 반시계방향으로 진행
  stepper.step(-1); 
}
void setup()
{
  Serial.begin(9600);
  ESP_wifi.begin(9600);
  ESP_wifi.setTimeout(5000);
  delay(1000);
  // 디스플레이 밝기 설정 (0 ~ 7)
    display.setBrightness(7);
    // 모터 속도
      stepper.setSpeed(220); 
      // 시작 시간 초기화
  startMillis = millis(); 
  // 빗물 감지 센서 핀 입력 설정
  pinMode(rainPin, INPUT);  
}

void loop(){
  //esp8266에서 값 받아오기 
  if(ESP_wifi.available()){
        String inChar =ESP_wifi.readStringUntil('\n');
        now=inChar;
  
   if(now.startsWith("state")==1){
    if(now=="state OPEN"){
      state="OPEN";
    }else if(now=="state CLOSE"){
      state="CLOSE";
    }
//    state=now.substring(6,-1);
  }
  if(now.startsWith("preSky")==1){
    preSky=now.substring(7,-1);
  }
  if(now.startsWith("khaiGrade")==1){
    khaiGrade=now.substring(10,-1);
  }

  if(now.startsWith("settingTime")==1){
  settingTime=now.substring(12,-1);
  }
  }
  
if(settingTime!=""){
  int displayTime = settingTime.toInt();
    // 콜론(:) 표시 0b01000000
  display.showNumberDecEx(displayTime, 0b01000000, true);
// 현재 작동 시간
    unsigned long currentMillis = millis(); 
// 빗물 감지 센서 값 읽기
  int rainValue = analogRead(rainPin);  
// 빗물 감지를 한다면
  if (rainValue < maxRainSensorValue) { 
     // check 가 true 일때
    if (check==true) {
      // state를 close로 바꾸고
      state = "CLOSE"; 
      // check를 false로 바꾼다
      check = false; 
    }
     // 빗물 미감지시
  } else {
    // check를 true로 바꾼다
    check = true; 
  }
// state가 stop이 아니고 모터가 작동한 이후로부터 경과된 현재 작동 시간이 동작 지속 시간 보다 크거나 같을 때
  if (state!=""&&state != "STOP" && currentMillis - startMillis >= duringTime) { 
    stopMotor(); // stopMotor 함수 실행
  } else {
    if (state == "OPEN") { // state가 open 이면
      openMotor(); // openMotor 함수 실행
    } else if (state == "CLOSE") { // state가 close 이면
      closeMotor(); // closeMotor 함수 실행
    }
  }
}
}
