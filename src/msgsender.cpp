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
#include "moba/message.h"

void MsgSender::sendActiveMessage() {
    nlohmann::json data;

    switch(activeMessage) {
        case MessageType::SERVER_RESET_CLIENT:
        case MessageType::SERVER_SELF_TESTING_CLIENT:
        case MessageType::LAYOUT_DEL_LAYOUT:
        case MessageType::LAYOUT_UNLOCK_LAYOUT:
        case MessageType::LAYOUT_LOCK_LAYOUT:
        case MessageType::LAYOUT_GET_LAYOUT_REQ:
        case MessageType::LAYOUT_GET_LAYOUT_READ_ONLY_REQ:
            data = m_CtrlString.get_integer();
            break;

        case MessageType::SYSTEM_SET_AUTOMATIC_MODE:
        case MessageType::SYSTEM_SET_STANDBY_MODE:
            data = m_CtrlBool.get_value();
            break;

        case MessageType::CLIENT_ECHO_REQ: {
            data = m_CtrlString.get_text();
            break;
        }

        case MessageType::TIMER_SET_GLOBAL_TIMER:
            data = m_CtrlGlobalTimer.get_value();
            break;

        case MessageType::TIMER_SET_COLOR_THEME:
            data = m_CtrlColorTheme.get_value();
            break;

        case MessageType::ENVIRONMENT_SET_ENVIRONMENT:
            data = m_CtrlEnvironment.get_value();
            break;

        case MessageType::ENVIRONMENT_SET_AMBIENCE:
            data = m_CtrlAmbience.get_value();
            break;

        case MessageType::ENVIRONMENT_SET_AMBIENT_LIGHT:
            data = m_CtrlAmbientLight.get_value();
            break;

        case MessageType::INTERFACE_CONNECTIVITY_STATE_CHANGED:
            data = m_CtrlConnectivity.get_value();
            break;

        case MessageType::INTERFACE_CONTACT_TRIGGERED:
            data = m_CtrlContact.get_value();
            break;

        case MessageType::INTERFACE_SET_BRAKE_VECTOR:
            data = m_CtrlSetBrakeVector.get_value();
            break;

        case MessageType::LAYOUT_CREATE_LAYOUT_REQ:
        case MessageType::LAYOUT_UPDATE_LAYOUT:
            data = m_CtrlTrackLayout.get_value();
            break;

        case MessageType::CONTROL_LOCK_BLOCK:
        case MessageType::CONTROL_LOCK_BLOCK_WAITING:
        case MessageType::CONTROL_UNLOCK_BLOCK:
            data = m_CtrlBlockLock.get_value();
            break;

        default:
            break;
    }
    return endpoint->sendMsg(activeGrpId, activeMsgId, data);
}

void MsgSender::setActiveMessage(
    MessageType cmd, const std::string &name, std::uint32_t grpId, std::uint32_t msgId,
    Gtk::ScrolledWindow &container
) {
    //container.remove_with_viewport();
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

        case MessageType::INTERFACE_CONTACT_TRIGGERED:
            m_CtrlContact.init(container);
            break;

        case MessageType::INTERFACE_SET_BRAKE_VECTOR:
            //case MessageType::INTERFACE_SET_LOCO_SPEED:
            //case MessageType::INTERFACE_SET_LOCO_DIRECTION:
            m_CtrlSetBrakeVector.init(container);
            break;

        case MessageType::LAYOUT_CREATE_LAYOUT_REQ:
            m_CtrlTrackLayout.init(false, container);
            break;

        case MessageType::LAYOUT_UPDATE_LAYOUT:
            m_CtrlTrackLayout.init(true, container);
            break;

        case MessageType::CONTROL_LOCK_BLOCK:
        case MessageType::CONTROL_LOCK_BLOCK_WAITING:
        case MessageType::CONTROL_UNLOCK_BLOCK:
            m_CtrlBlockLock.init(container);
            break;

        default:
            break;
    }
}
