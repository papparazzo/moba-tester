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
#include "moba/basemessage.h"

void MsgSender::sendActiveMessage() {

    switch(activeMessage) {
        case MessageType::SERVER_RESET_CLIENT:
        case MessageType::SERVER_SELF_TESTING_CLIENT:
        case MessageType::LAYOUTS_DEL_LAYOUT:
        case MessageType::LAYOUTS_UNLOCK_LAYOUT:
        case MessageType::LAYOUT_GET_LAYOUT_REQ:
            return msgep->sendMsg(DispatchGenericMessage{activeMessageName, m_CtrlString.get_jsonInt()});

        case MessageType::SYSTEM_SET_AUTOMATIC_MODE:
        case MessageType::SYSTEM_SET_EMERGENCY_STOP:
        case MessageType::SYSTEM_SET_STANDBY_MODE:
            return msgep->sendMsg(DispatchGenericMessage{activeMessageName, m_CtrlBool.get_value()});

        case MessageType::CLIENT_ECHO_REQ:
            return msgep->sendMsg(DispatchGenericMessage{activeMessageName, m_CtrlString.get_jsonText()});

        case MessageType::TIMER_SET_GLOBAL_TIMER:
            return msgep->sendMsg(DispatchGenericMessage{activeMessageName, m_CtrlGlobalTimer.get_value()});

        case MessageType::TIMER_SET_COLOR_THEME:
            return msgep->sendMsg(DispatchGenericMessage{activeMessageName, m_CtrlColorTheme.get_value()});

        case MessageType::ENVIRONMENT_SET_ENVIRONMENT:
            return msgep->sendMsg(DispatchGenericMessage{activeMessageName, m_CtrlEnvironment.get_value()});

        case MessageType::ENVIRONMENT_SET_AMBIENCE:
            return msgep->sendMsg(DispatchGenericMessage{activeMessageName, m_CtrlAmbience.get_value()});

        case MessageType::ENVIRONMENT_SET_AMBIENT_LIGHT:
            return msgep->sendMsg(DispatchGenericMessage{activeMessageName, m_CtrlAmbientLight.get_value()});

        case MessageType::INTERFACE_CONNECTIVITY_STATE_CHANGED:
            return msgep->sendMsg(DispatchGenericMessage{activeMessageName, m_CtrlConnectivity.get_value()});

        case MessageType::LAYOUTS_CREATE_LAYOUT_REQ:
        case MessageType::LAYOUTS_UPDATE_LAYOUT:
            return msgep->sendMsg(DispatchGenericMessage{activeMessageName, m_CtrlTrackLayout.get_value()});

        default:
            return msgep->sendMsg(DispatchGenericMessage{activeMessageName});
      }
}

void MsgSender::setActiveMessage(MessageType cmd, const std::string &name, Gtk::ScrolledWindow &container) {
    container.remove_with_viewport();
    activeMessage = cmd;
    activeMessageName = name;

    switch(cmd) {
        case MessageType::SERVER_RESET_CLIENT:
        case MessageType::SERVER_SELF_TESTING_CLIENT:
        case MessageType::LAYOUTS_DEL_LAYOUT:
        case MessageType::LAYOUTS_UNLOCK_LAYOUT:
        case MessageType::LAYOUT_GET_LAYOUT_REQ:
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

        case MessageType::LAYOUTS_CREATE_LAYOUT_REQ:
            m_CtrlTrackLayout.init(false, container);
            break;

        case MessageType::LAYOUTS_UPDATE_LAYOUT:
            m_CtrlTrackLayout.init(true, container);
            break;
            
        default:
            break;
    }
}
