/*
 *  Project:    moba-tester
 *
 *  Copyright (C) 2023 Stefan Paproth <pappi-@gmx.de>
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

#include "ctrlcontact.h"

CtrlContact::CtrlContact() {
    m_VBox.append(m_HBox_ModulAddress);
    m_VBox.append(m_HBox_ContactNumber);
    m_VBox.append(m_HBox_State);

    m_HBox_ModulAddress.append(m_Label_ModulAddress);
    m_HBox_ModulAddress.append(m_Entry_ModulAddress);

    m_HBox_ContactNumber.append(m_Label_ContactNumber);
    m_HBox_ContactNumber.append(m_Entry_ContactNumber);

    m_HBox_State.append(m_Check_State);

    m_Label_ModulAddress.set_label("Moduladresse");
    m_Label_ContactNumber.set_label("Kontaktnummer");
}

nlohmann::json CtrlContact::get_value() const {
    nlohmann::json data;

    data["state"] = m_Check_State.get_active();
    data["time"] = 2;

    nlohmann::json v;
    v["modulAddr"] = std::stol(m_Entry_ModulAddress.get_text());
    v["contactNb"] = std::stol(m_Entry_ContactNumber.get_text());

    data["contact"] = v;

    return data;
}

void CtrlContact::init(Gtk::ScrolledWindow &container) {
    m_Entry_ModulAddress.set_text("54");
    m_Entry_ContactNumber.set_text("");

    container.set_child(m_VBox);
    //container.show_all_children();
    m_Check_State.set_label("aktiv");
}
