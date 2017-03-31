/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   baseHandler.cpp
 * Author: stefan
 *
 * Created on March 31, 2017, 10:00 PM
 */

#include "systemhandler.h"
#include "helper.h"

void SystemHandler::handle(moba::Message::MessageType cmd) {
    switch(cmd) {
        case moba::Message::MT_EMERGENCY_STOP: {
            this->sysHandler.sendEmergencyStop();
            break;
        }

        case moba::Message::MT_EMERGENCY_STOP_CLEARING: {
            this->sysHandler.sendEmergencyStopClearing();
            break;
        }

        case moba::Message::MT_GET_HARDWARE_STATE: {
            this->sysHandler.sendGetHardwareState();
            break;
        }

        case moba::Message::MT_SET_HARDWARE_STATE: {
            moba::SystemHandler::HardwareState hs = moba::SystemHandler::HS_ERROR;
            std::string s;
            getData("ready, error, standby, power off [r|e|s|p]", s);
            if(s == "r") {
                hs = moba::SystemHandler::HS_READY;
            } else if(s == "s") {
                hs = moba::SystemHandler::HS_STANDBY;
            } else if(s == "p") {
                hs = moba::SystemHandler::HS_POWER_OFF;
            }
            this->sysHandler.sendSetHardwareState(hs);
            break;
        }

        case moba::Message::MT_HARDWARE_SHUTDOWN: {
            this->sysHandler.sendHardwareShutdown();
            break;
        }

        case moba::Message::MT_HARDWARE_RESET: {
            this->sysHandler.sendHardwareReset();
            break;
        }

        case moba::Message::MT_HARDWARE_SWITCH_STANDBY: {
            this->sysHandler.sendHardwareSwitchStandby();
            break;
        }
    }
}