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

#pragma once

#include "moba/endpoint.h"

#include <gtkmm.h>
#include <string>

#include "ctrlstring.h"
#include "ctrlbool.h"
#include "ctrlglobaltimer.h"
#include "ctrlcolortheme.h"
#include "ctrlenvironment.h"
#include "ctrlambience.h"
#include "ctrlambientlight.h"
#include "ctrltracklayout.h"
#include "ctrlconnectivity.h"

enum class MessageType {
    UNSET,

    CLIENT_VOID,
    CLIENT_ECHO_REQ,
    CLIENT_START,
    CLIENT_CLOSE,

    SERVER_RESET_CLIENT,
    SERVER_INFO_REQ,
    SERVER_CON_CLIENTS_REQ,
    SERVER_SELF_TESTING_CLIENT,

    TIMER_GET_GLOBAL_TIMER,
    TIMER_SET_GLOBAL_TIMER,
    TIMER_GET_COLOR_THEME,
    TIMER_SET_COLOR_THEME,

    ENVIRONMENT_GET_ENVIRONMENT,
    ENVIRONMENT_SET_ENVIRONMENT,
    ENVIRONMENT_GET_AMBIENCE,
    ENVIRONMENT_SET_AMBIENCE,
    ENVIRONMENT_GET_AMBIENT_LIGHT,
    ENVIRONMENT_SET_AMBIENT_LIGHT,

    INTERFACE_SET_CONNECTIVITY,

    SYSTEM_SET_AUTOMATIC_MODE,
    SYSTEM_SET_EMERGENCY_STOP,
    SYSTEM_SET_STANDBY_MODE,
    SYSTEM_GET_HARDWARE_STATE,
    SYSTEM_HARDWARE_SHUTDOWN,
    SYSTEM_HARDWARE_RESET,

    LAYOUTS_GET_LAYOUTS_REQ,
    LAYOUTS_DEL_LAYOUT,
    LAYOUTS_CREATE_LAYOUT_REQ,
    LAYOUTS_UPDATE_LAYOUT,
    LAYOUTS_UNLOCK_LAYOUT,

    LAYOUT_GET_LAYOUT_REQ,
};

class MsgSender {
    public:
        MsgSender(EndpointPtr msgep) : msgep{msgep} {
        }

        virtual ~MsgSender() {
        }

        void sendActiveMessage();
        void setActiveMessage(MessageType cmd, const std::string &name, Gtk::ScrolledWindow &container);

    protected:
        std::string activeMessageName;
        MessageType activeMessage;
        EndpointPtr msgep;

        CtrlString        m_CtrlString;
        CtrlBool          m_CtrlBool;
        CtrlGlobalTimer   m_CtrlGlobalTimer;
        CtrlColorTheme    m_CtrlColorTheme;
        CtrlEnvironment   m_CtrlEnvironment;
        CtrlAmbience      m_CtrlAmbience;
        CtrlAmbientLight  m_CtrlAmbientLight;
        CtrlConnectivity  m_CtrlConnectivity;
        CtrlTrackLayout   m_CtrlTrackLayout;
};
