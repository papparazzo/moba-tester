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

#include <string>

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

void CtrlEnvironment::get_value(rapidjson::Document &d) const {
    d.SetObject();
    d.AddMember("thunderStorm",     getSwitchState(Entries::THUNDER_STORM, d), d.GetAllocator());
    d.AddMember("wind",             getSwitchState(Entries::WIND, d), d.GetAllocator());
    d.AddMember("rain",             getSwitchState(Entries::RAIN, d), d.GetAllocator());
    d.AddMember("environmentSound", getSwitchState(Entries::ENVIRONMENT_SOUND, d), d.GetAllocator());
    d.AddMember("aux01",            getSwitchState(Entries::AUX_01, d), d.GetAllocator());
    d.AddMember("aux02",            getSwitchState(Entries::AUX_02, d), d.GetAllocator());
    d.AddMember("aux03",            getSwitchState(Entries::AUX_03, d), d.GetAllocator());
}

void CtrlEnvironment::init(Gtk::ScrolledWindow &container) {
    for(int i = 0; i < Entries::LAST_ENTRY; ++i) {
        m_Combo[i].set_active(1);
    }
    container.set_child(m_VBox);
   // container.show_all_children();
}

rapidjson::Value CtrlEnvironment::getSwitchState(CtrlEnvironment::Entries entry, rapidjson::Document &d) const {
    std::string str = m_Combo[entry].get_active_text();
    return rapidjson::Value{str.c_str(), static_cast<unsigned int>(str.length()), d.GetAllocator()};
}
