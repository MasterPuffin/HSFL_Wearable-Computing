#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#include "src/0.h"
#include "src/1.h"


#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
}

int currentImage = -1;
int loudness = 0;

void loop() {
  int imageToDraw;
  if (loudness > 127) {
    imageToDraw = 1;
  } else if (loudness > 63) {
    imageToDraw = 1;
  } else {
    imageToDraw = 0;
  }

  if (imageToDraw != currentImage) {
    display.clearDisplay();
    switch (imageToDraw) {
      case 1:
        display.drawBitmap(0, 0, bitmap_0, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
        break;
      case 0:
        display.drawBitmap(0, 0, bitmap_0, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
        break;
    }
    display.display();
    currentImage = imageToDraw;
    delay(100);
  }
}
