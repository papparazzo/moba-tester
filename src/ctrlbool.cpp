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

#include "ctrlbool.h"

#include <string>

CtrlBool::CtrlBool() {
    m_VBox.append(m_HBox_Bool);
    m_HBox_Bool.append(m_Check_Bool);
}

bool CtrlBool::get_value() const {
    return m_Check_Bool.get_active();
}

void CtrlBool::init(const std::string &caption, Gtk::ScrolledWindow &container) {
    m_Check_Bool.set_label(caption);
    m_Check_Bool.set_active(false);
    container.set_child(m_VBox);
  //  container.show_all_children();
}
