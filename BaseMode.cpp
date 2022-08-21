#include "BaseMode.h"
#include "Arduino.h"
#include "NonBlocking.h"

BaseMode::BaseMode(/* args */)
{
    blockTouchPanelCommand = false;  
    isEEPsubs=false;     
}

BaseMode::~BaseMode()
{
}
void BaseMode::TouchPanel_CommandHandler(Command cmd, Sender id)
{
    Serial.print("blockTouchPanelCommand");
    Serial.println(blockTouchPanelCommand);
}
bool BaseMode::isCommandHandable()
{
    return blockTouchPanelCommand;
}
void BaseMode::CompletedTimeOut_Handler(int ModeOrder)
{
    if (hasTimeOut)
    {
        Send(CreateMessage(Sender::ModeManager, String(ModeOrder), ""), (BaseDataReceiver *)(&Display::GetInstance()));
        stopTimer_A();
    }
}
void BaseMode::CompletedTimeOut()
{
    if (hasTimeOut)
    {
        setupTimerValue_A(completed_timeout, false);
        startTimer_A();
    }
}

