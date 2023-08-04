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

#include "ctrltracklayout.h"

#include <string>

CtrlTrackLayout::CtrlTrackLayout() {
    m_VBox.append(m_HBox_Id);
    m_VBox.append(m_HBox_Caption);
    m_VBox.append(m_HBox_Description);
    m_VBox.append(m_HBox_Active);

    m_HBox_Id.append(m_Label_Id);
    m_HBox_Id.append(m_Entry_Id);

    m_HBox_Caption.append(m_Label_Caption);
    m_HBox_Caption.append(m_Entry_Name);

    m_HBox_Description.append(m_Label_Description);
    m_HBox_Description.append(m_Entry_Description);

    m_HBox_Active.append(m_Label_Active);
    m_HBox_Active.append(m_Check_Bool);

    m_Label_Id.set_label("Id");
    m_Label_Caption.set_label("Name");
    m_Label_Description.set_label("Description");
    m_Label_Active.set_label("Active");
}

nlohmann::json CtrlTrackLayout::get_value() const {
    nlohmann::json data;

    if(m_Entry_Id.get_visible()) {
        data["id"] = std::stol(m_Entry_Id.get_text());
    }
    data["name"] = m_Entry_Name.get_text();
    data["description"] = m_Entry_Description.get_text();
    data["active"] = m_Check_Bool.get_active();

    return data;
}

void CtrlTrackLayout::init(bool enableIdSetting, Gtk::ScrolledWindow &container) {
    m_Entry_Id.set_text("");
    m_Entry_Name.set_text("");
    m_Entry_Description.set_text("");
    container.set_child(m_VBox);
   // container.show_all_children();
    m_Entry_Id.set_visible(enableIdSetting);
    m_Label_Id.set_visible(enableIdSetting);
}
