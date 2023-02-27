/**
 * @file power_port.cpp
 * @brief Class to emulate electrical power port
 */

#include "power_port.hpp"

#include <cfloat>
#include <library/initialize/initialize_file_access.hpp>

PowerPort::PowerPort() : kPortId(-1), current_limit_A_(10.0), minimum_voltage_V_(3.3), assumed_power_consumption_W_(0.0) {
  is_on_ = true;  // power on to work the component
  Initialize();
}

PowerPort::PowerPort(int port_id, double current_Limit)
    : kPortId(port_id), current_limit_A_(current_Limit), minimum_voltage_V_(3.3), assumed_power_consumption_W_(0.0) {
  Initialize();
}

PowerPort::PowerPort(int port_id, double current_Limit, double minimum_voltage, double assumed_power_consumption)
    : kPortId(port_id),
      current_limit_A_(current_Limit),
      minimum_voltage_V_(minimum_voltage),
      assumed_power_consumption_W_(assumed_power_consumption) {
  Initialize();
}

PowerPort::~PowerPort() {}

void PowerPort::Initialize(void) {
  voltage_V_ = 0.0f;
  current_consumption_A_ = 0.0f;
}

bool PowerPort::Update(void) {
  // switching
  if (voltage_V_ >= (minimum_voltage_V_ - DBL_EPSILON)) {
    is_on_ = true;
    current_consumption_A_ = assumed_power_consumption_W_ / voltage_V_;
  } else {
    current_consumption_A_ = 0.0;
    is_on_ = false;
  }
  // over current protection
  if (current_consumption_A_ >= (current_limit_A_ - DBL_EPSILON)) {
    current_consumption_A_ = 0.0;
    voltage_V_ = 0.0;
    is_on_ = false;
  }
  return is_on_;
}

bool PowerPort::SetVoltage(const double voltage) {
  voltage_V_ = voltage;
  Update();
  return is_on_;
}

void PowerPort::SubtractAssumedPowerConsumption(const double power) {
  assumed_power_consumption_W_ -= power;
  if (assumed_power_consumption_W_ < 0.0) assumed_power_consumption_W_ = 0.0;
  return;
}

void PowerPort::InitializeWithInitializeFile(const std::string file_name) {
  IniAccess initialize_file(file_name);
  const std::string section_name = "POWER_PORT";

  double minimum_voltage = initialize_file.ReadDouble(section_name.c_str(), "minimum_voltage_V");
  this->SetMinimumVoltage(minimum_voltage);
  double assumed_power_consumption = initialize_file.ReadDouble(section_name.c_str(), "assumed_power_consumption_W");
  this->SetAssumedPowerConsumption(assumed_power_consumption);
}
