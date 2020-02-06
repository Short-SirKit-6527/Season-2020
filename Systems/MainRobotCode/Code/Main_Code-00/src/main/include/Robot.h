/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <string>

#include <frc/TimedRobot.h>
#include <frc/smartdashboard/SendableChooser.h>
#include <frc/XboxController.h>
#include <frc/VictorSP.h>
#include <frc/drive/DifferentialDrive.h>
#include <frc/kinematics/DifferentialDriveKinematics.h>
#include <frc/kinematics/DifferentialDriveOdometry.h>
#include <frc/Joystick.h>
#include <frc/ADXRS450_Gyro.h>
#include <frc/AnalogPotentiometer.h>
#include <frc/geometry/Rotation2d.h>
#include <frc/Encoder.h>

#include "ctre/Phoenix.h"
#include "rev/ColorSensorV3.h"

#include "units.h"

#include <wpi/math>

class Robot : public frc::TimedRobot {
 public:
  void RobotInit() override;
  void RobotPeriodic() override;
  void AutonomousInit() override;
  void AutonomousPeriodic() override;
  void TeleopInit() override;
  void TeleopPeriodic() override;
  void TestInit() override;
  void TestPeriodic() override;

 private:
  
  // DRIVER INPUT CONFIG
  frc::XboxController m_driver0{0};
  frc::Joystick m_driver1{1};
  int m_driveMode = 3; //0 = No Drivers, 1 = Driver 0, 2 = Driver 1, 3 = Drivers 0 and 1
  #define boostCount 3
  static const int driverCount = 2;
  const int kDriveScaleBoostsButton[boostCount-1][driverCount] = {{0, 0}, {1, 1}};
  const double kDriveScaleBoostsX[boostCount] = { 3/3, 2/3, 1/3};
  const double kDriveScaleBoostsY[boostCount] = { 3/3, 2/3, 1/3};

  // DRIVE BASE CONFIG
  frc::VictorSP m_driveBaseL{0};
  frc::VictorSP m_driveBaseR{1};
  frc::DifferentialDrive m_driveSystem{m_driveBaseL, m_driveBaseR};
  frc::DifferentialDriveKinematics m_driveKinematics{27_in}; //distance between wheels
  frc::DifferentialDriveOdometry m_driveOdometry{frc::Rotation2d()};

  // SENSOR CONFIG
  frc::ADXRS450_Gyro m_gyro{frc::SPI::Port::kOnboardCS0};
  frc::AnalogPotentiometer m_frontUltrasonic{0, 201.378, 0};
  static constexpr auto i2cPort = frc::I2C::Port::kOnboard;
  rev::ColorSensorV3 m_colorSensor{i2cPort};
  static constexpr double kWheelRadius = 0.0508;  // meters
  static constexpr int kEncoderResolution = 4096;
  frc::Encoder m_leftEncoder{0, 1};
  frc::Encoder m_rightEncoder{2, 3};

  // AUTONOMOUS CONFIG
  frc::SendableChooser<int> m_chooser;
  int m_autoSelected;
  #define AUTOCOUNT 2
  const std::string kAutoNames[AUTOCOUNT] = {"Line Scoot", "Shoot 3"};
};