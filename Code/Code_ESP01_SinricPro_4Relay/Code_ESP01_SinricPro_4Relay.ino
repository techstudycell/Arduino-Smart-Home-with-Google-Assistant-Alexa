 /**********************************************************************************
 *  TITLE: Multiple SinricPro Switch device for ESP01 (send data to Arduino through Serial Terminal)
 *  Click on the following links to learn more. 
 *  YouTube Video: https://youtu.be/YpddzFyWMHA
 *  Related Blog : https://iotcircuithub.com/arduino-iot-project-with-google-assistant-alexa/
 *  by Tech StudyCell
 *
 *  Download the libraries 
 *  SinricPro 2.9.14 Library:     https://github.com/sinricpro/esp8266-esp32-sdk
 *  ArduinoJson 6.19.1 Library:   https://github.com/bblanchon/ArduinoJson
 *  WebSockets 2.3.5 Library:     https://github.com/Links2004/arduinoWebSockets
 *  IRremote 3.5.2 Library:       https://github.com/Arduino-IRremote/Arduino-IRremote
 *  Arduino-timer 2.3.1 Library:  https://github.com/contrem/arduino-timer
 **********************************************************************************/

// Uncomment the following line to enable serial debug output
//#define ENABLE_DEBUG

#ifdef ENABLE_DEBUG
       #define DEBUG_ESP_PORT Serial
       #define NODEBUG_WEBSOCKETS
       #define NDEBUG
#endif 

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "SinricPro.h"
#include "SinricProSwitch.h"

#define WIFI_SSID         "WIFI_NAME"    
#define WIFI_PASS         "WIFI_PASSWORD"
#define APP_KEY           "APP_KEY"
#define APP_SECRET        "APP_SECRET"

#define SWITCH_ID_1       "YOUR-DEVICE-ID"    
#define SWITCH_ID_2       "YOUR-DEVICE-ID"    
#define SWITCH_ID_3       "YOUR-DEVICE-ID"    
#define SWITCH_ID_4       "YOUR-DEVICE-ID"    

#define BAUD_RATE         9600                // Change baudrate to your need

#define WIFI_LED       2    // WiFi LED

bool connectedFlag = false;
String pinStatus = "0000";
String pinStatus_OLD = "0000";

bool onPowerState1(const String &deviceId, bool &state) {
  //Serial.printf("Device 1 turned %s\r\n", state?"on":"off");
  Serial.printf("%s", state?"A1":"A0");  
  return true; // request handled properly
}

bool onPowerState2(const String &deviceId, bool &state) {
  //Serial.printf("Device 2 turned %s\r\n", state?"on":"off");
  Serial.printf("%s", state?"B1":"B0");
  return true; // request handled properly
}

bool onPowerState3(const String &deviceId, bool &state) {
  //Serial.printf("Device 3 turned %s\r\n", state?"on":"off");
  Serial.printf("%s", state?"C1":"C0");
  return true; // request handled properly
}

bool onPowerState4(const String &deviceId, bool &state) {
  //Serial.printf("Device 4 turned %s\r\n", state?"on":"off");
  Serial.printf("%s", state?"D1":"D0");
  return true; // request handled properly
}


// setup function for WiFi connection
void setupWiFi() {
  Serial.printf("\r\n[Wifi]: Connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(250);
  }

  Serial.printf("connected!\r\n[WiFi]: IP-Address is %s\r\n", WiFi.localIP().toString().c_str());
}

// setup function for SinricPro
void setupSinricPro() {
  // add devices and callbacks to SinricPro
  SinricProSwitch& mySwitch1 = SinricPro[SWITCH_ID_1];
  mySwitch1.onPowerState(onPowerState1);

  SinricProSwitch& mySwitch2 = SinricPro[SWITCH_ID_2];
  mySwitch2.onPowerState(onPowerState2);

  SinricProSwitch& mySwitch3 = SinricPro[SWITCH_ID_3];
  mySwitch3.onPowerState(onPowerState3);

  SinricProSwitch& mySwitch4 = SinricPro[SWITCH_ID_4];
  mySwitch4.onPowerState(onPowerState4);

  // setup SinricPro
  SinricPro.onConnected([](){ 
    //Serial.printf("SinricPro Connected\r\n");
    Serial.printf("W1");
    digitalWrite(WIFI_LED, LOW);
    }); 
  SinricPro.onDisconnected([](){ 
    //Serial.printf("SinricPro Disconnected\r\n");
    Serial.printf("W0");
    digitalWrite(WIFI_LED, HIGH);
    });
  SinricPro.begin(APP_KEY, APP_SECRET);
}

void updateState(String state, String pre_state){
  SinricProSwitch& mySwitch1 = SinricPro[SWITCH_ID_1];
  SinricProSwitch& mySwitch2 = SinricPro[SWITCH_ID_2];
  SinricProSwitch& mySwitch3 = SinricPro[SWITCH_ID_3];
  SinricProSwitch& mySwitch4 = SinricPro[SWITCH_ID_4];

  if(state.substring(0,1) != pre_state.substring(0,1)){
    if (state.substring(0,1) == "1"){
      mySwitch1.sendPowerStateEvent(HIGH);
    }
    else{
      mySwitch1.sendPowerStateEvent(LOW);
    }
  }
  if(state.substring(1,2) != pre_state.substring(1,2)){
    if (state.substring(1,2) == "1"){
      mySwitch2.sendPowerStateEvent(HIGH);
    }
    else{
      mySwitch2.sendPowerStateEvent(LOW);
    }
  }
  if(state.substring(2,3) != pre_state.substring(2,3)){
    if (state.substring(2,3) == "1"){
      mySwitch3.sendPowerStateEvent(HIGH);
    }
    else{
      mySwitch3.sendPowerStateEvent(LOW);
    }
  }
  if(state.substring(3,4) != pre_state.substring(3,4)){
    if (state.substring(3,4) == "1"){
      mySwitch4.sendPowerStateEvent(HIGH);
    }
    else{
      mySwitch4.sendPowerStateEvent(LOW);
    }
  }
}

void setup() {
  Serial.begin(BAUD_RATE); Serial.printf("\r\n\r\n");
  pinMode(WIFI_LED, OUTPUT);
  setupWiFi();
  setupSinricPro();
}

void loop() {
  while(Serial.available()) 
  {
    pinStatus = Serial.readString();
    if(pinStatus != pinStatus_OLD) {      
      updateState(pinStatus, pinStatus_OLD);
      pinStatus_OLD = pinStatus;
    }
    //connectedFlag = true;
  }
  SinricPro.handle();
}
