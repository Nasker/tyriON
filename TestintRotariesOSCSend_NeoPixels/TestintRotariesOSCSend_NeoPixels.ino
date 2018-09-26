// NeoPixelFunRandomChange
// This example will randomly select a number pixels and then
// start an animation to blend them from their current color to
// randomly selected a color
//

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include <RTPRotary.h>
#include <ArduinoOSC.h>

#define ROT_RIGHT_PIN1 GPIO_NUM_25
#define ROT_LEFT_PIN1 GPIO_NUM_26
#define ROT_RIGHT_PIN2 GPIO_NUM_27
#define ROT_LEFT_PIN2 GPIO_NUM_9
#define ROT_RIGHT_PIN3 GPIO_NUM_10
#define ROT_LEFT_PIN3 GPIO_NUM_13
#define PIXEL_PIN GPIO_NUM_5

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  10        /* Time ESP32 will go to sleep (in seconds) */

RTC_DATA_ATTR int bootCount = 0;


const uint16_t PixelCount = 6; // make sure to set this to the number of pixels in your strip
//const uint8_t PixelPin = GPIO_NUM_5;  // make sure to set this to the correct pin, ignored for Esp8266

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PIXEL_PIN);
// For Esp8266, the Pin is omitted and it uses GPIO3 due to DMA hardware use.
// There are other Esp8266 alternative methods that provide more pin options, but also have
// other side effects.
//NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount);
//
// NeoEsp8266Uart800KbpsMethod uses GPI02 instead

NeoPixelAnimator animations(PixelCount); // NeoPixel animation management object

// what is stored for state is specific to the need, in this case, the colors.
// Basically what ever you need inside the animation update function
struct MyAnimationState {
  RgbColor StartingColor;
  RgbColor EndingColor;
};

// one entry per pixel to match the animation timing manager
MyAnimationState animationState[PixelCount];

WiFiUDP udp;
ArduinoOSCWiFi osc;
const char* ssid = "NaisDelClot";
const char* pwd = "xmp13051985";
const IPAddress ip(192, 168, 1, 201);
const IPAddress gateway(192, 168, 1, 1);
const IPAddress subnet(255, 255, 255, 0);
const char* host = "192.168.1.131";
const int recv_port = 10000;
const int send_port = 12000;

RTPRotary rotaryClick1(1, ROT_RIGHT_PIN1, ROT_LEFT_PIN1);
RTPRotary rotaryClick2(2, ROT_RIGHT_PIN2, ROT_LEFT_PIN2);
RTPRotary rotaryClick3(3, ROT_RIGHT_PIN3, ROT_LEFT_PIN3);
//arguments => (ID, rightRotaryPin, leftRotaryPin)



void setup() {
  Serial.begin(115200);
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));
  print_wakeup_reason();
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) +
                 " Seconds");


  WiFi.disconnect(true);
  WiFi.begin(ssid, pwd);
  WiFi.config(ip, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  osc.begin(udp, recv_port);
  strip.Begin();
  strip.Show();

  SetRandomSeed();
}


void loop() {
  rotaryClick1.callbackOnRotation(actOnRotCallback);
  rotaryClick2.callbackOnRotation(actOnRotCallback);
  rotaryClick3.callbackOnRotation(actOnRotCallback);

  if (animations.IsAnimating()) {
    // the normal loop just needs these two to run the active animations
    animations.UpdateAnimations();
    strip.Show();
  }
  else {
    // no animations runnning, start some
    //
    PickRandom(0.2f); // 0.0 = black, 0.25 is normal, 0.5 is bright
  }
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

void actOnRotCallback(int ID, String callbackString, int newPosition) {
  Serial.print("Rotary with ID #");
  Serial.print(ID);
  Serial.print(" ");
  Serial.print(callbackString);
  Serial.print("\t-Position: ");
  Serial.println(newPosition);

  Serial.println("SENDING OSC MESSAGE!");
  OSCMessage msg;
  msg.beginMessage(host, send_port);
  switch (ID) {
    case 1:
      msg.setOSCAddress("/#1Rotary");
      break;
    case 2:
      msg.setOSCAddress("/#2rotary");
      break;
    case 3:
      msg.setOSCAddress("/#3rotary");
      break;
    default:
      msg.setOSCAddress("/?rotary");
      break;

  }
  msg.addArgString(callbackString);
  msg.addArgInt32(newPosition);
  osc.send(msg);
  if (newPosition >= 20) {
    for (int i = 0; i < 6; i++) strip.SetPixelColor(i,  HslColor(0.0, 0.0, 0.0));
    OSCMessage msgSleep;
    msgSleep.addArgString("GOING TO SLEEP! BRB IN 10 SECONDS!");
    osc.send(msgSleep);
    Serial.println("Going to sleep now");
    delay(2000);
    esp_deep_sleep_start();
  }

}

void SetRandomSeed() {
  uint32_t seed;

  // random works best with a seed that can use 31 bits
  // analogRead on a unconnected pin tends toward less than four bits
  seed = analogRead(0);
  delay(1);

  for (int shifts = 3; shifts < 31; shifts += 3) {
    seed ^= analogRead(0) << shifts;
    delay(1);
  }

  // Serial.println(seed);
  randomSeed(seed);
}

// simple blend function
void BlendAnimUpdate(const AnimationParam& param) {
  // this gets called for each animation on every time step
  // progress will start at 0.0 and end at 1.0
  // we use the blend function on the RgbColor to mix
  // color based on the progress given to us in the animation
  RgbColor updatedColor = RgbColor::LinearBlend(
                            animationState[param.index].StartingColor,
                            animationState[param.index].EndingColor,
                            param.progress);
  // apply the color to the strip
  strip.SetPixelColor(param.index, updatedColor);
}

void PickRandom(float luminance) {
  // pick random count of pixels to animate
  uint16_t count = random(PixelCount);
  while (count > 0) {
    // pick a random pixel
    uint16_t pixel = random(PixelCount);

    // pick random time and random color
    // we use HslColor object as it allows us to easily pick a color
    // with the same saturation and luminance
    uint16_t time = random(100, 400);
    animationState[pixel].StartingColor = strip.GetPixelColor(pixel);
    animationState[pixel].EndingColor = HslColor(random(360) / 360.0f, 1.0f, luminance);

    animations.StartAnimation(pixel, time, BlendAnimUpdate);

    count--;
  }
}
