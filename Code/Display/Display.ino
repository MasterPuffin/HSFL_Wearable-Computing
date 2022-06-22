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

#define BTN_PIN 15

#include "src/0.h"
#include "src/1.h"
#include "src/2.h"
#include "src/3.h"
#include "src/w.h"


#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16


int lastButtonState;    // the previous state of button
int currentButtonState; // the current state of button

int sound_digital = 0;
int sound_analog = 4;
int maxsteps = 500;
int steps = maxsteps;
int avgval = 0;
int addedval = 0;


void setup() {
  Serial.begin(9600);

  pinMode(BTN_PIN, INPUT);
  currentButtonState = digitalRead(BTN_PIN);

  pinMode(sound_digital, INPUT);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
}

int currentImage = -1;
int loudness = 0;
bool draw = true;

void loop() {
  int imageToDraw = -1;

  //Sound detection
  int val_analog = analogRead(sound_analog);

  // calibrate by taking first 1000 values for envoronment - average
  if (steps > 0) {
    addedval = addedval + val_analog;
    steps = steps - 1;

    // calculate average after getting values
    if (steps == 0) {
      avgval = int(addedval / maxsteps);
    }
    delay(10);
    Serial.println("Wait, calibraiting");
    display.clearDisplay();
    display.drawBitmap(0, 0, bitmap_w, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
    display.display();
    return;
  }
  Serial.println(val_analog);
  // check if value is louder than environment + correction value
  if (val_analog > int(avgval * 1.14))
  {
    Serial.println("Stufe 2");
    imageToDraw = 3;
  }
  if (val_analog > int(avgval * 1.12))
  {
    Serial.println("Stufe 1");
    imageToDraw = 2;
  }
  else if (val_analog > int(avgval * 1.1))
  {
    Serial.println("Stufe 0");
    imageToDraw = 1;
  }

  // else don't show something
  else
  {
    Serial.println("Stufe none");
    imageToDraw = 0;
  }

  //Image Drawing
  lastButtonState    = currentButtonState;      // save the last state
  currentButtonState = digitalRead(BTN_PIN); // read new state


  if (lastButtonState == HIGH && currentButtonState == LOW) {
    draw = !draw;
  }

  if (!draw) {
    imageToDraw = -1;
  }

  Serial.println(draw);
  Serial.println(imageToDraw);

  if (imageToDraw != currentImage) {
    display.clearDisplay();
    switch (imageToDraw) {
      case 3:
        Serial.println("Draw 1");
        display.drawBitmap(0, 0, bitmap_3, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
        break;
      case 2:
        Serial.println("Draw 1");
        display.drawBitmap(0, 0, bitmap_2, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
        break;
      case 1:
        Serial.println("Draw 1");
        display.drawBitmap(0, 0, bitmap_1, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
        break;
      case 0:
        Serial.println("Draw 0");
        display.drawBitmap(0, 0, bitmap_0, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
        break;
    }
    display.display();
    currentImage = imageToDraw;

  }
  delay(100);
}
