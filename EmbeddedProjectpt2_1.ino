#include <Servo.h>
/**************************Autonomous Snow Plow**********************************
 * This code is intended for an autonomous snow plow project. This project will 
 * simply prove the concept and IS NOT the full working sketch for a final 
 * demonstration of such a project. This will simply prove the concept for our 
 * intents and purposes here. What this code does is drives the robot forward until 
 * it runs accross a piece of shiny reflective duct tape. It will then proceed to
 * backup for 1 and a half seconds, alternate whether is turns right or turns left
 * depending on what it did the previous run, and then proceed to drive forward 
 * again until it crosses another piece of shiny reflective duct tape. Due to lack
 * of waterproofing our design, we did not actually run this in the snow so we do
 * not currently know if it will indeed push snow, but the power behind it does 
 * suggest that it is indeed possible with this design. Further testing will be
 * needed for full implementation of this project.
 ********************************************************************************/
/******************Initialize our Victors (motor controllers)********************/
/*
 * The alternative to using servo library is pluggin these into the analog ports of 
 * the Arduino. However, I find that they work better when plugged into the PWM bus.
 * If they are using the PWM bus they will need to be called servos. 
 */
Servo frontLeftVictor; // Victor Motor Controllers must be declared as servos for them to work properly.  
Servo frontRightVictor; // Victor Motor controllers must be declared as servos for them to work properly.
Servo backLeftVictor;
Servo backRightVictor; 
/******************Victor Pin Definitions****************************************/
#define backLeftVictorPin 6
#define backRightVictorPin 5 
#define frontLeftVictorPin 2
#define frontRightVictorPin 3

/******************Light Sensor Pin Definitions**********************************/
/*
 * Our light sensors will trip when we pass over shiny duct tape. This will act as our barrier
 * for proof of concept. If the full implementation of this project we will need an inductive
 * fence (or invisible dog fence) that will create a signal when the barrier has been passed. 
 * For our intents and purposes, however, this will suffice. 
 */
#define RS1 22 // This is to ensure that we know where we're plugging in our light sensors to find the line. 
#define RS2 24
#define LS1 26
#define LS2 28
/*****************Global Variables*************************************************/
int count = 0;
unsigned long timeNow = 0;
unsigned long howLongBack = 1500;
unsigned long howLongTurn = 2250; 


void setup() {
/**************Victor Setup*******************/
/* We need to use 4 Victors because we are using 4 motors. Each motor will get 
 *  Its own victor so we can control speed variable per motor. The alternative
 *  to this is putting two motors on one victor to create more space on the 
 *  board and less clutter in the code but that may not be ideal for callibration
 *  purposes.
 */
  frontLeftVictor.attach(frontLeftVictorPin); // attaches Victor 1 to pin 2 
  frontRightVictor.attach(frontRightVictorPin); // attaches Victor 2 to pin 3
  backLeftVictor.attach(backLeftVictorPin); // attaches Victor 3 to pin 6
  backRightVictor.attach(backRightVictorPin); // attaches Victor 4 to pin 5

/***************Light Sensor Setup************/
/*
 * Each light sensor will act as a digital input that will read LOW 
 * when passing over the duct tape and will have no other value in our tests
 * due to the adjustment of the sensors and the reflectivness of the tape. 
 */
  pinMode(RS1, INPUT);
  pinMode(RS2, INPUT);
  pinMode(LS1, INPUT);
  pinMode(LS2, INPUT);
/*
 * This is simply so we can run error diagnostics to see if certain functions are being called 
 * and things are executing correctly. 
 */
  Serial.begin(9600);
}
/*************BACKUP*****************************************/
/*
 * These values are not particularly fast but they will prevent slipping 
 * since the ground will be slick. We don't want speed anyways, we want torque.
 * Perhaps now is a good time to mention what type of motor we're using. We're 
 * using 4 CIM 12 V/ 27 A rated motors. Two go into each gear box. The gear box used 
 * is a Tough Box produced and distributed by AndyMark. These parts were provided
 * to us by the FIRST Robotics Competition Team 1566 in Idaho Falls, Idaho. 
 */
void backup(){
  Serial.print("backup was called\n");
  frontLeftVictor.write(85);
  backLeftVictor.write(85);
  frontRightVictor.write(100);
  backRightVictor.write(100);  
}
/************************Turn Right***************************/
/*
 * We want our left side going faster than our right side to turn 
 * in the right direction. To help with turning, we are having our
 * right side actually go backwards. The right side motors run a little
 * slow so we have to adjust the numbers accordingly for calibration. These
 * values will not necessarily be the ones used in other projects used in the
 * future. 
 */
void turnRight() {
   Serial.print("turnRight was called\n");

    frontLeftVictor.write(110);
    backLeftVictor.write(110);
    frontRightVictor.write(118);
    backRightVictor.write(118);
}
/******************Turn Left******************************/
/*
 * This is the opposite logic of turnRight. This runs the 
 * right side faster than the left to produce a left side turn
 * and to aid in this, the left side will begin running in reverse. 
 */
void turnLeft() {
  Serial.print("turnLeft was called\n");
  frontLeftVictor.write(75);
  backLeftVictor.write(75);
  frontRightVictor.write(67);
  backRightVictor.write(67);
}
void loop() {
/**************Motor Running Forward*******************/
/*
 * The robot will run forward until it comes accross a 
 * LOW signal from a light sensor. This should ONLY occur 
 * because it went over the duct tape line we set up. When 
 * this happens, the backup function is called and depending
 * on whose turn it is, either turnRight or turnLeft will be 
 * called and will return to this; waiting for another LOW 
 * signal to trip the sensors. 
 */
   frontLeftVictor.write(100);
   backLeftVictor.write(100);
   frontRightVictor.write(83); //Right motors face opposite direction of left motors so they need to run the opposite direction. 
   backRightVictor.write(83);
   if(digitalRead(RS1) == LOW || digitalRead(LS1) == LOW || digitalRead(RS2) == LOW || digitalRead(LS2) == LOW )
   { 
      timeNow = millis(); // We use this to have timed loops instead of logic loops. 
      while(millis() - timeNow < howLongBack ) // The timed loop here will run for 1.5 seconds
      {
        backup();  
      }
      count++;
      Serial.print(count);
      if ( count % 2 == 1 ) // On odd passes, we want to turn right. 
      {
        Serial.print("IF occurred\n");
        timeNow = millis();
        while(millis() - timeNow < howLongTurn ) // The timed loop here will run for 2.25 seconds
        {
          turnRight();
        }
      }
      else if ( count % 2 == 0 )
      {
         timeNow = millis();
         Serial.print("ELSE occurred\n"); // On even passes, we want to turn left. 
         while(millis() - timeNow < howLongTurn)// The timed loop here will run for 2.25 seconds
         {
           turnLeft();         
         }
      }
    }  
}
