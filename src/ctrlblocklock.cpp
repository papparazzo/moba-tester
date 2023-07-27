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

#include "ctrlblocklock.h"

#include <string>
#include <sstream>

CtrlBlockLock::CtrlBlockLock() {
    m_VBox.append(m_HBox_Blocks);
    m_HBox_Blocks.append(m_Label_Blocks);
    m_HBox_Blocks.append(m_Entry_Blocks);

    m_Label_Blocks.set_label("BlockIds, Kommagetrennt");
}

nlohmann::json CtrlBlockLock::get_value() const {
    nlohmann::json data;

    std::string str = m_Entry_Blocks.get_text();
    std::replace(str.begin(), str.end(), ',', ' ');

    std::stringstream ss(str);

    int temp;
    while(ss >> temp) {
        data.push_back(temp);
    }
    return data;
}

void CtrlBlockLock::init(Gtk::ScrolledWindow &container) {
    m_Entry_Blocks.set_text("");

    container.set_child(m_VBox);
    //container.show_all_children();
}