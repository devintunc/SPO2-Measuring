#pragma once

#ifndef ModeSpo2_H
#define ModeSpo2_H


#include "MAX30100_PulseOximeter.h"
#include"BaseMode.h"
#include"TouchController.h"


#define MeasurementRepeat 30
#define REPORTING_PERIOD_MS 400
#define UptadeSpo2_Frequency 400 

// Timer4B_flag is using for spo2 update timing. 

class ModeSpo2 : public BaseMode
{
public:
TouchController* _touchController;
uint32_t tsLastReport = 0;
bool spoActive = false;
PulseOximeter pox;
 
  void Run() override;
  void Complete() override;
  void Terminate() override;
  void SingleMeasure();
  void Update();

  ModeSpo2();
  virtual ~ModeSpo2();

private: 
  int repeat = 0;
};

#endif
