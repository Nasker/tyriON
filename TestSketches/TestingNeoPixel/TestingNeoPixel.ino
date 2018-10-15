#include <NeoPixelBus.h>


#define PIXEL_PIN GPIO_NUM_5
#define PIXEL_COUNT 6
const uint16_t PixelCount = 6; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = PIXEL_PIN;  // make sure to set this to the correct pin, ignored for Esp8266

#define colorSaturation 128
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);


RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation);
RgbColor black(0);

HslColor hslRed(red);
HslColor hslGreen(green);
HslColor hslBlue(blue);
HslColor hslWhite(white);
HslColor hslBlack(black);


void setup(){
  Serial.begin(115200);
  while (!Serial); // wait for serial attach

  Serial.println();
  Serial.println("Initializing...");
  Serial.flush();

  // this resets all the neopixels to an off state
  strip.Begin();
  strip.Show();
  
  Serial.println();
  Serial.println("Running...");
}

void loop() {
  for (int i = 0; i < PIXEL_COUNT; i++){
    strip.SetPixelColor(i, red);
    strip.Show();
  }
  delay(100);
  for (int i = 0; i < PIXEL_COUNT; i++){
    strip.SetPixelColor(i, green);
    strip.Show();
  }
  delay(100);
  for (int i = 0; i < PIXEL_COUNT; i++){
    strip.SetPixelColor(i, blue);
    strip.Show();
  }
  delay(100);
  for (int i = 0; i < PIXEL_COUNT; i++){
    strip.SetPixelColor(i, white);
    strip.Show();
  }
  delay(100);
  for (int i = 0; i < PIXEL_COUNT; i++){
    strip.SetPixelColor(i, black);
    strip.Show();
  }
  delay(100);
}

