// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#ifndef ROBOTMAP_H
#define ROBOTMAP_H

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
#include "WPILib.h"
#include "CANTalon.h"
// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
#include "Pixy.h"
#include "AHRS.h"

/**
 * The RobotMap is a mapping from the ports sensors and actuators are wired into
 * to a variable name. This provides flexibility changing wiring, makes checking
 * the wiring easier and significantly reduces the number of magic numbers
 * floating around.
 */
class RobotMap {
public:
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	static std::shared_ptr<CANTalon> driveTrainLeft1;
	static std::shared_ptr<CANTalon> driveTrainRight2;
	static std::shared_ptr<CANTalon> driveTrainRight1;
	static std::shared_ptr<CANTalon> driveTrainLeft2;
	// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	static std::shared_ptr<DoubleSolenoid> transmissionShifterSolenoid;
	static std::shared_ptr<CANTalon> intakeMotor;
	static std::shared_ptr<Solenoid> extenderSolenoid;
	static std::shared_ptr<Solenoid> ventSolenoid;
	static std::shared_ptr<CANTalon> shooterTalon1;
	static std::shared_ptr<CANTalon> shooterTalon2;
	static std::shared_ptr<CANTalon> conveyorTalon;
	static std::shared_ptr<Pixy> pixy;
	static std::shared_ptr<AHRS> imu;
	static std::shared_ptr<CANTalon> indexTalon;
	static std::shared_ptr<CANTalon> gearSolenoidFlap;

	static std::shared_ptr<CANTalon> climberMotor;

	static void init();
};
#endif
