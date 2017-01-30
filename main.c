/**
 * Tinusaur MAX7219 Sample for flowing text on 8 LED 8x8 Matrixes in two rows.
 * 
 * Matrix indices:
 *  --- --- --- ---
 * | 0 | 1 | 2 | 3 |
 *  --- --- --- ---
 * | 4 | 5 | 6 | 7 |
 *  --- --- --- ---
 *  
 * Copyright (c) 2017 Asparuh Vitkinov. All Rights Reserved.
 * Distributed as open source software under MIT License, see LICENSE.txt file.
 * Please, as a favor, retain the link http://tinusaur.org to The Tinusaur Project.
 * 
 * @created 2017-01-21
 * @author Asparuh Vitkinov
 */
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

extern "C" {
  #include "max7219led8x8/max7219led8x8.h"
  #include "max7219led8x8/max7219led8x8f.h"
};

#define MAX7219_SEG_NUM (8) // Segments, number of 8x8 matrices + 1 extra
#define MAX7219_BUFFER_SIZE MAX7219_SEG_NUM * 8
#define MAX7219_SEG_ROWS 2
#define MAX7219_MATRIX_SIZE 8

uint8_t max7219_buffer[MAX7219_BUFFER_SIZE];

void max7219b_char(uint8_t x, uint8_t c) {
  for(uint8_t i = 0; i < MAX7219_MATRIX_SIZE; i++) {
    uint8_t charColumn = pgm_read_byte(&max7219led8x8_font[((c - 32) << 3) + i]);
    uint8_t topPart = convert(charColumn, 0, 4, MAX7219_SEG_ROWS);
    uint8_t bottomPart = convert(charColumn, 4, 8, MAX7219_SEG_ROWS);

    for (uint8_t column = 0; column < MAX7219_SEG_ROWS; column++) {
      max7219b_col(x * 4 - 1, topPart);
      max7219b_col(2 * x * 4 - 1, bottomPart);
      max7219b_out();
      max7219b_left();
      _delay_ms(10);
    }
  }
}

uint8_t convert(uint8_t number, int from, int to, int count) {
  int currentResultPosition = 0;
  uint8_t result = 0;
  for (int pos = from; pos < to; pos++) {
    for(int br = 0; br < count; br++) {
      if (((number >> pos) & 1) == 1) {
        result |= 1 << currentResultPosition;
      }
      currentResultPosition++;
    }
  }
  return result;
}

const char message[] = "Programmer's Club in Veliko Tarnovo  Tinusaur LED matrix control  ";

int main(void) {
  // ---- Initialization ----
  max7219b_init(max7219_buffer, MAX7219_BUFFER_SIZE);
  int length = sizeof(message)/sizeof(message[0]);
  
  // ---- Main Loop ----
  while(true) {
    for (int index = 0; index < length - 1; index++) {
      max7219b_char(MAX7219_SEG_NUM, message[index]);
    }
  }

  return 0;
}
