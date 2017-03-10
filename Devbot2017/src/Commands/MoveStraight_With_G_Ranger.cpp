#include "../I2C_Sensor_Mgr.h"
#include "MoveStraight_With_G_Ranger.h"

MoveStraight_With_G_Ranger::MoveStraight_With_G_Ranger(bool forward, double cm_target_distance): Command() {
        // Use requires() here to declare subsystem dependencies
    Requires(Robot::driveTrain.get());

    //forward is the intake side

    _forward = forward;
    //values are swapped
    if (_forward) {
    	output = -0.5;
    }
    else {
    	output = 0.5;
    }

    distance = 0.0;
    target_distance = cm_target_distance;

    count = 0;
}

// Called just before this Command runs the first time
void MoveStraight_With_G_Ranger::Initialize() {
	std::cout << "now moving straight" << std::endl;
}

// Called repeatedly when this Command is scheduled to run
void MoveStraight_With_G_Ranger::Execute() {
	Robot::driveTrain->DirectDrive(output, output);	//move forward or backwards
}

// Make this return true when this Command no longer needs to run execute()
bool MoveStraight_With_G_Ranger::IsFinished() {
	//get distance from ultrasonic sensor
	I2C_Sensor_Mgr::Instance()->Update_GearRangeFinder();
	distance = I2C_Sensor_Mgr::Instance()->Get_GearRange_cm();

	std::cout << "MS distance: " << distance << std::endl;

	if (distance > 1000) {
		count++;
		return false;
	}
	else {
		count = 0;
	}

	if (count > 10) { //stop if the numbers have been off for too long
		return true;
	}

	if (_forward) {	//distance should be increasing, so stop when it's past a certain distance
		if (distance > target_distance) {
			Robot::driveTrain->StopMotors(); //stop early
			return true;
		}
		else {
			return false;
		}
	}
	else {	//distance should be decreasing, so stop when it's less than a certain distance
		if (distance < target_distance) {
			Robot::driveTrain->StopMotors();
			return true;
		}
		else {
			return false;
		}
	}
}

// Called once after isFinished returns true
void MoveStraight_With_G_Ranger::End() {
	std::cout << "done moving straight" << std::endl;
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void MoveStraight_With_G_Ranger::Interrupted() {

}