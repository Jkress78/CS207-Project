/*
 * Sezme.
 * 
 * A game for the Arduino Uno that tests your memory.
 * 
 * Sezme is a memory game in which the Arduino generates a series
 * of random LED flashes. The player must then press the buttons and
 * repeat the sequence of flashes in the correct order. If the player
 * guesses incorrectly a buzzer sounds, the correct sequence is shown 
 * and the game starts again. If the player guesses correctly, the 
 * difficulty level goes up.
 * 
 * The Keypad is connected to the Arduino through digital pins 2 through 7.
 * 
 * The LEDs are connected to the arduino through the shift register via the following pins. 
 * [1]->Q0
 * [2]->Q1
 * [3]->Q2
 * [4]->Q3
 * [5]->Q4
 * [6]->Q5
 * [7]->Q6
 * [8]->Q7
 * 
 * The Data pin of the shift register is connected to digital pin 10.
 * The Latch pin of the shift register is connected to digital pin 11.
 * The Clock pin of the shift register is connected to digital pin 12.
 * 
 * Each Button on the Keypad controlls the corrosponding LED in the line.
 * 
 * Author: Mario Gianota December 2019
 * Modified by James Kress April 2020
 * 
 */
#include <Keypad.h>

const byte ROWS = 3; // Four rows
const byte COLS = 3; // Three columns

int latchPin = 11;
int clockPin = 12;
int dataPin = 10;


byte leds = 0;

// Define the Keymap
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'}

};

// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 2, 3, 4};
// Connect keypad COL0, COL1 and COL2 to 
byte colPins[COLS] = { 5, 6, 7 }; 

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS ); // Initialize the keypad

const int PIEZO = 8;  // The pin the piezo is connected to

const int STATE_START_GAME = 0;  // Initial state
const int STATE_PICK_RND_SEQUENCE = 1;  // Pick a random sequence of LEDs
const int STATE_SHOW_RND_SEQUENCE = 2;  // Show the randomly selected sequence of LED flashes
const int STATE_READ_PLAYER_GUESS = 3;  // Read the player's guess
const int STATE_VERIFY_GUESS = 4;  // Check the guess against the random sequence
const int STATE_GUESS_CORRECT = 5;  // Player guessed correctly
const int STATE_GUESS_INCORRECT = 6;  // Player guessed incorrectly

const int MAX_DIFFICULTY_LEVEL = 13;  // Maximum difficulty level (LED flash sequence length)

// Array used to store the generated random sequence
int randomSequence[MAX_DIFFICULTY_LEVEL];

// Array used to store the player's guess
int playerGuess[MAX_DIFFICULTY_LEVEL];
// A counter to record the number of button presses made by the player
int numButtonPresses;

// The current state the game is in
int currentState;
int nextState;

// The difficulty level (1..MAX_DIFFICULTY_LEVEL)
// (Do not set to zero!)
int difficultyLevel;

void setup() 
{
  
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT); 
  pinMode(clockPin, OUTPUT);
 
  randomSeed(analogRead(0)); 
  
  // Set initial difficulty level to 1 random flash && put game in start state
  difficultyLevel = 1;
  currentState = STATE_START_GAME;
  nextState = currentState;
  numButtonPresses = 0;
}

void loop() 
{
  leds = 0;
  updateShiftRegister();

  // Run the state machine controlling the game's states
  switch( currentState ) {
    case STATE_START_GAME: 
      delay(2000); // Give player time to get ready
      nextState = STATE_PICK_RND_SEQUENCE; 
      break;
    case STATE_PICK_RND_SEQUENCE:
      generateRndSequence();
      nextState = STATE_SHOW_RND_SEQUENCE;
      break; 
    case STATE_SHOW_RND_SEQUENCE:
      showRndSequence();  // Display the rnd sequence to the player
      nextState = STATE_READ_PLAYER_GUESS;
      break;
    case STATE_READ_PLAYER_GUESS:
      readPlayerGuess();  // Poll buttons and record each button press
      // If all inputs have been made then verify the guess
      if( numButtonPresses >= difficultyLevel ) {
        numButtonPresses = 0;
        nextState = STATE_VERIFY_GUESS;
      }
      break;
    case STATE_VERIFY_GUESS:
      // Check player's guess against the generated random sequence
      if( verifyGuess() ) {
        nextState = STATE_GUESS_CORRECT;
      } else {
        nextState = STATE_GUESS_INCORRECT;
      }
      break;
    case STATE_GUESS_CORRECT:
      // Player was right. Increase difficulty level and goto start game
      soundCorrectGuess();
      difficultyLevel++;
      if( difficultyLevel > MAX_DIFFICULTY_LEVEL )
        difficultyLevel = MAX_DIFFICULTY_LEVEL;
      nextState = STATE_START_GAME;
      break;
    case STATE_GUESS_INCORRECT:
      // Player was wrong. Sound buzzer, show correct sequence, set difficulty level to 1 and re-start game
      soundBuzzer();
      showRndSequence();
      difficultyLevel = 1;
      nextState = STATE_START_GAME;
      break;
  }
  currentState = nextState;
}


// Generate a random sequence of LED pin numbers
void generateRndSequence() 
{
  for(int i=0; i<difficultyLevel; i++) 
  {
    randomSequence[i] = rndLEDPin();
    
  }
}

// Show random sequence
void showRndSequence() 
{
  for(int i=0; i<difficultyLevel; i++) 
  {
    bitSet(leds, randomSequence[i]);
    updateShiftRegister();
    
    playSoundForLED(randomSequence[i]);
      
    delay(500);
    bitClear(leds, randomSequence[i]);
    delay(500);
  }
}

// Read a button press representing a guess from player
void readPlayerGuess() 
{
  char key = kpd.getKey();
  if(key)  // Check for a valid key.
  {
    if(key == '1') 
    {
      playerGuess[numButtonPresses] = 0;
      numButtonPresses++;
      bitSet(leds, 0);
      updateShiftRegister();
      playSoundForLED(0);    
    }
  
    else if(key == '2') 
    {
      playerGuess[numButtonPresses] = 1;
      numButtonPresses++;
      bitSet(leds, 1);
      updateShiftRegister();
      playSoundForLED(1);
    }
  
    else if(key == '3')
    {
      playerGuess[numButtonPresses] = 2;
      numButtonPresses++;
      bitSet(leds, 2);
      updateShiftRegister();
      playSoundForLED(2);
    }
  
    else if(key == '4')
    {
      playerGuess[numButtonPresses] = 3;
      numButtonPresses++;
      bitSet(leds, 3);
      updateShiftRegister();
      playSoundForLED(3);
    }

    else if(key == '5')
    {
      playerGuess[numButtonPresses] = 4;
      numButtonPresses++;
      bitSet(leds, 4);
      updateShiftRegister();
      playSoundForLED(4);
    }

    else if(key == '6')
    {
      playerGuess[numButtonPresses] = 5;
      numButtonPresses++;
      bitSet(leds, 5);
      updateShiftRegister();
      playSoundForLED(5);
    }

    else if(key == '7')
    {
      playerGuess[numButtonPresses] = 6;
      numButtonPresses++;
      bitSet(leds, 6);
      updateShiftRegister();
      playSoundForLED(6);
    }

    else if(key == '8')
    {
      playerGuess[numButtonPresses] = 7;
      numButtonPresses++;
      bitSet(leds, 7);
      updateShiftRegister();
      playSoundForLED(7);
    }
  
  }
}


// Compare the guess with the random sequence and return true if identical
bool verifyGuess() {
  bool identical = true;
  for(int i=0; i<difficultyLevel; i++)
  {
    if( playerGuess[i] != randomSequence[i] ) 
    {
      identical = false;
      break;
    }
  }
  return identical;
}

// Sound the buzzer for incorrect guess
void soundBuzzer() {
  tone(PIEZO, 100, 2000);
  delay(2300);  
}

void soundCorrectGuess() {
  tone(PIEZO, 700, 100);
  delay(100);
  tone(PIEZO, 800, 100);
  delay(100);
  tone(PIEZO, 900, 100);
  delay(100);
  tone(PIEZO, 1000, 100);
  delay(100);
  tone(PIEZO, 1100, 100);
  delay(100);
  tone(PIEZO, 1200, 100);
  delay(100);
}


void playSoundForLED(int ledPinNum)
{
  int pitch = 0;
  switch(ledPinNum) 
  {
    case 0: pitch = 150; break;
    case 1: pitch = 200; break;
    case 2: pitch = 250; break;
    case 3: pitch = 300; break;
    case 4: pitch = 350; break;
    case 5: pitch = 400; break;
    case 6: pitch = 450; break;
    case 7: pitch = 500; break;
  
  }
  tone(PIEZO, pitch, 800);
}

// Get a random LED pin number
int rndLEDPin()
{
  return random(0, 8);
}

void updateShiftRegister()
{
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, MSBFIRST, leds); //LSBFIRST starts from the least significant Byte, that corresponds to 8th pinout
   delay(200);
   digitalWrite(latchPin, HIGH);
}
