#include "main.h"
pros::Controller master(CONTROLLER_MASTER);

pros::Motor left_back(7, true);
pros::Motor left_front(8, true);
pros::Motor right_back(9);
pros::Motor right_front(10);
pros::Imu Inersial(6);

float wheel_size = 4.2; //2.75
float tick_per_rev = 360 * (7/3); //360
float circumference = wheel_size*M_PI;
float tick_per_inch = (tick_per_rev/circumference);
int clipit(int max, int min, int input)
{
  if(input > max)
  {
    return max;
  }
  else if(input < min)
  {
    return min;
  }
  return input;
}

bool checkshift()
{
  return master.get_digital(DIGITAL_R2);
}
int InersialReadings ()
{
  return Inersial.get_rotation();
}
void ResetInersial ()
{
  Inersial.reset();
}
int RightEncoderval ()
{
  int RightAvrage = (right_front.get_position() + right_back.get_position()) / 2;
  return RightAvrage;

}
int LeftEncoderval ()
{
  int RightAvrage = (left_front.get_position() + left_back.get_position()) / 2;
  return RightAvrage;

}
void drive(int r, int l)
{
  right_front.move(r);
  right_back.move(r);
  left_back.move(l);
  left_front.move(l);

}


int Right_Target_Value;
int Left_Target_Value;

void Set_Drive(int Target)
{
  Left_Target_Value = LeftEncoderval()  + (Target*tick_per_inch);
  Right_Target_Value = RightEncoderval() + (Target*tick_per_inch);
}
int angle;
void Set_Angle(int Target)
{
  angle = Target;
}
//_____________________________________________________________________________________________________________________________________________________
void
Drive_pid(void*)
 {
   int Re;
   int Le;
   int Ae;
   int Angle_P;
   int Rightmotorval;
   int Leftmotorval;
   int maxval = 80;
   int minval = 5;
   int angle = 0;
   float Kp = 0.05;
  while (true)
   {
     Angle_P = angle - InersialReadings();
     Le = Right_Target_Value - LeftEncoderval();
     Re = Left_Target_Value - RightEncoderval();
     Rightmotorval = Kp * Re;
     Leftmotorval = Kp * Le;
     Rightmotorval = clipit(maxval, minval, Rightmotorval);
     Leftmotorval = clipit(maxval, minval, Leftmotorval);
     printf("Rightmotorval = %d \n", Rightmotorval);
     printf("Angle_P = %d and InersialReadings = %d \n", Angle_P, InersialReadings());
      if (pros::millis()>=2000)
      {
        printf("working \n");
        drive(Rightmotorval + Angle_P, Leftmotorval + Angle_P);
      }
      else
      {
        printf("millis = %d \n", pros::millis());
        printf("Almost working \n");
        drive(0, 0);
      }

    //pid goes here
    pros::delay(10);
  }
}
pros::Task Drive_pid_task(Drive_pid, nullptr, "drive_pid");
//_____________________________________________________________________________________________________________________________________________________
void
Turn_pid(void*)
 {
   int Angle_P;
   int Output;
   int maxval = 80;
   int minval = 5;
   float Kp = 1.0;
  while (true)
   {
     Angle_P = angle - InersialReadings();
     Output = Angle_P * Kp;
     Output = clipit(maxval, minval, Output);
     drive(Output, -Output);
    //pid goes here
    pros::delay(10);
  }
}
pros::Task Turn_pid_task(Drive_pid, nullptr, "drive_pid");
void
wait_drive() {
  pros::delay(20);
  int i = 0;
  while (true) {
    pros::delay(20);
    if (abs(Left_Target_Value-LeftEncoderval())<40 && abs(Right_Target_Value-RightEncoderval())<40) {
      return;
    }
    if (abs(Left_Target_Value-LeftEncoderval())<150 && abs(Right_Target_Value-RightEncoderval())<150) {
      i++;
      if (i>1000/20)
      {
        return;
      }
    }
    else
    {
      i = 0;
    }
  }

}
