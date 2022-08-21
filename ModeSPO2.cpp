#include <Wire.h>
//#include"ModeSPO2.h"

#include "ModeManager.h"

ModeManager *_modeManager;

ModeSpo2::ModeSpo2()
{
  hasTimeOut = true;
  completed_timeout = 2;

  DataCount = 2;
  Data_Name_OFFSET_X=37;
  Dat0_Name = "Pulse";
  Dat0_Unit = "bpm";
  Dat1_Name = "O2 Rate";
  Dat1_Unit = "%";

  _touchController = (TouchController::GetInstance());
  if (!pox.begin())
  {
    Serial.println("FAILED");
    for (;;)
      ;
  }
  else
  {
    Serial.println("SUCCESS");
  }

  ID = Sender::SPO2;
  Serial.println("ModeSpo2 Constructed");
  //setupTimerValue_B(UptadeSpo2_Frequency, true);
}
ModeSpo2::~ModeSpo2()
{
  Serial.println("ModeSpo2 destructed");
  // disableTimer();
}

void ModeSpo2::Run()
{
  _touchController->BlockScanF = true; // BlockScanF setting false when ModeSpo2 complete task.
  pox.begin();                         // when I2C interrupted by another device pox should be begin again!!!

  spoActive = true;
  //startTimer_B();
  delay(100);
}

void ModeSpo2::Complete()
{
  spoActive = false;
  _touchController->BlockScanF = false;
  repeat = 0;
  pox.shutdown();
  //stopTimer_B();
  
  Send(CreateMessage("info", "Measurement,done!"), (BaseDataReceiver *)&Display::GetInstance());
  CompletedTimeOut();
}
void ModeSpo2::Terminate()
{
  Serial.print("SPO2 Mode Terminated.");
}
void onBeatDetected()
{
  Serial.println("Beat!");
}
void ModeSpo2::SingleMeasure()
{
  Send(CreateMessage(String(pox.getHeartRate()), String(pox.getSpO2())), (BaseDataReceiver *)(&Display::GetInstance()));
}
void ModeSpo2::Update()
{

  // Activity barrier
  if (!spoActive)
    return;

  // spoUF driven by timer4.
/*
  if (Timer4B_flag)
  {

    pox.update();
    Timer4B_flag = false;
  }
*/
pox.update();
  if (millis() - tsLastReport > REPORTING_PERIOD_MS)
  {
    Serial.println(repeat);
    SingleMeasure();
    repeat++;

    if (repeat == MeasurementRepeat)
      Complete();
    tsLastReport = millis();
  }
}
