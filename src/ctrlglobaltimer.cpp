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

#include "ctrlglobaltimer.h"

#include <string>

CtrlGlobalTimer::CtrlGlobalTimer() {
    m_VBox.append(m_HBox_CurModelTime);
    m_VBox.append(m_HBox_Multiplicator);

    m_HBox_CurModelTime.append(m_Label_CurModelTime);
    m_HBox_CurModelTime.append(m_Entry_CurModelTime);

    m_HBox_Multiplicator.append(m_Label_Multiplicator);
    m_HBox_Multiplicator.append(m_Entry_Multiplicator);

            //Gtk::Entry m_Entry_CurModelDay;


    m_Label_CurModelDay.set_label("Tag");
    m_Label_CurModelTime.set_label("hh:mm (z.B. 10:00)");
    m_Label_Multiplicator.set_label("Multiplikator");
}

CtrlGlobalTimer::~CtrlGlobalTimer() {
}

void CtrlGlobalTimer::get_value(rapidjson::Document &d) const {
    std::string txt = m_Entry_CurModelTime.get_text();
    d.SetObject();
    d.AddMember("curModelDay", rapidjson::Value("SATURDAY", d.GetAllocator()), d.GetAllocator());
    d.AddMember("curModelTime", rapidjson::Value(txt.c_str(), txt.length(), d.GetAllocator()), d.GetAllocator());
    d.AddMember("multiplicator", rapidjson::Value(std::stol(m_Entry_Multiplicator.get_text())), d.GetAllocator());
}

void CtrlGlobalTimer::init(Gtk::ScrolledWindow &container) {
    m_Entry_CurModelTime.set_text("");
    m_Entry_Multiplicator.set_text("");

    container.set_child(m_VBox);
 //   container.show_all_children();
}