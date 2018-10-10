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

        case moba::Message::MT_SET_AUTOMATIC_MODE:
        case moba::Message::MT_SET_EMERGENCY_STOP:
        case moba::Message::MT_SET_STANDBY_MODE:
            msgep->sendMsg(activeMessage, m_CtrlBool.get_value());
            break;

        case moba::Message::MT_ECHO_REQ:
            msgep->sendMsg(activeMessage, m_CtrlString.get_text());
            break;

        case moba::Message::MT_SET_GLOBAL_TIMER:
            msgep->sendMsg(activeMessage, m_CtrlGlobalTimer.get_value());
            break;

        case moba::Message::MT_SET_COLOR_THEME:
            msgep->sendMsg(activeMessage, m_CtrlColorTheme.get_value());
            break;

         case moba::Message::MT_SET_ENVIRONMENT:
            msgep->sendMsg(activeMessage, m_CtrlEnvironment.get_value());
            break;

        case moba::Message::MT_SET_AMBIENCE:
            msgep->sendMsg(activeMessage, m_CtrlAmbience.get_value());
            break;

        case moba::Message::MT_SET_AMBIENT_LIGHT:
            msgep->sendMsg(activeMessage, m_CtrlAmbientLight.get_value());
            break;

        case moba::Message::MT_SET_CONNECTIVITY:
            msgep->sendMsg(activeMessage, m_CtrlConnectivity.get_value());
            break;

        case moba::Message::MT_CREATE_LAYOUT_REQ:
        case moba::Message::MT_UPDATE_LAYOUT:
            msgep->sendMsg(activeMessage, m_CtrlTrackLayout.get_value());
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

        case moba::Message::MT_SET_AUTOMATIC_MODE:
        case moba::Message::MT_SET_EMERGENCY_STOP:
        case moba::Message::MT_SET_STANDBY_MODE:
            m_CtrlBool.init("aktiv", container);
            break;

        case moba::Message::MT_ECHO_REQ:
            m_CtrlString.init("Data", container);
            return;

        case moba::Message::MT_SET_GLOBAL_TIMER:
            m_CtrlGlobalTimer.init(container);
            break;

        case moba::Message::MT_SET_COLOR_THEME:
            m_CtrlColorTheme.init(container);
            break;

        case moba::Message::MT_SET_ENVIRONMENT:
            m_CtrlEnvironment.init(container);
            break;

        case moba::Message::MT_SET_AMBIENCE:
            m_CtrlAmbience.init(container);
            break;

        case moba::Message::MT_SET_AMBIENT_LIGHT:
            m_CtrlAmbientLight.init(container);
            break;

        case moba::Message::MT_SET_CONNECTIVITY:
            m_CtrlConnectivity.init(container);
            break;

        case moba::Message::MT_CREATE_LAYOUT_REQ:
            m_CtrlTrackLayout.init(false, container);
            break;

        case moba::Message::MT_UPDATE_LAYOUT:
            m_CtrlTrackLayout.init(true, container);
            break;
    }
}
