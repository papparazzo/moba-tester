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

#include "ctrlambience.h"

CtrlAmbience::CtrlAmbience() {
    for(int i = 0; i < LAST_ENTRY; ++i) {
        m_HBox[i].set_orientation(Gtk::Orientation::HORIZONTAL);
        m_HBox[i].append(m_Label[i]);
        m_HBox[i].append(m_Combo[i]);
        m_VBox.append(m_HBox[i]);
        m_Combo[i].append("ON");
        m_Combo[i].append("OFF");
        m_Combo[i].append("UNSET");
    }
    m_Label[CURTAIN_UP   ].set_label("curtain (down = off)");
    m_Label[MAIN_LIGHT_ON].set_label("main-light");
}

nlohmann::json CtrlAmbience::get_value() const {
    nlohmann::json data;
    data["curtainUp"] = m_Combo[CURTAIN_UP].get_active_text();
    data["mainLightOn"] = m_Combo[MAIN_LIGHT_ON].get_active_text();
    return data;
}

void CtrlAmbience::init(Gtk::ScrolledWindow &container) {
    for(auto & i : m_Combo) {
        i.set_active(1);
    }
    container.set_child(m_VBox);
    //container.show_all_children();
}
