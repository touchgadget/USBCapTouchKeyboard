/*
 * MIT License
 *
 * Copyright (c) 2020 touchgadgetdev@gmail.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

const size_t THRESHOLD_CAPTOUCH = 500;
typedef struct CAPTOUCH_t {
  int sensor;
  int threshold;
  uint16_t keycode;
} CAPTOUCH_t;

/*

View Teensy LC with USB connector on the left.

NS Button   |   |   |   |   |   |   |   |   |   |   |^  |>  |   |
Teensy Pins |5V |G  |3V |23 |22 |21 |20 |19 |18 |17 |16 |15 |14 |13
Teensy USB  |
Teensy Pins |G  |0  |1  |2  |3  |4  |5  |6  |7  |8  |9  |10 |11 |12
NS Button   |   |<  |V  |   |   |

*/

const size_t NUM_CAPTOUCH = 4;
CAPTOUCH_t TOUCH2KEY[NUM_CAPTOUCH] = {
  { 0, 1000, KEY_LEFT},
  { 1, 1000, KEY_DOWN},
  {15, 1000, KEY_UP},
  {16, 1000, KEY_RIGHT},
#if 0
  // Unused cap touch sensor pins
  { 3, 1000, },
  { 4, 1000, },
  {17, 1000, },
  {18, 1000, },
  {19, 1000, },
  {22, 1000, },
  {23, 1000, }
#endif
};

void setup() {
  /********************************************************
    Teensy 4.0: No Touch Sense

    Teensy 3.2/3.6
    Touch Sense pins: 0,1,15,16,17,18,19,22,23,25,32,33

    Teensy LC
    Touch Sense pins: 0,1,3,4,15,16,17,18,19,22,23
  ********************************************************/
#if defined(__MK64FX512__) || defined(__IMXRT1062__)
#error Capacitive touch not supported on this board.
#endif
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  for (size_t i = 0; i < NUM_CAPTOUCH; i++) {
    TOUCH2KEY[i].threshold = touchRead(TOUCH2KEY[i].sensor) + THRESHOLD_CAPTOUCH;
  }
}

void loop() {
  static size_t captouch = 0;

  CAPTOUCH_t *b = &TOUCH2KEY[captouch];
  if (touchRead(b->sensor) > b->threshold) {
    // User touch so press keyboard key
    digitalWrite(LED_BUILTIN, HIGH);
    Keyboard.press(b->keycode);
  }
  else {
    // User no touch so release keyboard key
    digitalWrite(LED_BUILTIN, LOW);
    Keyboard.release(b->keycode);
  }
  captouch++;
  if (captouch >= NUM_CAPTOUCH) captouch = 0;
}
