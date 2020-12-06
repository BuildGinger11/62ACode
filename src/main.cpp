#include "main.h"

//pros::Imu Inersial(17);
// R2 = shift
//R1 = intake
//R1+shift = outake
//Y = slow intake
//shift+Y = slow outake
//L1 = arm up
//L2 = arm down
//L1+shift = descore
//L2+shift = descore
//Right arrow = arm down, tray back
//B = tray in
//down arrow = tray out
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}
void initialize()
{
  //pros::delay(500);
//  tray_pid_task.suspend();

	Drive_pid_task.suspend();
//  armreset();
  pros::lcd::initialize();
  pros::lcd::set_text(1, "Hello PROS User!");
  pros::lcd::register_btn1_cb(on_center_button);
	ResetInersial();

}

void disabled() {}
void competition_initialize() {


}
void autonomous()
{
	Drive_pid_task.resume();
	//printf("working \n");
	set_angle(90);
	pros::delay(5000);
	Drive_pid_task.suspend();
}



void opcontrol()
{

  while (true)
  {
    drive(master.get_analog(ANALOG_RIGHT_Y), master.get_analog(ANALOG_LEFT_Y));
  	liftstuff();
  	intakestuff();
    pros::delay(10);
  }
}
