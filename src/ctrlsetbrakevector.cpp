/*
 *  Project:    moba-tester
 *
 *  Copyright (C) 2020 Stefan Paproth <pappi-@gmx.de>
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

#include "ctrlsetbrakevector.h"

CtrlSetBrakeVector::CtrlSetBrakeVector() {
    m_VBox.append(m_HBox_ModulAddress);
    m_VBox.append(m_HBox_ContactNumber);
    m_VBox.append(m_HBox_LocalId);

    m_HBox_ModulAddress.append(m_Label_ModulAddress);
    m_HBox_ModulAddress.append(m_Entry_ModulAddress);

    m_HBox_ContactNumber.append(m_Label_ContactNumber);
    m_HBox_ContactNumber.append(m_Entry_ContactNumber);

    m_HBox_LocalId.append(m_Label_LocalId);
    m_HBox_LocalId.append(m_Entry_LocalId);

    m_Label_ModulAddress.set_label("Moduladresse");
    m_Label_ContactNumber.set_label("Kontaktnummer");
    m_Label_LocalId.set_label("Lok-Id");
}

void CtrlSetBrakeVector::get_value(rapidjson::Document &d) const {
    d.SetArray();

    rapidjson::Value o;

    o.SetObject();
    o.AddMember("localId", rapidjson::Value(std::stol(m_Entry_LocalId.get_text())), d.GetAllocator());

    rapidjson::Value v;
    v.SetObject();
    v.AddMember("modulAddr", rapidjson::Value(std::stol(m_Entry_ModulAddress.get_text())), d.GetAllocator());
    v.AddMember("contactNb", rapidjson::Value(std::stol(m_Entry_ContactNumber.get_text())), d.GetAllocator());

    o.AddMember("contact", v, d.GetAllocator());
    d.PushBack(o, d.GetAllocator());
}

void CtrlSetBrakeVector::init(Gtk::ScrolledWindow &container) {
    m_Entry_ModulAddress.set_text("54");
    m_Entry_ContactNumber.set_text("");

    container.set_child(m_VBox);
   // container.show_all_children();
}
