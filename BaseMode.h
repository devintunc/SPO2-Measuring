#pragma once
#ifndef BaseMode_H
#define BaseMode_H

#include "BaseDataSndr.h"
#include"Timer4.h"
#include "Display.h"
#include "NonBlocking_Observer.h"
#include "EEPROM.h"

class BaseMode : public BaseDataSndr, public Timer4
{
public:
  // DataCount is produced data count from the mode measurements. For example SPO2 mode has 2 data that are pulse and O2 rate.
  byte DataCount;

  String Dat0_Name;
  String Dat1_Name;
  String Dat0_Unit;
  String Dat1_Unit;

  int Data_OFFSET_X = 35;
  int Data_Name_OFFSET_X; 

  bool isEEPsubs;
  int EEPROM_Adress;
  virtual void Run() = 0;
  virtual void Complete() = 0;
  virtual void Terminate() = 0;
  
  /*
  Touch Controller does not send command messages to the mode's command handler directly. It sends to Mode Manager.
  This function calls in Mode Manager in MessageHandler. 
  This function should be implement with isCommandHandable function.
  The isCommandHandable  return value using in Mode Manager to decide to call only this function or not.
  If returns true ModeManager does not run own command response.
  Mode Manager's decide mechanism shoul be consider when the function implement.
  This function should be restricted at least a command or modes should include exit button.
  Modes have a permisson to use only STouch in default. 
  When the function did not implemented by mode, it does not block Touch Controller command.
  Example implementation :
  void ModeWeightCalibration::TouchPanel_CommandHandler(Command cmd, Sender id)
{
    if (id == ID)
    {
        blockTouchPanelCommand=true;

        if (cmd == Command::STouch)
        {
           do something...
        }
        if (cmd == Command::UpSlide)//when id==ID but command is Upslide blockTouchPanelCommand is settling false to not block.
        {
            blockTouchPanelCommand = false;
        }      
    }
    else
        blockTouchPanelCommand = false;
    
}

  */
  virtual void TouchPanel_CommandHandler(Command cmd,Sender id);
  bool isCommandHandable();
  void CompletedTimeOut_Handler(int ModeOrder);



  BaseMode(/* args */);
  virtual ~BaseMode();

protected: 
  // Activate the Timer4_A if the mode has timeout. returnSelectionView attached the Timer4_A observer in default.
  void CompletedTimeOut();
  bool hasTimeOut;
  float completed_timeout; // Timeout in seconds.

  bool blockTouchPanelCommand;//Default false.
  bool isActive;
};

#endif