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

#include "ctrltestmessage.h"
#include <string>

CtrlTestMessage::CtrlTestMessage() {
    for(int i = 0; i < 3; ++i) {
        m_HBox[i].set_orientation(Gtk::Orientation::HORIZONTAL);
        m_HBox[i].append(m_Label[i]);
        m_HBox[i].append(m_Entry[i]);
        m_VBox.append(m_HBox[i]);
    }
    m_Label[0].set_label("Group-Id");
    m_Label[1].set_label("Message-Id");
    m_Label[2].set_label("Message");
}

void CtrlTestMessage::send(EndpointPtr endpoint) const {
    endpoint->sendMsg(
        std::stol(m_Entry[0].get_text()),
        std::stol(m_Entry[1].get_text()),
        std::string(m_Entry[2].get_text())
    );
}

void CtrlTestMessage::init(Gtk::ScrolledWindow &container) {
    container.set_child(m_VBox);
}
