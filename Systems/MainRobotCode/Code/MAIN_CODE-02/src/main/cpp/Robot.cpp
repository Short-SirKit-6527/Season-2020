/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "Robot.h"

#include <iostream>

#include <frc/smartdashboard/SmartDashboard.h>

void Robot::RobotInit() {
  //m_chooser.SetDefaultOption(kAutoNameDefault, kAutoNameDefault);
  //m_chooser.AddOption(kAutoNameCustom, kAutoNameCustom);
  m_chooser.SetDefaultOption(kAutoNames[0], 0);
  for (int autoID = 1; autoID < AUTOCOUNT; autoID++){
    m_chooser.SetDefaultOption(kAutoNames[autoID], autoID);
  }
  frc::SmartDashboard::PutData("Auto Modes", &m_chooser);

  m_gyro.Calibrate();

  m_leftEncoder.SetDistancePerPulse(2 * wpi::math::pi * kWheelRadius /
                                      kEncoderResolution);
  m_rightEncoder.SetDistancePerPulse(2 * wpi::math::pi * kWheelRadius /
                                      kEncoderResolution);

  auto ntinst = nt::NetworkTableInstance::GetDefault();
  auto positonTable = ntinst.GetTable("RoboRadar");
  xPos = positonTable->GetEntry("posX");
  yPos = positonTable->GetEntry("posY");
  rPos = positonTable->GetEntry("posR");
  sPos = positonTable->GetEntry("posRSin");
  cPos = positonTable->GetEntry("posRCos");
  tPos = positonTable->GetEntry("posRTan");
}

/**
 * This function is called every robot packet, no matter the mode. Use
 * this for items like diagnostics that you want ran during disabled,
 * autonomous, teleoperated and test.
 *
 * <p> This runs after the mode specific periodic functions, but before
 * LiveWindow and SmartDashboard integrated updating.
 */
void Robot::RobotPeriodic() {
  //printf("%f", m_leftEncoder.GetDistance());
  // Update Odometry (robots memorized position)
  frc::Pose2d position = m_driveOdometry.Update(frc::Rotation2d(-units::angle::degree_t(m_gyro.GetAngle())), units::length::meter_t(-m_leftEncoder.GetDistance()), units::length::meter_t(m_rightEncoder.GetDistance()));
  frc::Translation2d positonxy = position.Translation();
  frc::Rotation2d positionr = position.Rotation();

  units::length::meter_t mx = positonxy.Y();
  units::length::meter_t my = positonxy.X();
  units::length::inch_t x = mx;
  units::length::inch_t y = my;
  xPos.SetDouble(units::unit_cast<double>(x));
  yPos.SetDouble(units::unit_cast<double>(y));

  if (kSendRadians) rPos.SetDouble(-units::unit_cast<double>(-positionr.Radians()));
  else rPos.SetDouble(-units::unit_cast<double>(-positionr.Degrees()));
  sPos.SetDouble(-positionr.Sin());
  cPos.SetDouble(-positionr.Cos());
  tPos.SetDouble(positionr.Tan());

  
}

/**
 * This autonomous (along with the chooser code above) shows how to select
 * between different autonomous modes using the dashboard. The sendable chooser
 * code works with the Java SmartDashboard. If you prefer the LabVIEW Dashboard,
 * remove all of the chooser code and uncomment the GetString line to get the
 * auto name from the text box below the Gyro.
 *
 * You can add additional auto modes by adding additional comparisons to the
 * if-else structure below with additional strings. If using the SendableChooser
 * make sure to add them to the chooser code above as well.
 */
void Robot::AutonomousInit() {
  m_autoSelected = m_chooser.GetSelected();
  // m_autoSelected = SmartDashboard::GetString("Auto Selector",
  //     kAutoNameDefault);
  std::cout << "Auto selected: " << std::to_string(m_autoSelected)  << " - " << kAutoNames[m_autoSelected] << std::endl;

  resetSystems();

  switch (m_autoSelected)
  {
  case 0: // Line Scoot
    break;
  case 1: // Shoot 3
    break;
  }
  m_driveMode = 0;
}

void Robot::AutonomousPeriodic() {
  switch (m_autoSelected)
  {
  case 0: // Line Scoot
    break;
  case 1: // Shoot 3
    break;
  }
}

void Robot::TeleopInit() {
  m_driveMode = 3;
}

void Robot::TeleopPeriodic() {
  if (m_driver0.GetStartButtonPressed()){
    if (m_driveMode == 3){
      m_driveMode = 1;
    }
    else {
      m_driveMode = 3;
    }
  }

  m_driveMode = 3;
  double dx = 0;
  double dy = 0;
  //double dz = 0;

  switch(m_driveMode){
    case 0:
      dx = 0;
      dy = 0;
      break;
    case 1:
      dx = m_driver0.GetX(frc::GenericHID::kLeftHand);
      dy = m_driver0.GetY(frc::GenericHID::kLeftHand);
      break;
    case 2:
      dx = m_driver1.GetX();
      dy = m_driver1.GetY();
      break;
    case 3:
      dx = m_driver0.GetX(frc::GenericHID::kLeftHand) +  m_driver1.GetX();
      dy = m_driver0.GetY(frc::GenericHID::kLeftHand) +  m_driver1.GetY();
      break;
    default:
      m_driveMode = 3;
      dx = 0;
      dy = 0;
  }
  dx = m_driver0.GetX(frc::GenericHID::kLeftHand);
  dy = m_driver0.GetY(frc::GenericHID::kLeftHand);
  for (int boostID = 0; boostID < boostCount; boostID++){
    if (boostID + 1 < boostCount){
      if (m_driver0.GetRawButton(kDriveScaleBoostsButton[boostID][0]) || m_driver1.GetRawButton(kDriveScaleBoostsButton[boostID][1])){
        dx *= kDriveScaleBoostsX[boostID];
        dy *= kDriveScaleBoostsY[boostID];
        boostID = boostCount;
      }
    }
    else {
      dx *= kDriveScaleBoostsX[boostID];
      dy *= kDriveScaleBoostsY[boostID];
    }
  }
  dx = m_driver0.GetX(frc::GenericHID::kLeftHand) / 2;
  dy = m_driver0.GetY(frc::GenericHID::kLeftHand) / 2;
  m_driveSystem.ArcadeDrive(dy, dx);
}

void Robot::TestInit() {
  m_driveMode = 3;

  frc::Color detectedColor = m_colorSensor.GetColor();
  double IR = m_colorSensor.GetIR();
  uint32_t proximity = m_colorSensor.GetProximity();

  frc::SmartDashboard::PutNumber("Red", detectedColor.red);
  frc::SmartDashboard::PutNumber("Green", detectedColor.green);
  frc::SmartDashboard::PutNumber("Blue", detectedColor.blue);
  frc::SmartDashboard::PutNumber("IR", IR);
  frc::SmartDashboard::PutNumber("Proximity", proximity);

}

//Pneumatics
void Robot::TestPeriodic() {
  //pusherDouble.Set(frc::DoubleSolenoid::Value::kOff);
  if (m_driver1.GetRawButton(3)){
    lifterDouble.Set(frc::DoubleSolenoid::Value::kForward);
  }
  if (m_driver1.GetRawButton(5)){
    lifterDouble.Set(frc::DoubleSolenoid::Value::kReverse);
  }

  if (m_driver1.GetRawButton(1)){
    pusherDouble.Set(frc::DoubleSolenoid::Value::kForward);
  } else {
    pusherDouble.Set(frc::DoubleSolenoid::Value::kReverse);
  }
  double l = m_drivePidL.Calculate(m_leftEncoder.GetRate(), m_driver0.GetY(frc::GenericHID::kLeftHand));
  double r = m_drivePidR.Calculate(m_rightEncoder.GetRate(), m_driver0.GetY(frc::GenericHID::kRightHand));
  m_driveSystem.TankDrive(l, r);
}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif

void Robot::resetSystems() {
  m_driveOdometry.ResetPosition(frc::Pose2d(), frc::Rotation2d());
  m_leftEncoder.Reset();
  m_rightEncoder.Reset();
}
