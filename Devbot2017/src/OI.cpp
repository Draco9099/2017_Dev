// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#include "OI.h"

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
#include "SmartDashboard/SmartDashboard.h"
#include "Commands/AutonomousCommand.h"
#include "Commands/TeleopDrive.h"
#include "Commands/ToggleShift.h"
// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
#include "Commands/FuelIntake.h"
#include "Commands/ReleaseGear.h"
#include "Commands/Shoot.h"
#include "Commands/Climb.h"
#include "Commands/Vent.h"
#include "Commands/ClimbSequence.h"
#include "Commands/PrintBlockData.h"
#include "Commands/OpenFlap.h"
#include "Commands/PushGearHolder.h"
#include "Commands/PullGearHolder.h"
#include "Commands/ToggleGearFlap.h"

#include <cmath> //for dead zones

OI::OI() {
    // Process operator interface input here.
    robotOperator.reset(new XboxController(1));
    robotDriver.reset(new XboxController(0));
    
    //reset shared pointer buttons
    shift.reset(new JoystickButton(robotDriver.get(), Xbox_Button::A));
    ReleaseGear_B.reset(new JoystickButton(robotOperator.get(), Xbox_Button::Right_Bumper));
    shoot.reset(new JoystickButton(robotOperator.get(), Xbox_Button::X));
    RobotClimb.reset(new JoystickButton(robotOperator.get(), Xbox_Button::Y));
    RobotVent.reset(new JoystickButton(robotOperator.get(), Xbox_Button::Left_Bumper));
    Flap.reset(new JoystickButton(robotOperator.get(), Xbox_Button::B));

    //Specify which commands to call when button is pressed, released, etc
    Flap->WhenPressed(new OpenFlap());	//actually while held
    ReleaseGear_B->WhenPressed(new ReleaseGear());
    shoot->WhenPressed(new Shoot());	//actually while held
    RobotClimb->WhenPressed(new ClimbSequence());	//actually while held
    RobotVent->WhenPressed(new Vent());
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS
    shift->WhenPressed(new ToggleShift());


    // SmartDashboard Buttons
    SmartDashboard::PutData("ToggleShift", new ToggleShift());
    SmartDashboard::PutData("Autonomous Command", new AutonomousCommand());
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS
    SmartDashboard::PutData("PrintBlockData", new PrintBlockData());
    SmartDashboard::PutData("Vent", new Vent());
    SmartDashboard::PutData("PushGearHolder", new PushGearHolder());
    SmartDashboard::PutData("PullGearHolder", new PullGearHolder());
    SmartDashboard::PutData("ToggleGearFlap", new ToggleGearFlap());
}

std::shared_ptr<XboxController> OI::getRobotDriver() {
   return robotDriver;
}

std::shared_ptr<XboxController> OI::getRobotOperator() {
   return robotOperator;
}

std::shared_ptr<JoystickButton> OI::getClimbButton() {
   return RobotClimb;
}

std::shared_ptr<JoystickButton> OI::getShootButton() {
   return shoot;
}

std::shared_ptr<JoystickButton> OI::getFlapButton() {
   return Flap;
}

float OI::GetAxisValue(std::shared_ptr<XboxController> controller, int axis) {
	float value;

	switch (axis) {
	case Xbox_Axis::Xbox_Axis_Left_X:
		value = No_DeadZone_Value(controller->GetRawAxis(axis), LeftX_min);
		if (value == 0.0) { value = 0.0; }	//so value isn't -0, but is 0
		break;

	case Xbox_Axis::Xbox_Axis_Left_Y:
		value = No_DeadZone_Value(controller->GetRawAxis(axis), LeftY_min);	//y axis shouldn't be inverted
																			//as the arcade drive code is tailored to
																			//the y axis not being inverted
		if (value == 0.0) { value = 0.0; }
		break;

	case Xbox_Axis::Xbox_Axis_Left_Trigger:
		value = No_DeadZone_Value(controller->GetRawAxis(axis), LeftTrigger_min);
		break;

	case Xbox_Axis::Xbox_Axis_Right_Trigger:
		value = No_DeadZone_Value(controller->GetRawAxis(axis), Righttrigger_min);
		break;

	case Xbox_Axis::Xbox_Axis_Right_X:
		value = No_DeadZone_Value(controller->GetRawAxis(axis), RightX_min);
		if (value == 0.0) { value = 0.0; }
		break;

	case Xbox_Axis::Xbox_Axis_Right_Y:
		value = No_DeadZone_Value(controller->GetRawAxis(axis), RightY_min);	//y axis shouldn't be inverted
		if (value == 0.0) { value = 0.0; }
		break;

	case Xbox_Axis::Xbox_Axis_DPad_X:
		value = controller->GetRawAxis(axis);
		break;

	case Xbox_Axis::Xbox_Axis_DPad_Y:
		value = controller->GetRawAxis(axis);
		break;

	default:
		std::cerr << "Unhandled axis: " << __LINE__ << " value " << axis << std::endl;
		value = 0;
	}

	return value;
}

float OI::No_DeadZone_Value(float value, float deadZone) {	//works with all axes
	if ( fabs(value) < deadZone ) {
		return 0.0;
	}
	else {
		return value;
	}
}
