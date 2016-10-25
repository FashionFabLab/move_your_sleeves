/*
#                     MOVE YOUR SLEEVES
#
# Software to manage dress's sleeves.
#
# Copyright (c) 2016 Tiziana Dallapè
# Copyright (c) 2016 Matteo Perini
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 3 as
# published by the Free Software Foundation
#
# See LICENSE file for details
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc.,59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
#
# See https://github.com/FashionFabLab/move_your_sleeves for detailed
# project
*/



/*----------------------------------------------------------------------
 objects and variables declarations and definitions list
----------------------------------------------------------------------*/
const int sensor = A9;           //attach temperature sensor to pin A9
const int enable_motor_L = 10;   //attach enable left motor to pin 10
const int enable_motor_R = 10;   //attach enable right motor to pin 10
const int control1_L = 2;        //attach left motor to pins 2 3
const int control2_L = 3;
const int control3_R = 6;        //attach left motor to pins 6 12
const int control4_R = 12;
//service variables
float T = 0;      //environmental temperature measure
boolean b_up;     //variable to block motor up position
boolean b_down;   //variable to block motor down position
//user customizable variables
const float T_threshold_down = 40.0;/*temperature threshold to move down
                                      sleeves; default: 25.0°C*/
const float T_threshold_up = 38.0;  /*temperature threshold to move up
                                      sleeves; default: 27.0°C*/
int rotation_time_up = 9500;        /*change value according to sleeves
                                      lenght; default: 9500ms*/
int rotation_time_down = 8450;      //default: 8450ms
int check_T_time = 1000;            //time between temperature readings



void setup() {
/*----------------------------------------------------------------------
 SETUP FUNCTION
   this function runs once at the start of the program which is used to
   define initial environmental settings
----------------------------------------------------------------------*/
  pinMode(enable_motor_L, OUTPUT);    //left motor impulse
  pinMode(enable_motor_R, OUTPUT);    //right motor impulse
  pinMode(control1_L, OUTPUT);
  pinMode(control2_L, OUTPUT);
  pinMode(control3_R, OUTPUT);
  pinMode(control4_R, OUTPUT);
  pinMode(sensor, INPUT);             //sensor reading
  Serial.begin(9600);                 //serial initialization
  digitalWrite(enable_motor_L, LOW);  //initialize motors off
  digitalWrite(enable_motor_R, LOW);  
  b_up = false;                       //initialize "up" function block
  b_down = true;                      /*initialize "down" function block;
                                        set sleeves down when
                                        microcontroller is powered*/
}


float check_T() {
/*----------------------------------------------------------------------
 environment temperature check function
   temperature sensor reads units values that are converted into degrees
----------------------------------------------------------------------*/
  Serial.print("Temperature is... ");
  int sensor_val = analogRead(sensor);
  float voltage = (sensor_val/1024.0)*3.7;
  T = (voltage-.5)*100;
  Serial.print(T);
  Serial.print((char)176);
  Serial.println("C");
  return T;
}


void up(){
/*----------------------------------------------------------------------
 motor moving function which pulls up sleeves
----------------------------------------------------------------------*/
  Serial.println("moving up");
  digitalWrite(enable_motor_L, HIGH);
  digitalWrite(enable_motor_R, HIGH);
  digitalWrite(control1_L, HIGH);
  digitalWrite(control3_R, HIGH);
  digitalWrite(control2_L, LOW);
  digitalWrite(control4_R, LOW);
  delay(rotation_time_up);
  digitalWrite(enable_motor_L, LOW);
  digitalWrite(enable_motor_R, LOW);
  b_up = true;     //block sleeves up
  b_down = false;
  delay(5000);
}
void down(){
/*----------------------------------------------------------------------
 motor moving function which pulls down sleeves
----------------------------------------------------------------------*/
  Serial.println("moving down");
  digitalWrite(enable_motor_L, HIGH);
  digitalWrite(enable_motor_R, HIGH);
  digitalWrite(control1_L, LOW);
  digitalWrite(control3_R, LOW);
  digitalWrite(control2_L, HIGH);
  digitalWrite(control4_R, HIGH);
  delay(rotation_time_down);
  digitalWrite(enable_motor_L, LOW);
  digitalWrite(enable_motor_R, LOW);
  b_up = false;
  b_down = true;  //block sleeves down
  delay(2000);
}


void loop() {
/*----------------------------------------------------------------------
 LOOP FUNCTION
   moving loop routine runs countless times calling some functions;
   when microcontroller is powered moving loop starts checking
   temperature: if the environmental temperature is higher than
   temperature threshold, sleeves move up; when temperature decrease
   under temperature threshold, sleeves move down
----------------------------------------------------------------------*/
  T = check_T();
  if(T>T_threshold_up && b_up==false){
    up();
  }else if(T<T_threshold_down && b_down==false){
    down();
  }else{
    delay(check_T_time);
  }
}
