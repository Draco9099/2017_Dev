// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#include "CANTalon.h"

#include "DriveTrain.h"
#include "../RobotMap.h"
#include "../Commands/TeleopDrive.h"
// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS
// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS

DriveTrain::DriveTrain() : Subsystem("DriveTrain") {
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
    left1 = RobotMap::driveTrainLeft1;
    right2 = RobotMap::driveTrainRight2;
    right1 = RobotMap::driveTrainRight1;
    left2 = RobotMap::driveTrainLeft2;
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS

    imu = RobotMap::imu;

    Config();
}

void DriveTrain::InitDefaultCommand() {
    // Set the default command for a subsystem here.
     SetDefaultCommand(new TeleopDrive()); //setDefaultCommand is used to run commands that take axis values
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND
}


// Put methods for controlling this subsystem
// here. Call these from Commands.

void DriveTrain::Config() {
	//put config stuff here

	left1->SetInverted(true);
	left2->SetInverted(true);
	right1->SetInverted(false);
	right2->SetInverted(false);
}

float DriveTrain::Limit(float num) {	//set number to correct value if it's over 1 or under -1
										//( joystick value is usually between 1 and -1, but may be otherwise)
	if (num > 1) {
		return 1;
	}
	else if (num < -1) {
		return -1;
	}
	else {
		return num;
	}
}

void DriveTrain::ArcadeDrive(float x, float y) {
	float nx = Limit(x);	//the n stand for new (new x)
	float ny = Limit(y);

	//print axis values
	std::cout << "ArcadeDrive axes: x: " << nx << ", y: " << ny << std::endl;

	float leftOutput;
	float rightOutput;

	//calculate outputs
	if (ny > 0.0)  // If moving forward
	{
		if (nx > 0.0) // If turning/rotating right
		{
			leftOutput = ny - nx;
			rightOutput = std::max(ny, nx);
		}
		else if (nx < 0.0) // If turning/rotating left or not moving
		{
			leftOutput = std::max(ny, -nx);
			rightOutput = ny + nx;
		} else
		{
			leftOutput = -1 * ny;
			rightOutput = -1 * ny;
		}
	}
	else  // If moving backward
	{
		if (nx > 0.0) // If turning/rotating right
		{
			leftOutput = - std::max(-ny, nx);
			rightOutput = ny + nx;
		}
		else if (nx < 0.0) // If turning/rotating left or not moving
		{
			leftOutput = ny - nx;
			rightOutput = - std::max(-ny, -nx);
		} else
		{
			leftOutput = -1 * ny;
			rightOutput = -1 * ny;
		}
	}

	std::cout << "L: " << leftOutput << " R: " << rightOutput << std::endl;

	//set outputs
	left1->Set(leftOutput);
	right1->Set(rightOutput);

	//output voltage and current
	//std::cout << "ArcadeDrive voltage: left1: " << left1->GetOutputVoltage() << ", ";
	//std::cout << "left2: " << left1->GetOutputVoltage() << ", ";
	//std::cout << "right1: " << right1->GetOutputVoltage() << ", ";
	//std::cout << "right2: " << right2->GetOutputVoltage() << std::endl;

	//std::cout << "ArcadeDrive current: left1" << left1->GetOutputCurrent() << ", ";
	//std::cout << "left2: " << left1->GetOutputCurrent() << ", ";
	//std::cout << "right1: " << right1->GetOutputCurrent() << ", ";
	//std::cout << "right2: " << right2->GetOutputCurrent() << std::endl;
}

void DriveTrain::TankDrive(float left, float right) {
	float nleft = Limit(left);	//n stands for new
	float nright = Limit(right);

	//print axis values
	std::cout << "TankDrive axes: Left: " << nleft << " Right: " << nright << std::endl;

	//set outputs
	left1->Set(nleft);
	right1->Set(nright);

	//output voltage and current
	std::cout << "TankDrive voltage: left1: " << left1->GetOutputVoltage() << ", ";
	std::cout << "left2: " << left1->GetOutputVoltage() << ", ";
	std::cout << "right1: " << right1->GetOutputVoltage() << ", ";
	std::cout << "right2: " << right2->GetOutputVoltage() << std::endl;

	std::cout << "TankDrive current: left1" << left1->GetOutputCurrent() << ", ";
	std::cout << "left2: " << left1->GetOutputCurrent() << ", ";
	std::cout << "right1: " << right1->GetOutputCurrent() << ", ";
	std::cout << "right2: " << right2->GetOutputCurrent() << std::endl;
}


void DriveTrain::SetVoltageMode() {
	//change mode
	left1->SetControlMode(CANSpeedController::kPercentVbus);
	left2->SetControlMode(CANSpeedController::kFollower);
	right1->SetControlMode(CANSpeedController::kPercentVbus);
	right2->SetControlMode(CANSpeedController::kFollower);

	//set which master talons the slave talons follow
	left2->Set(1); // change to work with config system
	right2->Set(3);

	std::cout << "drivetrain set Voltage mode on kPercentVbus" << std::endl;

	left1->Enable();	//enable after changing the control mode
	left2->Enable();
	right1->Enable();
	right2->Enable();
}

void DriveTrain::SetPositionMode(){
	//set encoder and change control mode
	left1->SetFeedbackDevice(CANTalon::QuadEncoder);
	left1->ConfigEncoderCodesPerRev(1024);
	left1->SetControlMode(CANSpeedController::kPosition);
	left2->SetControlMode(CANSpeedController::kFollower);

	right1->SetFeedbackDevice(CANTalon::QuadEncoder);
	right1->ConfigEncoderCodesPerRev(1024);
	right1->SetControlMode(CANSpeedController::kPosition);
	right2->SetControlMode(CANSpeedController::kFollower);

	//set which master talons the slave talons follow
	left2->Set(1); // change to work with config system
	right2->Set(3);

	//motors won't move
	left1->Set(0);
	right1->Set(0);
}

void DriveTrain::SetPosition(int left, int right) {
	left1->SetPosition(left);
	right1->SetPosition(right);
}
