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
#include "moba/basemessage.h"

void MsgSender::sendActiveMessage() {
    Message message{activeGrpId, activeMsgId};

    switch(activeMessage) {
        case MessageType::SERVER_RESET_CLIENT:
        case MessageType::SERVER_SELF_TESTING_CLIENT:
        case MessageType::LAYOUT_DEL_LAYOUT:
        case MessageType::LAYOUT_UNLOCK_LAYOUT:
        case MessageType::LAYOUT_LOCK_LAYOUT:
        case MessageType::LAYOUT_GET_LAYOUT_REQ:
        case MessageType::LAYOUT_GET_LAYOUT_READ_ONLY_REQ:
            message.data.SetInt(m_CtrlString.get_integer());
            break;

        case MessageType::SYSTEM_SET_AUTOMATIC_MODE:
        case MessageType::SYSTEM_SET_EMERGENCY_STOP:
        case MessageType::SYSTEM_SET_STANDBY_MODE:
            message.data.SetBool(m_CtrlBool.get_value());
            break;

        case MessageType::CLIENT_ECHO_REQ: {
            auto txt = m_CtrlString.get_text();
            message.data.SetString(txt.c_str(), txt.length(), message.data.GetAllocator());
            break;
        }

        case MessageType::TIMER_SET_GLOBAL_TIMER:
            m_CtrlGlobalTimer.get_value(message.data);
            break;

        case MessageType::TIMER_SET_COLOR_THEME:
            m_CtrlColorTheme.get_value(message.data);
            break;

        case MessageType::ENVIRONMENT_SET_ENVIRONMENT:
            m_CtrlEnvironment.get_value(message.data);
            break;

        case MessageType::ENVIRONMENT_SET_AMBIENCE:
            m_CtrlAmbience.get_value(message.data);
            break;

        case MessageType::ENVIRONMENT_SET_AMBIENT_LIGHT:
            m_CtrlAmbientLight.get_value(message.data);
            break;

        case MessageType::INTERFACE_CONNECTIVITY_STATE_CHANGED:
            m_CtrlConnectivity.get_value(message.data);
            break;

        case MessageType::LAYOUT_CREATE_LAYOUT_REQ:
        case MessageType::LAYOUT_UPDATE_LAYOUT:
            m_CtrlTrackLayout.get_value(message.data);
            break;

        default:
            break;
    }
    return msgep->sendMsg(message);
}

void MsgSender::setActiveMessage(MessageType cmd, const std::string &name, std::uint32_t grpId, std::uint32_t msgId, Gtk::ScrolledWindow &container) {
    container.remove_with_viewport();
    activeMessage = cmd;
    activeMessageName = name;

    activeGrpId = grpId;
    activeMsgId = msgId;

    switch(cmd) {
        case MessageType::SERVER_RESET_CLIENT:
        case MessageType::SERVER_SELF_TESTING_CLIENT:
        case MessageType::LAYOUT_DEL_LAYOUT:
        case MessageType::LAYOUT_UNLOCK_LAYOUT:
        case MessageType::LAYOUT_LOCK_LAYOUT:
        case MessageType::LAYOUT_GET_LAYOUT_REQ:
        case MessageType::LAYOUT_GET_LAYOUT_READ_ONLY_REQ:
            m_CtrlString.init("Id", container);
            return;

        case MessageType::SYSTEM_SET_AUTOMATIC_MODE:
        case MessageType::SYSTEM_SET_EMERGENCY_STOP:
        case MessageType::SYSTEM_SET_STANDBY_MODE:
            m_CtrlBool.init("aktiv", container);
            break;

        case MessageType::CLIENT_ECHO_REQ:
            m_CtrlString.init("Data", container);
            return;

        case MessageType::TIMER_SET_GLOBAL_TIMER:
            m_CtrlGlobalTimer.init(container);
            break;

        case MessageType::TIMER_SET_COLOR_THEME:
            m_CtrlColorTheme.init(container);
            break;

        case MessageType::ENVIRONMENT_SET_ENVIRONMENT:
            m_CtrlEnvironment.init(container);
            break;

        case MessageType::ENVIRONMENT_SET_AMBIENCE:
            m_CtrlAmbience.init(container);
            break;

        case MessageType::ENVIRONMENT_SET_AMBIENT_LIGHT:
            m_CtrlAmbientLight.init(container);
            break;

        case MessageType::INTERFACE_CONNECTIVITY_STATE_CHANGED:
            m_CtrlConnectivity.init(container);
            break;

        case MessageType::LAYOUT_CREATE_LAYOUT_REQ:
            m_CtrlTrackLayout.init(false, container);
            break;

        case MessageType::LAYOUT_UPDATE_LAYOUT:
            m_CtrlTrackLayout.init(true, container);
            break;

        default:
            break;
    }
}
