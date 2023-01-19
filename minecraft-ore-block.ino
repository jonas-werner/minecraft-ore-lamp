#include <Adafruit_NeoPixel.h>

#define BUTTON_PIN   2
#define PIXEL_PIN    9  // Use pull-up resistor
#define PIXEL_COUNT  4
#define WIPE_TIME    320 // in ms; overall time spent to wipe the full range of LEDs
                         // (should be multiple of PIXEL_COUNT)
#define LONGPRESS_OFF_TIME 1000 // in ms; the long-press time to turn LEDS off at any state;


Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

bool oldState = HIGH;
int showType = 0;
unsigned int longPressTime = 0;
unsigned int debounceDelay = 20;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {

  unsigned long startTime = millis();

  // Get current button state.
  bool newState = digitalRead(BUTTON_PIN);

  // Check if state changed from high to low (button press).
  if (newState == LOW && oldState == HIGH) {
    // Short delay to debounce button.
    delay(debounceDelay);
    // Check if button is still low after debounce.
    newState = digitalRead(BUTTON_PIN);
    if (newState == LOW) {
      showType++;
      if (showType > 5)
        showType = 0;
      startShow(showType, WIPE_TIME/PIXEL_COUNT);
    } 
  } else if (newState == LOW && oldState == LOW && longPressTime >= LONGPRESS_OFF_TIME) {
      showType = 0;
      startShow(showType, 0);
      longPressTime = 0;
  }

  // Calculate runtime of loop and add to long press time
  unsigned long stopTime = millis();
  updateLongPress(startTime, stopTime, newState);

  // Set the last button state to the old state.
  oldState = newState;
}

void startShow(int i, uint8_t wipeTime) {
  switch(i){
    case 0: colorWipe(strip.Color(0, 0, 0), wipeTime);    // Black/off
            break;
    case 1: colorWipe(strip.Color(255, 0, 0), wipeTime);  // Redstone
            break;
    case 2: colorWipe(strip.Color(0, 255, 0), wipeTime);  // Emerald
            break;
    case 3: colorWipe(strip.Color(0, 0, 255), wipeTime);  // Lapis Lazuli
            break;
    case 4: colorWipe(strip.Color(255, 100, 0), wipeTime);  // Gold
            break;
    case 5: colorWipe(strip.Color(100, 245, 228), wipeTime);  // Diamond
            break;            
  }
}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void updateLongPress(unsigned long startTime, unsigned long stopTime, bool newState) {
  if (newState == HIGH) {
    // Button went up, cancel longpress
    longPressTime = 0;
  } else {
    // Button down, add time we spent in loop to long press duration
    unsigned long diff;
    if (stopTime < startTime) { // overflow, happens after ~50d
      diff = 4294967295L - startTime + stopTime;
    } else {
      diff = stopTime - startTime;
    }
    longPressTime += diff;
  }
}
