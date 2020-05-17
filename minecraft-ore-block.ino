#include <Adafruit_NeoPixel.h>

#define BUTTON_PIN   2
#define PIXEL_PIN    9  // Use pull-up resistor
#define PIXEL_COUNT  4 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

bool oldState = HIGH;
int showType = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  // Get current button state.
  bool newState = digitalRead(BUTTON_PIN);

  // Check if state changed from high to low (button press).
  if (newState == LOW && oldState == HIGH) {
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.
    newState = digitalRead(BUTTON_PIN);
    if (newState == LOW) {
      showType++;
      if (showType > 5)
        showType=0;
      startShow(showType);
    }
  }

  // Set the last button state to the old state.
  oldState = newState;
}

void startShow(int i) {
  switch(i){
    case 0: colorWipe(strip.Color(0, 0, 0), 80);    // Black/off
            break;
    case 1: colorWipe(strip.Color(255, 0, 0), 80);  // Redstone
            break;
    case 2: colorWipe(strip.Color(0, 255, 0), 80);  // Emerald
            break;
    case 3: colorWipe(strip.Color(0, 0, 255), 80);  // Lapis Lazuli
            break;
    case 4: colorWipe(strip.Color(255, 100, 0), 80);  // Gold
            break;
    case 5: colorWipe(strip.Color(100, 245, 228), 80);  // Diamond
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


