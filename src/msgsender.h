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

#include "gtkmm.h"
#include "string"

class MsgSender {
    public:
        MsgSender(moba::MsgEndpointPtr msgep);
        virtual ~MsgSender();

        void sendActiveMessage();
        void setActiveMessage(moba::Message::MessageType cmd, Gtk::ScrolledWindow &container);

    protected:
        moba::Message::MessageType activeMessage;
        moba::MsgEndpointPtr msgep;

        void setInputbox(const std::string &caption, Gtk::ScrolledWindow &container);
        void setCheckbox(const std::string &caption, Gtk::ScrolledWindow &container);

        // String / Int
        Gtk::Box m_HBox;
        Gtk::Entry m_Entry;
        Gtk::Label m_Label;

        // Bool
        Gtk::CheckButton m_Check;
};
