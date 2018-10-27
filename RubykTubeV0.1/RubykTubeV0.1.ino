#include <NeoPixelBus.h>//#include <RTPPsyPixel.h">
#include <RTPRotary.h>
#include <RTPPeriodicBang.h>
#include <RTPBatteryControl.h>
#include <ArduinoOSC.h>
#include "PinsAndConstants.h"

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PIXEL_COUNT, PIXEL_PIN);
RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation, colorSaturation, colorSaturation);
RgbColor black(0);

WiFiUDP udp;
ArduinoOSCWiFi osc;

RTPRotary rotariesArray[N_ROTARIES] = {
  RTPRotary(0, ROT1_RIGHT_PIN, ROT1_LEFT_PIN),
  RTPRotary(1, ROT2_RIGHT_PIN, ROT2_LEFT_PIN),
  RTPRotary(2, ROT3_RIGHT_PIN, ROT3_LEFT_PIN)
};
//arguments => (ID, rightRotaryPin, leftRotaryPin)

RTPPeriodicBang iddleCounter(PERIOD_MILLIS, IDDLE_SECONDS);
RTPBatteryControl batteryControl(VCC_LEVEL_PIN, FIREBEETLE_ADC_LEVELS);
//RTPPsyPixel psyPixel(PIXEL_COUNT, PIXEL_PIN);

unsigned long int blinkPastMillis;
bool ledsOn = false;

void callback() {
  Serial.println("TOUCHÉ!");
}

void setup() {
  Serial.begin(115200);
  initialBatteryLevelDisplay();
  afterDeepSleepAPICalls();
  initWifiandOSC();
  for (int i = 0; i < N_ROTARIES; i++)
    rotariesArray[i].callbackOnRotation(initRotaries);
}

void loop() {
  for (int i = 0; i < N_ROTARIES; i++)
    rotariesArray[i].callbackOnRotation(actOnRotaryCallback);
  blinkLEDs();
  iddleCounter.callbackIddleCounter(actOnCounterTick);
  batteryControl.monitorBatteryCallbacks(actOnBatteryCallback);
}

void blinkLEDs() {
  if (blinkPastMillis + BLINK_LENGTH < millis() && ledsOn) {
    stripShowColor(black);//psyPixel.blackenStrip();//
    ledsOn = false;
  }
}

void initRotaries(int ID, String callbackString, int rotationDirection, int newPosition) {
  Serial.println("Rotaries init!");
}

void initialBatteryLevelDisplay() {
  HslColor batteryLevelColor(batteryControl.getHueLevel() / 360.0f, 1.0, 0.5);
  strip.Begin();//psyPixel.beginStrip();//
  stripShowColor(batteryLevelColor);
  delay(500);
}

void initWifiandOSC() {
  WiFi.disconnect(true);
  WiFi.onEvent(onWiFiEvent);
  WiFi.begin(ssid, pwd);
  WiFi.config(ip, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED && millis() < 10000) {
    delay(500);
    Serial.print(".");
    stripShowColor(white);//psyPixel.whiteStrip();//
    delay(500);
    Serial.print("-");
    stripShowColor(black);//psyPixel.blackenStrip();//
  }
  if (WiFi.status() == WL_CONNECTED)
    stripShowColor(green);//psyPixel.greenStrip();//
  else
    stripShowColor(blue);//psyPixel.blueStrip(); //

  delay(500);
  stripShowColor(black); //psyPixel.blackenStrip(); //

  osc.begin(udp, recv_port);
  osc.addCallback("/RubykTube", &actOnReceiveOSCCallback);
}

void afterDeepSleepAPICalls() {
  print_wakeup_reason();
}

void beforeDeepSleepAPICalls() {
  print_wakeup_reason();
  //touchAttachInterrupt(WAKEUP_TOUCH_PIN, callback, TOUCH_TRESHOLD);
  //esp_sleep_enable_touchpad_wakeup();   //Configure Touchpad as wakeup source
  //esp_sleep_enable_ext1_wakeup(GPIO_MASK, ESP_EXT1_WAKEUP_ALL_LOW);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_25, 0);
  Serial.println("Going to sleep now");
  delay(1000);
  esp_deep_sleep_start();//esp_light_sleep_start();

}

void stripShowColor(RgbColor color) {
  for (int i = 0; i < PIXEL_COUNT; i++) {
    strip.SetPixelColor(i, color);
  }
  strip.Show();
}

void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch (wakeup_reason) {
    case 1  : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case 2  : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case 3  : Serial.println("Wakeup caused by timer"); break;
    case 4  : Serial.println("Wakeup caused by touchpad"); break;
    case 5  : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.println("Wakeup was not caused by deep sleep"); break;
  }
}

void actOnReceiveOSCCallback(OSCMessage& m) {
  Serial.print(m.getOSCAddress());
  Serial.println(m.getArgAsString(0));
}

void actOnBatteryCallback(String callbackString, float vccLevel) {
  Serial.print(callbackString);
  Serial.print(" and level is ");
  Serial.print(vccLevel);
  Serial.println("V");

  Serial.println("SENDING BATTERY LEVEL OSC MESSAGE!");
  OSCMessage msg;
  msg.beginMessage(host, send_port);
  msg.setOSCAddress("/status");
  msg.addArgString(callbackString);
  msg.addArgFloat(vccLevel);
  osc.send(msg);
}

void actOnRotaryCallback(int ID, String callbackString, int rotationDirection, int newPosition) {
  stripShowColor(black);//psyPixel.blackenStrip();//
  stripShowColor(red);//psyPixel.psyColorStrip();//
  blinkPastMillis = millis();
  ledsOn = true;
  Serial.print("Rotary with ID #");
  Serial.print(ID);
  Serial.print(" ");
  Serial.print(callbackString);
  Serial.print("\t-Direction: ");
  Serial.print(rotationDirection);
  Serial.print("\t-Position: ");
  Serial.println(newPosition);

  Serial.println("SENDING ROTARIES OSC MESSAGE!");
  OSCMessage msg;
  msg.beginMessage(host, send_port);
  switch (ID) {
    case 0: msg.setOSCAddress("/state_1"); break;
    case 1: msg.setOSCAddress("/state_2"); break;
    case 2: msg.setOSCAddress("/state_3"); break;
  }
  msg.addArgInt32(rotationDirection);
  //msg.addArgInt32(newPosition);
  osc.send(msg);
  iddleCounter.resetIddleCounter();
}

void actOnCounterTick(String callbackString, int ticksLeft) {
  //stripShowColor(black);//psyPixel.blackenStrip();//
  if (callbackString == "TICK") {
    Serial.print(callbackString);
    Serial.print("s left: #");
    Serial.println(ticksLeft);
  }
  if (callbackString == "IDDLE EXPIRED") {
    OSCMessage msgSleep;
    msgSleep.beginMessage(host, send_port);
    msgSleep.setOSCAddress("/Status");
    msgSleep.addArgString("GOING TO SLEEP!");
    osc.send(msgSleep);
    beforeDeepSleepAPICalls();
  }
}

void onWiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case SYSTEM_EVENT_STA_START: {
        Serial.println("STA Started");
        break;
      }
    case SYSTEM_EVENT_STA_CONNECTED: {
        Serial.println("STA Connected");
        break;
      }
    case SYSTEM_EVENT_STA_GOT_IP: {
        Serial.print("STA IPv4: ");
        Serial.println(WiFi.localIP());
        break;
      }
    case SYSTEM_EVENT_STA_DISCONNECTED: {
        Serial.println("STA Lost Connection");
        Serial.println("Trying to connect back again!");
        initWifiandOSC();
        break;
      }
    case SYSTEM_EVENT_STA_STOP: {
        Serial.println("STA Stopped");
        Serial.println("Trying to connect back again!");
        initWifiandOSC();
        break;
      }
    default: {
        break;
      }
  }
}
