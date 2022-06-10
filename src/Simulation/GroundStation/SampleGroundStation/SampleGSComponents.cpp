#include "SampleGSComponents.h"

#include <Interface/InitInput/IniAccess.h>

SampleGSComponents::SampleGSComponents(const SimulationConfig* config) : config_(config) {
  IniAccess iniAccess = IniAccess(config_->ini_base_fname_);

  std::string ant_ini_path = iniAccess.ReadString("COMPONENTS_FILE", "ant_gs_file");
  config_->main_logger_->CopyFileToLogDir(ant_ini_path);
  antenna_ = new Antenna(InitAntenna(2, ant_ini_path));
  std::string gscalculator_ini_path = iniAccess.ReadString("COMPONENTS_FILE", "gscalculator_file");
  config_->main_logger_->CopyFileToLogDir(gscalculator_ini_path);
  gs_calculator_ = new GScalculator(InitGScalculator(gscalculator_ini_path));
}

SampleGSComponents::~SampleGSComponents() {
  delete antenna_;
  delete gs_calculator_;
}

void SampleGSComponents::CompoLogSetUp(Logger& logger) {
  // logger.AddLoggable(ant_);
  logger.AddLoggable(gs_calculator_);
}
