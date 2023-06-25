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

#include "ctrlstring.h"

#include <string>

CtrlString::CtrlString() {
    m_VBox.append(m_HBox_String);
    m_HBox_String.append(m_Label_String);
    m_HBox_String.append(m_Entry_String);
}

CtrlString::~CtrlString() {
}

void CtrlString::init(const std::string &caption, Gtk::ScrolledWindow &container) {
    m_Entry_String.set_text("");
    m_Label_String.set_label(caption);
    container.set_child(m_VBox);
  //  container.show_all_children();
}

long CtrlString::get_integer() const {
    return std::stol(m_Entry_String.get_text());
}

float CtrlString::get_float() const {
    return std::stof(m_Entry_String.get_text());
}

std::string CtrlString::get_text() const {
    return m_Entry_String.get_text();
}
