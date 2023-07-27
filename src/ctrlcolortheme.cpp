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

#include "ctrlcolortheme.h"

#include <string>

CtrlColorTheme::CtrlColorTheme() {
    m_VBox.append(m_HBox_DimTime);
    m_VBox.append(m_HBox_BrightTime);
    m_VBox.append(m_HBox_Condition);

    m_HBox_DimTime.append(m_Label_DimTime);
    m_HBox_DimTime.append(m_Entry_DimTime);

    m_HBox_BrightTime.append(m_Label_BrightTime);
    m_HBox_BrightTime.append(m_Entry_BrightTime);

    m_HBox_Condition.append(m_Label_Condition);
    m_HBox_Condition.append(m_Combo_Condition);

    m_Label_DimTime.set_label("Dim-Time (hh:mm)");
    m_Label_BrightTime.set_label("Bright-Time (hh:mm)");
    m_Label_Condition.set_label("Condition");

    m_Combo_Condition.append("ON");
    m_Combo_Condition.append("OFF");
    m_Combo_Condition.append("AUTO");
    m_Combo_Condition.append("UNSET");
}

nlohmann::json CtrlColorTheme::get_value() const {
    nlohmann::json data;

    data["dimTime"] = m_Entry_DimTime.get_text();
    data["brightTime"] = m_Entry_BrightTime.get_text();
    data["condition"] = m_Combo_Condition.get_active_text();
    return data;
}

void CtrlColorTheme::init(Gtk::ScrolledWindow &container) {
    m_Entry_DimTime.set_text("");
    m_Entry_BrightTime.set_text("");
    m_Combo_Condition.set_active(1);
    container.set_child(m_VBox);
    //container.show_all_children();
}
