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

#include "ctrlenvironment.h"

CtrlEnvironment::CtrlEnvironment() {
    for(int i = 0; i < Entries::LAST_ENTRY; ++i) {
        m_HBox[i].set_orientation(Gtk::Orientation::HORIZONTAL);
        m_HBox[i].append(m_Label[i]);
        m_HBox[i].append(m_Combo[i]);
        m_VBox.append(m_HBox[i]);
        m_Combo[i].append("ON");
        m_Combo[i].append("OFF");
        m_Combo[i].append("TRIGGER");
        m_Combo[i].append("AUTO");
        m_Combo[i].append("UNSET");
    }
    m_Label[Entries::THUNDER_STORM    ].set_label("thunder-storm");
    m_Label[Entries::WIND             ].set_label("wind");
    m_Label[Entries::RAIN             ].set_label("rain");
    m_Label[Entries::ENVIRONMENT_SOUND].set_label("environment-sound");
    m_Label[Entries::AUX_01           ].set_label("aux 01");
    m_Label[Entries::AUX_02           ].set_label("aux 02");
    m_Label[Entries::AUX_03           ].set_label("aux 03");
}

nlohmann::json CtrlEnvironment::get_value() const {
    nlohmann::json data;

    data["thunderStorm"    ] = m_Combo[Entries::THUNDER_STORM].get_active_text();
    data["wind"            ] = m_Combo[Entries::WIND].get_active_text();
    data["rain"            ] = m_Combo[Entries::RAIN].get_active_text();
    data["environmentSound"] = m_Combo[Entries::ENVIRONMENT_SOUND].get_active_text();
    data["aux01"           ] = m_Combo[Entries::AUX_01].get_active_text();
    data["aux02"           ] = m_Combo[Entries::AUX_02].get_active_text();
    data["aux03"           ] = m_Combo[Entries::AUX_03].get_active_text();

    return data;
}

void CtrlEnvironment::init(Gtk::ScrolledWindow &container) {
    for(auto &i: m_Combo) {
        i.set_active(1);
    }
    container.set_child(m_VBox);
    // container.show_all_children();
}
