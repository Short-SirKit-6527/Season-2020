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
#include <frc/SpeedControllerGroup.h>
#include <frc/drive/DifferentialDrive.h>
#include <frc/kinematics/DifferentialDriveKinematics.h>
#include <frc/kinematics/DifferentialDriveOdometry.h>
#include <frc/Joystick.h>
#include <frc/ADXRS450_Gyro.h>
#include <frc/AnalogPotentiometer.h>
#include <frc/geometry/Rotation2d.h>
#include <frc/Encoder.h>

#include <frc/controller/PidController.h>

#include "networktables/NetworkTable.h"
#include "networktables/NetworkTableEntry.h"
#include "networktables/NetworkTableInstance.h"

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
  void resetSystems();

  // NETWORK TABLE CONFIG
  const bool kSendRadians = true;
  nt::NetworkTableEntry xPos;
  nt::NetworkTableEntry yPos;
  nt::NetworkTableEntry rPos;
  nt::NetworkTableEntry sPos;
  nt::NetworkTableEntry cPos;
  nt::NetworkTableEntry tPos;
  
  // DRIVER INPUT CONFIG
  frc::XboxController m_driver0{0};
  frc::Joystick m_driver1{1};
  int m_driveMode = 3; //0 = No Drivers, 1 = Driver 0, 2 = Driver 1, 3 = Drivers 0 and 1
  #define boostCount 3
  static const int driverCount = 2;
  const int kDriveScaleBoostsButton[boostCount-1][driverCount] = {{1, 1}, {2, 2}};
  const double kDriveScaleBoostsX[boostCount] = { 3/3, 2/3, 1/2};
  const double kDriveScaleBoostsY[boostCount] = { 3/3, 2/3, 1/2};

  // DRIVE BASE CONFIG
  WPI_VictorSPX m_driveBaseL0{2};
  WPI_VictorSPX m_driveBaseL1{3};
  frc::SpeedControllerGroup m_driveBaseL{m_driveBaseL0, m_driveBaseL1};
  WPI_VictorSPX m_driveBaseR0{0};
  WPI_VictorSPX m_driveBaseR1{1};
  double kDriveP = 0.5;
  double kDriveI = 0;
  double kDriveD = 0;
  frc2::PIDController m_drivePidL{kDriveP, kDriveI, kDriveD};
  frc2::PIDController m_drivePidR{kDriveP, kDriveI, kDriveD};
  frc::SpeedControllerGroup m_driveBaseR{m_driveBaseR0, m_driveBaseR1};
  frc::DifferentialDrive m_driveSystem{m_driveBaseL, m_driveBaseR};
  frc::DifferentialDriveKinematics m_driveKinematics{27_in}; //distance between wheels
  frc::DifferentialDriveOdometry m_driveOdometry{frc::Rotation2d()};

  // SENSOR CONFIG
  frc::ADXRS450_Gyro m_gyro{frc::SPI::Port::kOnboardCS0};
  frc::AnalogPotentiometer m_frontUltrasonic{0, 201.378, 0};
  static constexpr auto i2cPort = frc::I2C::Port::kOnboard;
  rev::ColorSensorV3 m_colorSensor{i2cPort};
  static constexpr double kWheelRadius = 0.0762;  
  static constexpr int kEncoderResolution = 360; //360 cycles per revolution, 1440 pulses per revolution
  frc::Encoder m_leftEncoder{2, 3};
  frc::Encoder m_rightEncoder{0, 1};

  // AUTONOMOUS CONFIG
  frc::SendableChooser<int> m_chooser;
  int m_autoSelected;
  #define AUTOCOUNT 2
  const std::string kAutoNames[AUTOCOUNT] = {"Line Scoot", "Shoot 3"};
};