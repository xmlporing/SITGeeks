#include <Adafruit_NeoPixel.h>

/*
 * This programme is for the opening of SIT Carnival 2016 by SIT Geeks running alongside with runnable, addressable LED using Adafruit_NeoPixel library.
 * All LED strips start from the bottom of the letter, thus the addressing always start from bottom box.
 */

/*
 * LED strips data pin and number definition
 */

// Button input pin
#define SWITCH_IN 3

// number of LED per box
#define LED_PER_BOX 2

// LED data pin & number of LED per strip
#define NUM_LED_P 3 * LED_PER_BOX
#define PIN_P 2
#define NUM_LED_S 11 * LED_PER_BOX
#define PIN_S 5
#define NUM_LED_I 4 * LED_PER_BOX
#define PIN_I 6
#define NUM_LED_T 6 * LED_PER_BOX
#define PIN_T 10
#define TOTAL_LED NUM_LED_S + NUM_LED_I + NUM_LED_T

/*
 * Timing definition
 */

// Delay Timing
#define BUTTONPRESSDELAY 2000
#define COUNTDOWNDELAY 1000
#define CLEARFILL 75
#define PLAYSPEED 1000 //1000 = 1 sec
#define SNAKEFILLDELAY 333
#define SPEEDFILLDELAY 120
#define RANDOMFILLDELAY 200
#define SPREADFILLDELAY 175
#define BLINKDELAY 350
#define BLINKDELAYFAST 100
#define PACMANDELAY 800
#define WHITEWHOLEREDDOTDELAY (180 * 1000L) // 3min

/*
 * LED strips
 * pixelList, 0 is Podium, 1 is S, 2 is I, 3 is T
 * numOfLED to access each Letter strips
 * NUMOFSTRIPS define how many addressable strips there are
 */
Adafruit_NeoPixel pixelList[] = {
   Adafruit_NeoPixel(NUM_LED_P, PIN_P, NEO_RGB + NEO_KHZ800),
   Adafruit_NeoPixel(NUM_LED_S, PIN_S, NEO_RGB + NEO_KHZ800),
   Adafruit_NeoPixel(NUM_LED_I, PIN_I, NEO_RGB + NEO_KHZ800),
   Adafruit_NeoPixel(NUM_LED_T, PIN_T, NEO_RGB + NEO_KHZ800)
};

uint8_t numOfLED [] = {
  NUM_LED_P, NUM_LED_S, NUM_LED_I, NUM_LED_T
};

#define PODIUM_PIXELLIST 0
#define S_PIXELLIST 1
#define I_PIXELLIST 2
#define T_PIXELLIST 3

#define NUMOFSTRIPS (sizeof(pixelList)/sizeof(pixelList[0]))

/*
 * Dimension definition
 */

// Dimension for each Letter
#define ROW_PER_LETTER 5
#define COL_PER_LETTER 3

// Dimension for whole flow
#define MAX_COL COL_PER_LETTER * NUMOFSTRIPS
#define MAX_ROW ROW_PER_LETTER

// ROW DATA, left to right
#define ROW0_P { -1, -1, -1 }
#define ROW1_P { -1, 0 * LED_PER_BOX, -1 }
#define ROW2_P { -1, 1 * LED_PER_BOX, -1 }
#define ROW3_P { -1, 2 * LED_PER_BOX, -1 }
#define ROW4_P { -1, -1, -1 }
#define ROW0_S { 0 * LED_PER_BOX, 1 * LED_PER_BOX, 2 * LED_PER_BOX }
#define ROW1_S { -1, -1, 3 * LED_PER_BOX }
#define ROW2_S { 6 * LED_PER_BOX, 5 * LED_PER_BOX, 4 * LED_PER_BOX }
#define ROW3_S { 7 * LED_PER_BOX, -1, -1 }
#define ROW4_S { 8 * LED_PER_BOX, 9 * LED_PER_BOX, 10 * LED_PER_BOX }
#define ROW0_I { -1, 0 * LED_PER_BOX, -1 }
#define ROW1_I { -1, 1 * LED_PER_BOX, -1 }
#define ROW2_I { -1, 2 * LED_PER_BOX, -1 }
#define ROW3_I { -1, -1, -1 }
#define ROW4_I { -1, 3 * LED_PER_BOX, -1 }
#define ROW0_T { -1, 0 * LED_PER_BOX, -1 }
#define ROW1_T { -1, 1 * LED_PER_BOX, -1 }
#define ROW2_T { -1, 2 * LED_PER_BOX, -1 }
#define ROW3_T { -1, 3 * LED_PER_BOX, -1 }
#define ROW4_T { 5 * LED_PER_BOX, 4 * LED_PER_BOX, -1 }

// Hold array of each individual box
int rowArray[ROW_PER_LETTER][NUMOFSTRIPS][COL_PER_LETTER] = { 
    {ROW0_P, ROW0_S, ROW0_I, ROW0_T},
    {ROW1_P, ROW1_S, ROW1_I, ROW1_T},
    {ROW2_P, ROW2_S, ROW2_I, ROW2_T},
    {ROW3_P, ROW3_S, ROW3_I, ROW3_T},
    {ROW4_P, ROW4_S, ROW4_I, ROW4_T}
};

// Centre of whole thing
#define CENTER_COL 7
#define CENTER_ROW 4

/*
 * Color definition
 */

#define MIN_COLOUR 365

// Predefined colour
uint32_t colour_black, colour_white, colour_red, colour_yellow, colour_blue;

// Arduino init
void setup() {
  // Button setup
  pinMode(SWITCH_IN, INPUT);
  digitalWrite(SWITCH_IN, HIGH);
  
  // init colour
  colour_black = pixelList[PODIUM_PIXELLIST].Color(0, 0, 0);
  colour_red = pixelList[PODIUM_PIXELLIST].Color(255, 0, 0);
  colour_white = pixelList[PODIUM_PIXELLIST].Color(240, 220, 240);
  colour_yellow = pixelList[PODIUM_PIXELLIST].Color(255, 0, 228);
  colour_blue = pixelList[PODIUM_PIXELLIST].Color(0, 255, 0);

  // init LED strips at each letters
  initLED();
  // seed random
  randomSeed(analogRead(0));
}

void loop() {
  countdown();
}

void initLED(){
  /*
   * This function do initialisation of all LED strips
   * Input: None
   * Output: None
   */
  uint8_t i;
  //init all strips
  for (i = 0; i < NUMOFSTRIPS; i++){
    pixelList[i].begin();
    pixelList[i].show(); // Initialize all pixels to 'off'
  }
}

void countdown(){
  /*
   * This function do the choreography of whole performance
   * Start based on button pressed
   * Input: None
   * Output: None
   */
  int i;
  int pressed = 0;
  // read button pressed
  if (digitalRead(SWITCH_IN) == LOW){ 
    _fillLetterSolid(PODIUM_PIXELLIST, colour_red);
    delay(BUTTONPRESSDELAY);
    for(i = NUM_LED_P - 1; i > 0; i-=2){
      // P
      pixelList[PODIUM_PIXELLIST].setPixelColor(i, colour_white);
      pixelList[PODIUM_PIXELLIST].setPixelColor(i - 1, colour_white);
      pixelList[PODIUM_PIXELLIST].show();
      delay(COUNTDOWNDELAY);
    }
    _fillLetterReset(PODIUM_PIXELLIST);
    pressed = 1;
  }
  while (pressed){
    //Start of SIT
    //random
    randomFill(13);
    
    //OPM
    blinkFill(colour_red);
    //spread
    solidFill(colour_white);
    delay(SPREADFILLDELAY);
    spreadFill(colour_red);

    //random
    randomFill(15);
    
    //PACMAN
    blinkFill(colour_white);
    pacManFill();
    
    //Not done
    delay(PLAYSPEED);
    rowFill();
    delay(PLAYSPEED);
    snakeFill();
    delay(PLAYSPEED);
    randomFill(12);
    delay(PLAYSPEED);
    speedFill();
    delay(PLAYSPEED);
    clearFill();

    //all letter white and dot on i red
    solidFill(colour_white);
    _fillPixelRowCol(CENTER_ROW, CENTER_COL, colour_red);
    delay(WHITEWHOLEREDDOTDELAY);
  }
}

/*
 * High level rendering function, to create display
 */
void clearFill(){
  /*
   * This function do clear filling
   * Input: None
   * Output: None
   */
  // init
  uint8_t currentLetter;
  //start from 0 to include P
  for (currentLetter = 0; currentLetter < NUMOFSTRIPS; currentLetter++){
    _fillLetterReset(currentLetter);
  }
  // added delay to prevent colour not updating
  delay(CLEARFILL);
}

void solidFill(const uint32_t & color){
  /*
   * This function do solid filling
   * Input: None
   * Output: None
   */
  // init
  uint8_t currentLetter;
  // clear the colour
  clearFill();
  //start from 1 to avoid P
  for (currentLetter = 1; currentLetter < NUMOFSTRIPS; currentLetter++){
    _fillLetterSolid(currentLetter, color);
  }
}

void rowFill(){
  /*
   * This function do row filling
   * Input: None
   * Output: None
   */
   // init
   uint8_t currentRow;
   // clear
   clearFill();
   // fill each row
   for (currentRow = 0; currentRow < ROW_PER_LETTER; currentRow++){
    _fillRow(currentRow, _genRandomColour(0));
    delay(PLAYSPEED);
   }
}

void snakeFill(){
  /*
   * This function do random snake like filling
   * From S(1 - 11) to I(15 - 12) to T(16 - 21A/B)
   * 1 = S, 2 = I, 3 = T
   * Input: None
   * Output: None
   */
   //init
   uint8_t currentLetter;
   uint8_t reverse = 0;
   //start from 1 to avoid P
   for (currentLetter = 1; currentLetter < NUMOFSTRIPS; currentLetter++){
    _fillLetterSeq(currentLetter, _genRandomColour(currentLetter), reverse, SNAKEFILLDELAY);
    //reverse order
    if (reverse)
      reverse = 0;
    else
      reverse = 1;
   }
}

void randomFill(uint8_t randomOccurence){
  /*
   * This function do random filling
   * Randomly lit up 1 to 4 pixel at a single time
   * Input: randomOccurence, number of times random pixel(s) lit up
   * Output: None
   */
   //init
   uint8_t count, letter, numberOfPixel, randPixel, i;
   
   //random
   for (count = 0; count < randomOccurence; count++){
    //numberOfPixel = (int)random(1, 5); //random max is exclusive
    // clear 
    clearFill();
    // check for number of pixel to be displayed
    //for (i = 0; i < numberOfPixel; i++){
    for (i = 0; i < 2; i++){
      letter = (int)random(1, NUMOFSTRIPS + 1);
      randPixel = (int)random(1, numOfLED[letter] - 1); //should be -2 for i max is 6,7, but exclusive
      _fillPixel(letter, randPixel, _genRandomColour(letter));
    }
    delay(RANDOMFILLDELAY);
   }
}

void speedFill(){
  /*
   * This function do speed filling, going up and down
   * Input: None
   * Output: None
   */
   //init
   uint8_t currentLetter;
   uint8_t reverse = 0;
   //clear
   clearFill();
   //start from 1 to avoid P
   for (currentLetter = 1; currentLetter < NUMOFSTRIPS; currentLetter++){
    // 0 is btm to top, 1 is top to btm
    _fillLetterSeq(currentLetter, _genRandomColour(currentLetter), 0, SPEEDFILLDELAY);
    _fillLetterSeq(currentLetter, _genRandomColour(currentLetter), 1, SPEEDFILLDELAY);
   }
}

void spreadFill(const uint32_t & color){
  /*
   * This function do spread filling, starting from pre-defined box with row and col
   * Input: color, color to be displayed as spreading
   * Output: None
   */
   //init
   int expansionRate = 0;
   //expand till unable to expand
   while (_fillCircle(CENTER_ROW, CENTER_COL, color, expansionRate)){
    expansionRate++;
    delay(SPREADFILLDELAY);
   }
}

void pacManFill(){
  /*
   * This function do Pacman like filling, pacman eating the dots while ghost chasing the pacman
   * Pacman will be colour_yellow, ghost will be colour_blue, empty will be colour_black
   * Input: None
   * Output: None
   */
   // init
   uint8_t currentLetter, j;
   int previous = -1 * LED_PER_BOX;
   int ghost = -2 * LED_PER_BOX;
   int previous_ghost = -3 * LED_PER_BOX;
   // clear fill
   clearFill();
   // fill with white
   solidFill(colour_white);
   // numbering based on total LED
   for (currentLetter = 0; currentLetter < TOTAL_LED; currentLetter+= LED_PER_BOX){
    // fill the pixel
    _fillPixelSingleReverse(currentLetter, colour_yellow);
    _fillPixelSingleReverse(previous, colour_black);
    _fillPixelSingleReverse(ghost, colour_red);
    _fillPixelSingleReverse(previous_ghost, colour_black);
    // increase the count
    previous += LED_PER_BOX;
    ghost += LED_PER_BOX;
    previous_ghost += LED_PER_BOX;
    // delay
    if (currentLetter > LED_PER_BOX)
      delay(PACMANDELAY);
   }
}

void blinkFill(const uint32_t & colour){
  /*
   * This function do blinking filling, slow and fast
   * Input: color, color to be displayed as blinking
   * Output: None
   */
  // clear
  clearFill();
  // fill colour
  solidFill(colour);
  delay(BLINKDELAY);
  clearFill();
  delay(BLINKDELAY);
  solidFill(colour);
  delay(BLINKDELAYFAST);
  // clear
  clearFill();
}

/*
 * Low level rendering function, to manipulate LED
 */
void _fillPixel(const uint8_t & choice, const uint8_t & pixel, const uint32_t & color){
   /*
   * This function do single pixel/box filling based on letter and pixel/box
   * Input: choice, which letter to be displayed; pixel, which box to be displayed; color, which color to be displayed
   * Output: None
   */
  // init
  uint8_t i;
  // Render the pixel
  for (i = 0; i < LED_PER_BOX; i++){
    pixelList[choice].setPixelColor(pixel + i, color);
  }
  pixelList[choice].show();
} 

void _fillPixelRowCol(const uint8_t & row, const uint8_t & col, const uint32_t & color){
  /*
   * This function do single pixel/box filling based on row and col
   * Input: row, which row to be displayed; col, which col to be displayed; color, which color to be displayed
   * Output: None
   */
  // init
  int letter = col / COL_PER_LETTER;
  int innerCol = col % COL_PER_LETTER;
  uint8_t i;
  //check if invalid lighting LED
  if (rowArray[row][letter][innerCol] != -1){
    for (i = 0; i < LED_PER_BOX; i++){
      pixelList[letter].setPixelColor(rowArray[row][letter][innerCol] + i, color);
    }
    pixelList[letter].show();
  }
}

void _fillPixelSingle(uint8_t number, const uint32_t & color){
  /*
   * This function do single pixel/box filling based on numbering sequence
   * Input: number, which number sequence to be displayed; color, which color to be displayed
   * Output: None
   */
  // init
  uint8_t i;
  // from number sequence, determine which letter
  for (i = 1; i < NUMOFSTRIPS; i++){
    if (number >= numOfLED[i])
      number -= numOfLED[i];
    else
      break;
  }
  // number if negative
  if (number < 0)
    return;
  // fill pixel based on found letter and adjusted number
  _fillPixel(i, number, color);
} 

void _fillPixelSingleReverse(uint8_t number, const uint32_t & color){
  /*
   * This function do single pixel/box filling based on numbering sequence
   * Input: number, which number sequence to be displayed; color, which color to be displayed
   * Output: None
   */
  // init
  uint8_t i;
  // from number sequence, determine which letter
  for (i = 1; i < NUMOFSTRIPS; i++){
    if (number >= numOfLED[i])
      number -= numOfLED[i];
    else
      break;
  }
  // number if negative
  if (number < 0)
    return;
  if (i == 2){
    // fill pixel based on found letter and adjusted number
    _fillPixel(i, numOfLED[i] - number - LED_PER_BOX, color);
  }else{
    // fill pixel based on found letter and adjusted number
    _fillPixel(i, number, color);
  }
} 
 
void _fillLetterSeq(const uint8_t & choice, const uint32_t & color, const uint8_t & reverse, const int & delayValue){
  /*
   * This function do letter sequence filling based on letter/choice
   * Input: choice, which letter to be displayed; color, which color to be displayed; reverse, if 1 reversed else normal seq; delayValue, how fast to fill it
   * Output: None
   */
  //init
  uint8_t i;
  uint8_t j;
  //loop to fill letter
  if (reverse){
    for (i = numOfLED[choice]; i > 0; i -= 2){
      _fillPixel(choice, i - LED_PER_BOX, color);
      delay(delayValue);
    }
  }else{
    for (i = 0; i < numOfLED[choice]; i += 2){
      _fillPixel(choice, i, color);
      delay(delayValue);
    }
  }
}

void _fillLetterSolid(const uint8_t & choice, const uint32_t & color){
  /*
   * This function do letter solid filling based on letter/choice
   * Input: choice, which letter to be displayed; color, which color to be displayed;
   * Output: None
   */
  // init
  uint8_t i;
  // loop to fill letter
  for (i = 0; i < numOfLED[choice]; i++){
    pixelList[choice].setPixelColor(i, color);
  }
  // show
  pixelList[choice].show();
}

void _fillLetterReset(const uint8_t & choice){
  /*
   * This function do letter reset filling based on letter/choice
   * Input: choice, which letter to be displayed;
   * Output: None
   */
   _fillLetterSolid(choice, colour_black);
}

void _fillRow(const uint8_t & row, const uint32_t & color){
  /*
   * This function do row filling based on row number
   * Input: row, which row to be displayed; color, what color to be displayed
   * Output: None
   */
  //init 
  uint8_t i, j;
  //loop to fill row
  for (i = 1; i < NUMOFSTRIPS; i++){
    for (j = 0; j < COL_PER_LETTER; j++){
      // check if need to be filled
      if (rowArray[row][i][j] == -1)
        continue;
      _fillPixel(i, rowArray[row][i][j], color);
    }
  }
}

int _fillCircle(const int & row, const int & col, const uint32_t & color, const int & expansionRate){
  /*
   * This function do square border filling based on center row and col
   * Input: row, which row to be displayed; col, which col to be displayed; color, what color to be displayed; expansionRate, the distance from center
   * Output: 1 if expandable, 0 if can't be expand
   */
  //init 
  int leftBound, rightBound, topBound, btmBound;
  int leftRender, rightRender, topRender, btmRender;
  uint8_t i;
  // all set as rendering
  leftRender = rightRender = topRender = btmRender = 1;
  /*
   * Check if out of bound
   */
  //left bound
  leftBound = col - expansionRate;
  if (leftBound < 0){
    leftBound = 0;
    leftRender = 0;
  }
  //right bound
  rightBound = col + expansionRate;
  if (rightBound > MAX_COL - 1){
    rightBound = MAX_COL - 1;
    rightRender = 0;
  }
  //btm bound
  btmBound = row - expansionRate;
  if (btmBound < 0){
    btmBound = 0;
    btmRender = 0;
  }
  //top bound
  topBound = row + expansionRate;
  if (topBound > MAX_ROW - 1){
    topBound = MAX_ROW - 1;
    topRender = 0;
  }
  /*
   * Draw circle based on checked bounds
   */
  //draw left, btm row to top row
  if (leftRender){
    for (i = btmBound; i <= topBound; i++){
      _fillPixelRowCol(i, leftBound, color);
    }
  }
  //draw right, btm row to top row
  if (rightRender){
    for (i = btmBound; i <= topBound; i++){
      _fillPixelRowCol(i, rightBound, color);
    }
  }
  //draw btm, left col to right col
  if (btmRender){
    for (i = leftBound; i <= rightBound; i++){
      _fillPixelRowCol(btmBound, i, color);
    }
  }
  //draw btm, left col to right col
  if (topRender){
    for (i = leftBound; i <= rightBound; i++){
      _fillPixelRowCol(topBound, i, color);
    }
  }
  // return if no more to expand
  return (leftRender) || (rightRender) || (btmRender) || (topRender);
}

uint32_t _genRandomColour(const uint8_t & choice){
  /*
   * This function generate random color
   * Input: choice, which letter color to be generated;
   * Output: None
   */
  int blue, green, red;
  int total = 0;
  blue = (int)random(0, 255);
  green = (int)random(0, 255);
  red = (int)random(0, 255);
  total = blue + green + red;
  if (total < MIN_COLOUR){
    // change one to be max colour
    int randomColor = (int)random(0,3);
    if (randomColor){
      if (randomColor == 2)
        red = 230;
      else
        green = 230;
    }else{
      blue = 220;
    }
  }
  return pixelList[choice].Color(blue, green, red);
}

