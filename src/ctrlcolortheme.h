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

#pragma once

#include <gtkmm.h>
#include "moba/nlohmann/json.hpp"

class CtrlColorTheme {
public:
    CtrlColorTheme() ;

    virtual ~CtrlColorTheme() = default;

    void init(Gtk::ScrolledWindow &container);

    nlohmann::json get_value() const;

protected:
    Gtk::Box m_VBox{Gtk::Orientation::VERTICAL, 6};
    Gtk::Box m_HBox_DimTime{Gtk::Orientation::HORIZONTAL, 6};
    Gtk::Box m_HBox_BrightTime{Gtk::Orientation::HORIZONTAL, 6};
    Gtk::Box m_HBox_Condition{Gtk::Orientation::HORIZONTAL, 6};

    Gtk::Label m_Label_DimTime;
    Gtk::Entry m_Entry_DimTime;

    Gtk::Label m_Label_BrightTime;
    Gtk::Entry m_Entry_BrightTime;

    Gtk::Label m_Label_Condition;
    Gtk::ComboBoxText m_Combo_Condition;
};
