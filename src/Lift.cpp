#include "main.h"
#include "api.h"
//B = tray in
//down arrow = tray out
//down = 1165
//middle = 2375
//up = 3842
pros::Motor Index(20, true);
pros::Motor lift(19);
pros::ADIDigitalIn AutoIndexer(8);
pros::ADIDigitalIn Stopper(2);
pros::ADIDigitalIn Hoodstopper(1);
//pros::IntakeCheak(2);
void EnReset()
{
  Index.set_zero_position(0);
  lift.set_zero_position(0);
}
void Lift(int power)
{
  lift.move(power);
  Index.move(power);
}
//void EnLift(int power, int encoder)
//{
//  EnReset();
//  if(lift.get_position() < encoder)
//  {
//    lift.move(power);
//    Index.move(power);

//  }
//}
int i = 0;
bool LPressed = false;
void bringdown ()
{
  while(!Stopper.get_value())
    {
      if(master.get_digital(DIGITAL_R2))
      {
        break;
      }
      Lift(-127);
      if(i*10 >= 300)
      {
        i = 0;
        break;
      }
      i++;
      pros::delay(20);
    }
}
int amount = 0;
bool Auto = false;
void liftstuff()
{
  if(AutoIndexer.get_value() == 1)
  {
      Lift(127);
      Auto = true;
  }
  if(Auto == true && Stopper.get_value() == 0)
  {
    Lift(100);
  }
  if(Stopper.get_value() == 1)
  {
    Auto = false;
    Lift(0);
  }
  else
  {
    Lift(0);
  }

  if(master.get_digital(DIGITAL_L2) && master.get_digital(DIGITAL_L1))
  {
    Lift(-127);
    //bringdown();
  }
  else if(Hoodstopper.get_value() == 1)
  {
    amount++;
  }
  else if(amount == 3)
  {
    amount = 0;
    bringdown();
  }
  else if(master.get_digital(DIGITAL_R1))
  {
    while(master.get_digital(DIGITAL_R1))
    {
        Lift(127);
    }
    bringdown();
  }
  else
  {
    Lift(0);
  }
}
