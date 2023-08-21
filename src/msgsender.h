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
#include <utility>

#include "ctrlstring.h"
#include "ctrlbool.h"
#include "ctrlglobaltimer.h"
#include "ctrlcolortheme.h"
#include "ctrlcontact.h"
#include "ctrlenvironment.h"
#include "ctrlambience.h"
#include "ctrlambientlight.h"
#include "ctrltracklayout.h"
#include "ctrlconnectivity.h"
#include "ctrlsetbrakevector.h"
#include "ctrlblocklock.h"

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

    INTERFACE_CONNECTIVITY_STATE_CHANGED,
    INTERFACE_CONTACT_TRIGGERED,
    INTERFACE_SET_BRAKE_VECTOR,
    //INTERFACE_SET_LOCO_SPEED,
    //INTERFACE_SET_LOCO_DIRECTION,
    //INTERFACE_SET_LOCO_FUNCTION,

    SYSTEM_SET_AUTOMATIC_MODE,
    SYSTEM_TOGGLE_AUTOMATIC_MODE,
    SYSTEM_TRIGGER_EMERGENCY_STOP,
    SYSTEM_RELEASE_EMERGENCY_STOP,

    SYSTEM_SET_STANDBY_MODE,
    SYSTEM_TOGGLE_STANDBY_MODE,
    SYSTEM_GET_HARDWARE_STATE,
    SYSTEM_HARDWARE_SHUTDOWN,
    SYSTEM_HARDWARE_RESET,

    LAYOUT_GET_LAYOUTS_REQ,
    LAYOUT_DEL_LAYOUT,
    LAYOUT_CREATE_LAYOUT_REQ,
    LAYOUT_UPDATE_LAYOUT,
    LAYOUT_UNLOCK_LAYOUT,
    LAYOUT_LOCK_LAYOUT,

    LAYOUT_GET_LAYOUT_REQ,
    LAYOUT_GET_LAYOUT_READ_ONLY_REQ,

    CONTROL_GET_CONTACT_LIST_REQ,
    CONTROL_GET_SWITCH_STAND_LIST_REQ,
    CONTROL_GET_TRAIN_LIST_REQ,
    CONTROL_LOCK_BLOCK,
    CONTROL_LOCK_BLOCK_WAITING,
    CONTROL_UNLOCK_BLOCK
};

class MsgSender {
    public:
        explicit MsgSender(EndpointPtr endpoint) : endpoint{std::move(endpoint)} {
        }

        virtual ~MsgSender() = default;

        void sendActiveMessage();
        void setActiveMessage(MessageType cmd, const std::string &name, std::uint32_t grpId, std::uint32_t msgId, Gtk::ScrolledWindow &container);

    protected:
        std::string activeMessageName{};
        MessageType activeMessage{};
        EndpointPtr endpoint;

        std::uint32_t activeGrpId{};
        std::uint32_t activeMsgId{};

        CtrlString         m_CtrlString;
        CtrlBool           m_CtrlBool;
        CtrlGlobalTimer    m_CtrlGlobalTimer;
        CtrlColorTheme     m_CtrlColorTheme;
        CtrlEnvironment    m_CtrlEnvironment;
        CtrlAmbience       m_CtrlAmbience;
        CtrlAmbientLight   m_CtrlAmbientLight;
        CtrlConnectivity   m_CtrlConnectivity;
        CtrlTrackLayout    m_CtrlTrackLayout;
        CtrlContact        m_CtrlContact;
        CtrlSetBrakeVector m_CtrlSetBrakeVector;
        CtrlBlockLock      m_CtrlBlockLock;
};
