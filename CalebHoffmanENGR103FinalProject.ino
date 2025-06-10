#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>

AsyncDelay full_Beat;
AsyncDelay half_Beat;

volatile bool RButtonFlag = 0;
volatile bool LButtonFlag = 0;
volatile bool switchFlag = 0;
bool errorFlag = 0;

int loopCount = 1;
int secondCount = 0;
float redSet;
float greenSet;
float blueSet;

int randomPicker = 0;
int playerScore = 0;
int mistakes = 5;

//left button interrupt function
void LeftButtonChecker() {
  LButtonFlag = 1;
}

//right button interrupt function
void RightButtonChecker() {
  RButtonFlag = 1;
}

//switch interrupt function
void pause() {
  switchFlag = !switchFlag;
}

void setup() {
  CircuitPlayground.begin();
  pinMode(4, INPUT_PULLDOWN);
  pinMode(5, INPUT_PULLDOWN);
  pinMode(7, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(4), LeftButtonChecker, RISING);
  attachInterrupt(digitalPinToInterrupt(5), RightButtonChecker, RISING);
  attachInterrupt(digitalPinToInterrupt(7), pause, CHANGE);

  Serial.begin(9600);


  while(!Serial);
  delay(1000);
  Serial.println("Ready?");
  delay(1000);
  Serial.println("3!");
  delay(500);
  Serial.println("2!");
  delay(500);
  Serial.println("1!");
  delay(500);
  Serial.println("Go!");
  delay(500);

//start timer loops
  full_Beat.start(500, AsyncDelay::MILLIS);
  half_Beat.start(250, AsyncDelay::MILLIS);

}

void loop() {

  //check if the game is paused
  if (switchFlag==1) {
    delay(5);
    //reset delays
    full_Beat.start(500, AsyncDelay::MILLIS);
    half_Beat.start(250, AsyncDelay::MILLIS);
    loopCount = 1;
  } else if (mistakes == 0 || playerScore == 20) {
    Serial.println("Game Over");
    Serial.print("Total Score:  ");
    Serial.println(playerScore);
    Serial.println("Thank you for playing. To restart, press button in the center of the Circuit Playground");
    mistakes = mistakes-1;
  } else if (mistakes < 0) {
    delay(1000);
  } else {

    //see if a new pattern needs to be generated
    if (loopCount == 1) {
      randomPicker = random(0, 3);
    }
    //find the associated pattern
    switch(randomPicker) { 
      case 0:
        LeftPattern1(255, 0, 255);
        break;
      case 1:
        RightPattern1(0, 100, 255);
        break;
      case 2:
        delay(random(1,3)*400);
        loopCount = 1;
        break;
/*    case 3:
        holdPattern(100, 200, 0);
        break;
*/
      default:
        break;
    }
  }

/*
  if (randomPicker != 2 && CircuitPlayground.mic.soundPressureLevel(10) > 80 && secondCount > 1) {
    for (int i = 0; i < 10; i ++) {
      CircuitPlayground.setPixelColor(i, 255, 0, 0);
    }
    loopCount = 1;
  }
*/

}

//Left button pattern - RGB inputs
void LeftPattern1 (int redSet, int greenSet, int blueSet) {
  if (full_Beat.isExpired()) {
    if (loopCount > 5) {
      if (LButtonFlag == 1) {
        //Correct signal
        LButtonFlag = 0;
        //turn pixels green
        for (int i = 0; i < 10; i++) {
          CircuitPlayground.setPixelColor(i, 0, 255, 0);
        }
        playerScore = playerScore+1;
        //award points in serial window
        Serial.print("Score:  ");
        Serial.println(playerScore);
        Serial.print("Errors Remaining:  ");
        Serial.println(mistakes);

      } else if (LButtonFlag == 0) {
        //incorrect signal
        //turn pixels red
        for (int i = 0; i < 10; i++) {
          CircuitPlayground.setPixelColor(i, 255, 0, 0);
        }
        mistakes = mistakes-1;
        //score update
        Serial.print("Score:  ");
        Serial.println(playerScore);
        Serial.print("Errors Remaining:  ");
        Serial.println(mistakes);
      }
      loopCount = 1;
    } else {
      if (LButtonFlag == 1) {
        //turn pixels red
        for (int i = 0; i < 10; i ++) {
          CircuitPlayground.setPixelColor(i, 255, 0, 0);
        }
        mistakes = mistakes-1;
        //score update
        Serial.print("Score:  ");
        Serial.println(playerScore);
        Serial.print("Errors Remaining:  ");
        Serial.println(mistakes);
      loopCount = 1;
      } else if (LButtonFlag == 0) {
        //shift lit pixels up one
        CircuitPlayground.clearPixels();
        CircuitPlayground.setPixelColor((4+loopCount), redSet, greenSet, blueSet);
        CircuitPlayground.setPixelColor((5-loopCount), redSet, greenSet, blueSet);
        loopCount = loopCount+1;
      }
    }
    secondCount = secondCount+1;
    full_Beat.repeat();
  }
}

void RightPattern1 (int redSet, int greenSet, int blueSet) {
  if (full_Beat.isExpired()) {
    if (loopCount > 5) {
      if (RButtonFlag == 1) {
        RButtonFlag = 0;
        for (int i = 0; i < 10; i++) {
          CircuitPlayground.setPixelColor(i, 0, 255, 0);
        }
        playerScore = playerScore+1;
        //award points in serial window
        Serial.print("Score:  ");
        Serial.println(playerScore);
        Serial.print("Errors Remaining:  ");
        Serial.println(mistakes);

      } else if (RButtonFlag == 0) {
        for (int i = 0; i < 10; i ++) {
          CircuitPlayground.setPixelColor(i, 255, 0, 0);
          }
        mistakes = mistakes-1;
        //score update
        Serial.print("Score:  ");
        Serial.println(playerScore);
        Serial.print("Errors Remaining:  ");
        Serial.println(mistakes);
      }
      loopCount = 1;

    } else {
      if (RButtonFlag == 1) {
        for (int i = 0; i < 10; i ++) {
          CircuitPlayground.setPixelColor(i, 255, 0, 0);
        }
        mistakes = mistakes-1;
        //score update
        Serial.print("Score:  ");
        Serial.println(playerScore);
        Serial.print("Errors Remaining:  ");
        Serial.println(mistakes);
      loopCount = 1;
      } else if (RButtonFlag == 0) {
        CircuitPlayground.clearPixels();
        CircuitPlayground.setPixelColor((4+loopCount), redSet, greenSet, blueSet);
        CircuitPlayground.setPixelColor((5-loopCount), redSet, greenSet, blueSet);
        loopCount = loopCount+1;
      }

    }
    secondCount = secondCount+1;
    full_Beat.repeat();
  }
}
 
/*
void holdPattern (int redSet, int greenSet, int blueSet) {
  if (full_Beat.isExpired()) {
    if (loopCount < 4) {
      CircuitPlayground.clearPixels();
      CircuitPlayground.setPixelColor((4+loopCount), redSet, greenSet, blueSet);
      CircuitPlayground.setPixelColor((5-loopCount), redSet, greenSet, blueSet);
      loopCount = loopCount+1;
    } else if (3 < loopCount <= 5) {
      if (CircuitPlayground.mic.soundPressureLevel(10) > 70) {
        CircuitPlayground.setPixelColor((4+loopCount), redSet, greenSet, blueSet);
        CircuitPlayground.setPixelColor((5-loopCount), redSet, greenSet, blueSet);
        loopCount = loopCount+1;
        
      } else {
        for (int i = 0; i < 10; i ++) {
          CircuitPlayground.setPixelColor(i, 255, 0, 0);
        }
        loopCount = 1;
        secondCount = 0;
      }
    } else if (loopCount > 5) {
      for (int i = 0; i < 10; i ++) {
        CircuitPlayground.setPixelColor(i, 0, 255, 0);
      }
      loopCount = 1;
      secondCount = 0;
    }


    full_Beat.repeat();
    Serial.println(CircuitPlayground.mic.soundPressureLevel(10));
  }
}
*/

