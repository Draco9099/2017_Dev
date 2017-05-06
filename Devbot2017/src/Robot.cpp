// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#include "Robot.h"

#include <thread>

#include "profiles.h"

#include "mindsensors.h"
using mindsensors::CANLight;

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INITIALIZATION
std::shared_ptr<DriveTrain> Robot::driveTrain;
std::shared_ptr<Transmission> Robot::transmission;
std::unique_ptr<OI> Robot::oi;
// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INITIALIZATION
std::shared_ptr<Intake> Robot::intake;
std::shared_ptr<GearHolder> Robot::gearholder;
std::shared_ptr<FloorGear> Robot::floorGear;
std::shared_ptr<Shooter> Robot::shooter;
std::shared_ptr<ShooterPixy> Robot::shooterPixy;
std::shared_ptr<GearPixy> Robot::gearPixy;
std::shared_ptr<Climber> Robot::climb;
std::shared_ptr<ShooterIndex> Robot::shooterIndex;
std::shared_ptr<Video> Robot::video;
std::shared_ptr<AHRS> Robot::imu;

cameraControl Robot::cc;

void VideoT()
{
	Robot::video->Run();
}

void Robot::RobotInit() {
	std::cout << "robot init start" << std::endl;

	RobotMap::init();

    frameLights = new CANLight(3);
    ds = &DriverStation::GetInstance();

	imu.reset(new AHRS(frc::SPI::Port::kMXP));
	shooterIndex.reset(new ShooterIndex());
	climb.reset(new Climber());
	intake.reset(new Intake());
	gearholder.reset(new GearHolder());
	floorGear.reset(new FloorGear());
	shooter.reset(new Shooter());
	shooterPixy.reset(new ShooterPixy());
	gearPixy.reset(new GearPixy());
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS
    driveTrain.reset(new DriveTrain());
    transmission.reset(new Transmission());
	// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS
	// This MUST be here. If the OI creates Commands (which it very likely
	// will), constructing it during the construction of CommandBase (from
	// which commands extend), subsystems are not guaranteed to be
	// yet. Thus, their requires() statements may grab null pointers. Bad
	// news. Don't move it.
	oi.reset(new OI());

	// instantiate the command used for the autonomous period
	chooser.reset(new frc::SendableChooser<Command*>());
	chooser->AddDefault(std::string("2) Auto Move past line by timeout."), new AutoForward());
	chooser->AddObject(std::string("0) Auto Move past line by sensor."), new AutoSimple());
	chooser->AddObject(std::string("3) Place gear on center peg."), new Auto2());
	chooser->AddObject(std::string("4) Place gear on left peg."), new Auto1());
	chooser->AddObject(std::string("5) Place gear on right peg."), new Auto3());
	frc::SmartDashboard::PutData("Autonomous Modes", chooser.get());

	video.reset(new Video());	//must be called before the thread
	cc.SelectedCamera = 1;
	cc.changeCount = 0;
	video->SetCC(&cc);
	video->Init();

	std::thread visionThread(VideoT);
	visionThread.detach();

	imu->ZeroYaw(); //reset yaw before match starts

	//add motion profiles here
	//Robot::driveTrain->AddMotionProfile(k3ftMotionProfileNdx, 1, k3ftMotionProfile, k3ftMotionProfileSz, k3ftMotionProfile, k3ftMotionProfileSz);

	//shooter preferences
    Preferences::GetInstance()->PutFloat("Indexer::RPM",0);
    Preferences::GetInstance()->PutBoolean("Indexer::DirReversed",false);
    Preferences::GetInstance()->PutBoolean("Indexer::SensorReversed",false);

    Preferences::GetInstance()->PutDouble("Indexer::P",0.0);
    Preferences::GetInstance()->PutDouble("Indexer::I",0.0);
    Preferences::GetInstance()->PutDouble("Indexer::D",0.0);

	Preferences::GetInstance()->PutInt("Indexer::EncoderPulses",4096);
	Preferences::GetInstance()->PutInt("Indexer::AllowableCLerr",2048);
	Preferences::GetInstance()->PutDouble("Indexer::CLRamp",12.0);

    Preferences::GetInstance()->PutFloat("Shooter::RPM",0);
    Preferences::GetInstance()->PutBoolean("Shooter::DirReversed",false);
    Preferences::GetInstance()->PutBoolean("Shooter::SensorReversed",false);

    Preferences::GetInstance()->PutDouble("Shooter::P",0.0);
    Preferences::GetInstance()->PutDouble("Shooter::I",0.0);
    Preferences::GetInstance()->PutDouble("Shooter::D",0.0);

	Preferences::GetInstance()->PutInt("Shooter::EncoderPulses",512);
	Preferences::GetInstance()->PutInt("Shooter::AllowableCLerr",256);
	Preferences::GetInstance()->PutDouble("Shooter::CLRamp",12.0);

	std::cout << "robot init complete" << std::endl;
  }

/**
 * This function is called when the disabled button is hit.
 * You can use it to reset subsystems before shutting down.
 */
void Robot::DisabledInit(){
	/*
	if(ds->GetAlliance() == DriverStation::Alliance::kRed){
		disableseqred();
	}else if(ds->GetAlliance() == DriverStation::Alliance::kBlue){
		disableseqblue();
	}
	*/
}

void Robot::DisabledPeriodic() {
	Scheduler::GetInstance()->Run();

	//inactivefade();
}

void Robot::AutonomousInit() {
	autonomous.reset(chooser->GetSelected());
	if (autonomous.get() != nullptr)
		autonomous->Start();
}

void Robot::AutonomousPeriodic() {
	Scheduler::GetInstance()->Run();

	/*
	if(ds->GetAlliance() == DriverStation::Alliance::kRed){
		frameLights->ShowRGB(255, 0, 0);
	}else if(ds->GetAlliance() == DriverStation::Alliance::kBlue){
		frameLights->ShowRGB(0, 0, 255);
	}else if(ds->GetAlliance() == DriverStation::Alliance::kInvalid){
		frameLights->ShowRGB(255, 200, 0); //Yellow
	}
	*/
}

void Robot::TeleopInit() {
	// This makes sure that the autonomous stops running when
	// teleop starts running. If you want the autonomous to
	// continue until interrupted by another command, remove
	// these lines or comment it out.
	if (autonomous.get() != nullptr)
		autonomous->Cancel();
}

void Robot::TeleopPeriodic() {
	Scheduler::GetInstance()->Run();

	/*
	if(ds->GetAlliance() == DriverStation::Alliance::kRed){
	    frameLights->ShowRGB(255, 0, 0);
	}else if(ds->GetAlliance() == DriverStation::Alliance::kBlue){
	    frameLights->ShowRGB(0, 0, 255);
	}else if(ds->GetAlliance() == DriverStation::Alliance::kInvalid){
	    frameLights->ShowRGB(255, 200, 0); //Yellow
	}
	*/
}

void Robot::TestPeriodic() {
	lw->Run();

	//frameLights->ShowRGB(255, 128, 0);
}

//for the lights
void Robot::inactivefade() {
	frameLights->WriteRegister(0, 2.5, 255, 0, 0); //Red
	frameLights->WriteRegister(1, 0.5, 255, 0, 128);
	frameLights->WriteRegister(2, 0.5, 255, 0, 255); //Red to Blue Transition
	frameLights->WriteRegister(3, 0.5, 128, 0, 255);
	frameLights->WriteRegister(4, 2.5, 0, 0, 255); //Blue
	frameLights->WriteRegister(5, 0.5, 128, 0, 255);
	frameLights->WriteRegister(6, 0.5, 255, 0, 255); //Blue to Red Transition
	frameLights->WriteRegister(7, 0.5, 255, 0, 128);
	frameLights->Cycle(0, 7);
}

void Robot::disableseqred() {
	frameLights->WriteRegister(0, 0.5, 255, 0, 0); //Fade red to off
	frameLights->WriteRegister(1, 0.5, 218, 0, 0);
	frameLights->WriteRegister(2, 0.5, 181, 0, 0);
	frameLights->WriteRegister(3, 0.5, 144, 0, 0);
	frameLights->WriteRegister(4, 0.5, 107, 0, 0);
	frameLights->WriteRegister(5, 0.5, 70, 0, 0);
	frameLights->WriteRegister(6, 0.5, 33, 0, 0);
	frameLights->WriteRegister(7, 1.5, 0, 0, 0);
	frameLights->Cycle(0, 7);
}

void Robot::disableseqblue() {
	frameLights->WriteRegister(0, 0.5, 0, 0, 255); //Fade blue to off
    frameLights->WriteRegister(1, 0.5, 0, 0, 218);
    frameLights->WriteRegister(2, 0.5, 0, 0, 181);
    frameLights->WriteRegister(3, 0.5, 0, 0, 144);
    frameLights->WriteRegister(4, 0.5, 0, 0, 107);
   	frameLights->WriteRegister(5, 0.5, 0, 0, 70);
   	frameLights->WriteRegister(6, 0.5, 0, 0, 33);
   	frameLights->WriteRegister(7, 1.5, 0, 0, 0);
   	frameLights->Cycle(0, 7);
}

START_ROBOT_CLASS(Robot);

