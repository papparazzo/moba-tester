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

#include "ctrlambientlight.h"

CtrlAmbientLight::CtrlAmbientLight() {
    for(int i = 0; i < LAST_ENTRY; ++i) {
        m_adjustment[i] = Gtk::Adjustment::create(100.0, 0.0, 4095.0, 1.0, 1.0, 0.0);
        m_SpinButton[i].set_adjustment(m_adjustment[i]);
        m_HBox[i].set_orientation(Gtk::Orientation::HORIZONTAL);
        m_HBox[i].append(m_Label[i]);
        m_HBox[i].append(m_SpinButton[i]);
        m_VBox.append(m_HBox[i]);
    }
    m_Label[RED  ].set_label("red");
    m_Label[BLUE ].set_label("blue");
    m_Label[GREEN].set_label("green");
    m_Label[WHITE].set_label("white");
}

nlohmann::json CtrlAmbientLight::get_value() const {
    nlohmann::json data;

    data["red"] =  m_SpinButton[RED  ].get_value_as_int();
    data["blue"] = m_SpinButton[BLUE ].get_value_as_int();
    data["green"] = m_SpinButton[GREEN].get_value_as_int();
    data["white"] = m_SpinButton[WHITE].get_value_as_int();
    return data;
}

void CtrlAmbientLight::init(Gtk::ScrolledWindow &container) {
    container.set_child(m_VBox);
    //container.show_all_children();
}

