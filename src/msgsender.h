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

#include <moba/message.h>
#include <moba/msgendpoint.h>

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

class MsgSender {
    public:
        MsgSender(moba::MsgEndpointPtr msgep);
        virtual ~MsgSender();

        void sendActiveMessage();
        void setActiveMessage(moba::Message::MessageType cmd, Gtk::ScrolledWindow &container);

    protected:
        moba::Message::MessageType activeMessage;
        moba::MsgEndpointPtr msgep;

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
