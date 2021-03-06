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

    //talon values are set at their normal values when true, and are set to a fraction of their normal values when false
    HighSpeed = true;

    leftMC.reset(new MotionController("LeftDriveMC",left1));
    rightMC.reset(new MotionController("RightDriveMC",right1));
    motionControlled = false;

    								    //p,   i,   d,   f,                  source,             output
    turnControl.reset(new PIDController(0.0, 0.0, 0.0, 0.0, (PIDSource *) Robot::imu.get(), this));
    turnControl->SetContinuous(true);
    turnControl->SetInputRange(-180.0,    180.0);
    turnControl->SetOutputRange(-1.0, 1.0);	//so that the robot moves slowly when turning
    turnControl->SetAbsoluteTolerance(4.0);

    SetVoltageMode();
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

void DriveTrain::PIDWrite(double output) {
	ArcadeDrive(output, 0);	//robot only turns
}

void DriveTrain::Rotate_by_PID(float angle) {
	Robot::imu->ZeroYaw();	//"Sets the user-specified yaw offset to the current yaw value reported by the sensor."
	turnControl->SetSetpoint(angle);
	turnControl->Enable();
}

void DriveTrain::Config() {
	//put config stuff here

	left1->SetInverted(true);
	left2->SetInverted(true);
	right1->SetInverted(false);
	right2->SetInverted(false);

	//left1->SetVoltageRampRate(1);
	//left2->SetVoltageRampRate(1);
	//right1->SetVoltageRampRate(1);
	//right1->SetVoltageRampRate(1);

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

//arcade drive also works as a split drive
void DriveTrain::ArcadeDrive(float x, float y)
{
	//std::cout <<  "YAW: " << Robot::imu->GetYaw() << std::endl;

	if (motionControlled) return;
	float nx = Limit(x);	//the n stand for new (new x)
	float ny = Limit(y);

	//square axis values without changing signs
	if (ny >= 0) {
		ny = ny * ny;
	}
	else {
		ny = -(ny * ny);
	}

	if (nx >= 0) {
		nx = nx * nx;
	}
	else {
		nx = -(nx * nx);
	}

	//print axis values
	std::cout << "4329_LOG: " << GetTime() <<  ", driverinput: axes: x: " << nx << ", y: " << ny;

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
		else if (nx < 0.0)// If turning/rotating left or not moving
		{
			leftOutput = std::max(ny, -nx);
			rightOutput = ny + nx;
		}
		else {
			leftOutput = ny;
			rightOutput = ny;
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
		}
		else {
			leftOutput = ny;
			rightOutput = ny;
		}
	}

	//set outputs
	if (!HighSpeed) {
		leftOutput *= 0.5;
		rightOutput *= 0.5;
	}

	left1->Set(leftOutput);
	right1->Set(rightOutput);

	std::string solenoid_value;
	if (RobotMap::transmissionShifterSolenoid->Get() == frc::DoubleSolenoid::kForward) {
		solenoid_value = "Forward";
	}
	else {
		solenoid_value = "Reverse";
	}

	//print outputs
	std::cout << ", motor command L: " << leftOutput << " R: " << rightOutput << ", transmission: " << solenoid_value;

	//output voltage and current
	std::cout << ", output voltage: left1: " << left1->GetOutputVoltage() << ", ";
	//std::cout << "left2: " << left2->GetOutputVoltage() << ", ";
	std::cout << "right1: " << right1->GetOutputVoltage() << ", ";
	//std::cout << "right2: " << right2->GetOutputVoltage() << std::endl;

	std::cout << "output current: left1: " << left1->GetOutputCurrent() << ", ";
	//std::cout << "left2: " << left1->GetOutputCurrent() << ", ";
	std::cout << "right1: " << right1->GetOutputCurrent() << ", " << std::endl;
	//std::cout << "right2: " << right2->GetOutputCurrent() << std::endl;

}

void DriveTrain::TankDrive(float left, float right)
{
	if (motionControlled) return;
	float leftOutput = Limit(left);	//n stands for new
	float rightOutput = Limit(right);

	//print axis values
	std::cout << "TankDrive axes: Left: " << leftOutput << " Right: " << rightOutput << std::endl;

	//set outputs
	if (!HighSpeed) {
		leftOutput *= 0.5;
		rightOutput *= 0.5;
	}

	left1->Set(leftOutput);
	right1->Set(rightOutput);

	//print outputs
	std::cout << "4329 Log: " << GetTime() << ", L: " << leftOutput << " R: " << rightOutput << std::endl;

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

//Positive voltage moves the robot in the direction of the gear holder
//negative voltage moves the robot in the direction of the intake
void DriveTrain::DirectDrive(float left, float right) {
	std::cout << "4329 Log: " << GetTime() << ", Left: " << left << " Right: " << right << std:: endl;
	left1->Set(left);
	right1->Set(right);
}

void DriveTrain::HeadingDrive(float output) {
	float adjust = max_adjust;
	float left = output;
	float right = output;

	//check the gear holder side of the robot is facing forward or if it is facing the other direction
	bool gear_forward;
	if (output > 0) {
		gear_forward = true;
	}
	else {
		gear_forward = false;
	}

	//recalibrate the output
	double yaw = Robot::imu->GetYaw();
	if (gear_forward) {
		if (yaw > 0) {
			//increase left side
			adjust = max_adjust * (yaw / max_angle);
			left += fabs(adjust);
			std::cout << "adjust "<< adjust << ", yaw: " << yaw << std::endl;
		}
		else {
			//increase right side
			adjust = max_adjust * (yaw / max_angle);
			right += fabs(adjust);
		} //yaw = 0 will not changes output
	}
	else {
		if (yaw > 0) {
			//increase right side
			adjust = max_adjust * (yaw / max_angle);
			right -= fabs(adjust);
		}
		else {
			//increase left side
			adjust = max_adjust * (yaw / max_angle);
			left -= fabs(adjust);

		} //yaw = 0 will not changes output
	}


	//print out stuff
	std::cout << "4329 Log: " << GetTime() << ", gear_forward: " << gear_forward << ", yaw: " << yaw << ", right: " << right << ", left: " << left << std::endl;

	//set the output
	left1->Set(left);
	right1->Set(right);
}

void DriveTrain::StopMotors() {
	left1->Set(0);
	right1->Set(0);
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

	std::cout << "4329 Log: " << GetTime() << ", drivetrain set Voltage mode on kPercentVbus" << std::endl;

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

void DriveTrain::SetPosition(int left, int right)
{
	if (motionControlled) return;
	left1->SetPosition(left);
	right1->SetPosition(right);
}

void DriveTrain::ToggleHighSpeed() {
	if (HighSpeed) {
		HighSpeed = false;
	}
	else if (!HighSpeed) {
		HighSpeed = true;
	}
	else {	//just in case HighSpeed was neither true nor false
		HighSpeed = true;
	}
}

bool DriveTrain::GetHighSpeed() {
	return HighSpeed;
}

void DriveTrain::AddMotionProfile(int index, int gainslot, double leftProfilePoints[][3], int leftSize,
		                                       double rightProfilePoints[][3], int rightSize )
{
	leftMC->AddProfile(index,leftProfilePoints,leftSize,gainslot);
	rightMC->AddProfile(index,rightProfilePoints,rightSize,gainslot);
}

bool DriveTrain::SetActiveMotionProfile(int index)
{
	return (leftMC->SetActiveProfile(index) && rightMC->SetActiveProfile(index));
}

void DriveTrain::StartMotionProfile()
{
	leftMC->Start();
	rightMC->Start();
	motionControlled = true;
}

void DriveTrain::MotionControl()
{
	leftMC->Control();
	rightMC->Control();
}

void DriveTrain::StopMotionProfile()
{
	leftMC->Stop();
	rightMC->Stop();
	motionControlled = false;
	SetVoltageMode();
}

void DriveTrain::ResetMotionProfile()
{
	leftMC->Reset();
	rightMC->Reset();
	motionControlled = false;
	SetVoltageMode();
}

bool DriveTrain::MotionProfileInProgress()
{
	return (leftMC->ProfileInProgress() || rightMC->ProfileInProgress());
}

bool DriveTrain::MotionProfileComplete()
{
	return (leftMC->ProfileComplete() && rightMC->ProfileComplete());
}

void DriveTrain::EnableMotionDebug(bool enable)
{
	leftMC->EnableDebug(enable);
	rightMC->EnableDebug(enable);
}


