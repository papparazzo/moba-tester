/*
 *  Project:    moba-tester
 *
 *  Copyright (C) 2016 Stefan Paproth <pappi-@gmx.de>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/agpl.txt>.
 *
 */

#include "msgsender.h"
#include <moba/jsonabstractitem.h>

MsgSender::MsgSender(moba::MsgEndpointPtr msgep) : msgep{msgep} {
}

MsgSender::~MsgSender() {
}

void MsgSender::sendActiveMessage() {

    switch(activeMessage) {
        case moba::Message::MT_RESET_CLIENT:
        case moba::Message::MT_SELF_TESTING_CLIENT:
        case moba::Message::MT_DEL_LAYOUT:
        case moba::Message::MT_UNLOCK_LAYOUT:
        case moba::Message::MT_GET_LAYOUT_REQ:
            msgep->sendMsg(activeMessage, m_CtrlString.get_jsonInt());
            break;

        case moba::Message::MT_ECHO_REQ:
            msgep->sendMsg(activeMessage, m_CtrlString.get_text());
            break;

        case moba::Message::MT_SET_AUTO_MODE:
            msgep->sendMsg(activeMessage, m_CtrlBool.get_value());
            break;

        case moba::Message::MT_SET_GLOBAL_TIMER:
            msgep->sendMsg(activeMessage, m_CtrlGlobalTimer.get_value());
            break;

        case moba::Message::MT_SET_COLOR_THEME:
            msgep->sendMsg(activeMessage, m_CtrlColorTheme.get_value());
            break;

        default:
            msgep->sendMsg(activeMessage);
            break;
    }
}

void MsgSender::setActiveMessage(moba::Message::MessageType cmd, Gtk::ScrolledWindow &container) {
    container.remove_with_viewport();
    activeMessage = cmd;

    switch(cmd) {
        case moba::Message::MT_RESET_CLIENT:
        case moba::Message::MT_SELF_TESTING_CLIENT:
        case moba::Message::MT_DEL_LAYOUT:
        case moba::Message::MT_UNLOCK_LAYOUT:
        case moba::Message::MT_GET_LAYOUT_REQ:
            m_CtrlString.init("Id", container);
            return;

        case moba::Message::MT_ECHO_REQ:
            m_CtrlString.init("Data", container);
            return;

        case moba::Message::MT_SET_AUTO_MODE:
            m_CtrlBool.init("AutoMode", container);
            break;

        case moba::Message::MT_SET_GLOBAL_TIMER:
            m_CtrlGlobalTimer.init(container);
            break;

        case moba::Message::MT_SET_COLOR_THEME:
            m_CtrlColorTheme.init(container);
            break;

        //EnvironmentData
        case moba::Message::MT_SET_ENVIRONMENT:

        //AmbienceData
        case moba::Message::MT_SET_AMBIENCE:

        //AmbientLightData
        case moba::Message::MT_SET_AMBIENT_LIGHT:

        //HardwareState
        case moba::Message::MT_SET_HARDWARE_STATE:

        //TrackLayoutData
        case moba::Message::MT_CREATE_LAYOUT_REQ:
        case moba::Message::MT_UPDATE_LAYOUT:
            return;
    }
}

/*

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
            this->sysHandler.sendSetHardwareState(hs);*/

/*
         case moba::Message::MT_CREATE_LAYOUT_REQ: {
            std::string name;
            std::string desc;
            getData("name", name);
            getData("desc", desc);
            this->tloHandler.sendCreateLayout(name, desc);
            break;
        }

        case moba::Message::MT_UPDATE_LAYOUT: {
            int id = 0;
            std::string name;
            std::string desc;
            getData("LayoutId", id);
            getData("name", name);
            getData("desc", desc);
            this->tloHandler.sendUpdateLayout(id, name, desc);
            break;
        }


 * switch(cmd) {

        case moba::Message::MT_SET_GLOBAL_TIMER: {
            std::string date = "Sa 10:00";  // FIXME getString("DD hh:mm (e.g. Sa 10:00)");
            unsigned int intervall = 0;
            getData("Seconds (modulo 5)", intervall);
            unsigned int multiplicator = 0;
            getData("Multiplicator (realtime * multiplicator = modeltime)", multiplicator);
            this->envHandler.sendSetGlobalTimer(date, intervall, multiplicator);
            break;
        }


        case moba::Message::MT_SET_ENVIRONMENT:
            this->envHandler.sendSetEnvironment(
                getSwitchState("thunder"),
                getSwitchState("wind"),
                getSwitchState("rain"),
                getSwitchState("environmentSound"),
                getSwitchState("aux01"),
                getSwitchState("aux02"),
                getSwitchState("aux03")
            );
            break;


        case moba::Message::MT_SET_AMBIENCE: {
            this->envHandler.sendSetAmbience(
                getToggleState("curtainUp"),
                getToggleState("mainLightOn")
            );
            break;
        }

        case moba::Message::MT_SET_AMBIENT_LIGHT: {
            std::vector<moba::EnvironmentHandler::AmbientLightData> v;
            moba::EnvironmentHandler::AmbientLightData ald;
            for(int i = 0; i < 3; ++i) {
                getData("red", ald.red);
                getData("blue", ald.blue);
                getData("white", ald.white);
                v.push_back(ald);
            }
            this->envHandler.sendSetAmbientLight(v);
            break;
        }

        case moba::Message::MT_SET_COLOR_THEME: {
            this->envHandler.sendSetColorTheme(
                getString("dimTime [hh:mm]"),
                getString("brightTime [hh:mm]"),
                getThreeState("condition")
            );
            break;
        }
    }
 *
 *



moba::JsonToggleState::ToggleState getToggleState(const std::string &s) {
    std::string str;
    getData(s + "[on|off|unset]", str);
    if(str == "on") {
        return moba::JsonToggleState::ON;
    }
    if(str == "off") {
        return moba::JsonToggleState::OFF;
    }
    return moba::JsonToggleState::UNSET;
}

moba::JsonSwitch::Switch getSwitchState(const std::string &s) {
    std::string str;
    getData(s + "[on|off|auto|unset|trigger]", str);
    if(str == "on") {
        return moba::JsonSwitch::ON;
    }
    if(str == "off") {
        return moba::JsonSwitch::OFF;
    }
    if(str == "auto") {
        return moba::JsonSwitch::AUTO;
    }
    if(str == "trigger") {
        return moba::JsonSwitch::TRIGGER;
    }
    return moba::JsonSwitch::UNSET;
}

 *
 */