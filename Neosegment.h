// NeoSegment Library by Maksim Surguy: https://github.com/Neosegment/Arduino
// Modified to use FastLED library instead of NeoPixel libary by Jason Coon

/*
  Neosegment modules have LEDs in the following order (the number signifies index):
    55555
  4        6
  4        6
  4        6
    33333
  0        2
  0        2
  0        2
    11111
*/

const bool _numberArray [7][10] = {
  //       0  1  2  3  4  5  6  7  8  9
  /* 0 */ {1, 0, 1, 0, 0, 0, 1, 0, 1, 0},
  /* 1 */ {1, 0, 1, 1, 0, 1, 1, 0, 1, 1},
  /* 2 */ {1, 1, 0, 1, 1, 1, 1, 1, 1, 1},
  /* 3 */ {0, 0, 1, 1, 1, 1, 1, 0, 1, 1},
  /* 4 */ {1, 0, 0, 0, 1, 1, 1, 0, 1, 1},
  /* 5 */ {1, 0, 1, 1, 0, 1, 1, 1, 1, 1},
  /* 6 */ {1, 1, 1, 1, 1, 0, 0, 1, 1, 1},
};

const bool _letterArray [7][19] = {
  //       A  b  c  d  E  F  g  H  I  J, L, n, o, P, q, r, S, t, U,
  /* 0 */ {1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1},
  /* 1 */ {0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1},
  /* 2 */ {1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1},
  /* 3 */ {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0},
  /* 4 */ {1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1},
  /* 5 */ {1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0},
  /* 6 */ {1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1},
};

const String _letterIndexes = "abcdefghijlnopqrstu";

// Sets a digit with specified index to specified numerical value and an RGB color
void setDigit(uint16_t index, uint8_t number, CRGB color, CRGB* ledarray)
{
  if (number > 9)
    return;

  for (uint32_t segm = 7 * index; segm < 7 * index + 7; segm++) {
    if (_numberArray[segm % 7][number]) {
      ledarray[segm] = color;
    }
  }
}

// Sets a digit with specified index to specified numerical value and an HSV color
void setDigit(uint16_t index, uint8_t number, CHSV color, CRGB* ledarray)
{
  if (number > 9)
    return;

  for (uint32_t segm = 7 * index; segm < 7 * index + 7; segm++) {
    if (_numberArray[segm % 7][number]) {
      ledarray[segm] = color;
    }
  }
}

// Sets a digit with specified index to specified letter with an RGB color
void setLetter(int index, char letter, CRGB color, CRGB* ledarray)
{ 
  int letterIndex = _letterIndexes.indexOf(tolower(letter));
  for (int segm = 7 * index; segm < 7 * index + 7; segm++) {
    if (_letterArray[segm % 7][letterIndex]) {
      ledarray[segm] = color;
    }
  }
}

// Sets a digit with specified index to specified letter with an HSV color
void setLetter(int index, char letter, CHSV color, CRGB* ledarray)
{ 
  int letterIndex = _letterIndexes.indexOf(tolower(letter));
  for (int segm = 7 * index; segm < 7 * index + 7; segm++) {
    if (_letterArray[segm % 7][letterIndex]) {
      ledarray[segm] = color;
    }
  }
}
