CS 207 Final Project  
 
 This project is a memory game built as a final project for my computer science 207 class. My design was based on a project done by Mario Gianota. The game uses a 595 shift register to help control the 8 LEDs used in the game without using up 8 of the digital pins. Additionally, there is a 4x4 Keypad that is used for the player to input their answers, although the Keypad is 4x4 I only wired in a 3x3 grid since wiring in all of the buttons would have been unnecessary considering only 8 buttons are required. There is also a Piezo buzzer that chimes every time a LED flashes and plays different chimes depending on if the players guess was correct or not. The bases of the game is that the Arduino generates a random sequence which is then displayed using the LEDs, once the sequence has been shown the player must repeat the sequence back using the keypad. If the player repeats the sequence correctly the buzzer plays a series of chimes the moves onto the next level increasing the number of LEDs shown in each sequence. If the player gets the sequence wrong the buzzer makes a buzzing sound then shows the sequence again and the resets the level back to 1.  
 
 
 
 
 
 
 
 
 
 
 
 
Repository Contents • /src - Software for the game. • /libraries-The required library for the game software. • /img – The images used in the readme. • /LICENSE – The license file. • /README – The file you are currently reading. 
 
Requirements and Materials 
Libraries: • Keypad.h -> https://playground.arduino.cc/Code/Keypad/ 
Materials: • 4x green LEDs • 4x red LEDs • 8x 560-ohm resistor • 1x Piezo Buzzer • 1x breadboard • 1x Arduino UNO • 1x 595 shift register • 1x Keypad 
 
Build Instructions 
 
 
 
 
 
 
 
 
 
Building this project can become quite messy with all of the wires on a small bread board. The best way to start is with placing the shift register, then connecting the LEDs in order. The proper configuration for the LEDs is as follows. 
(from left to right starting from the top row) • [1]->Q0 • [2]->Q1 • [3]->Q2 
• [4]->Q3 • [5]->Q4 • [6]->Q5 • [7]->Q6 • [8]->Q7 
Once the LEDs are connected properly add the voltage, ground, latch, clock, data wires to the appropriate pins. Wiring the buzzer is simple so it will be left out. For the Keypad you will want wires coming from pins 2-7 into the bread board. As shown in the above diagram the purple wires are the row pins and the green are the column pins. Notice there is a gap in the middle of the setup this is because not all of the rows are used and if that gap is not there the keypad will not work properly. 
 
Usage 
 Using this project is simple once you have the setup complete. All that a player has to do once the build is complete is upload the code and the game will start 10 seconds after the upload or once the power is connected if the code is already on the Arduino. 
 
Team • James Kress -> did everything Credits • Mario Gionata – original creator of this project before I modified it. 
 
