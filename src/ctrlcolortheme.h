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
#include "moba/rapidjson/document.h"

class CtrlColorTheme {
    public:
        CtrlColorTheme() ;

        virtual ~CtrlColorTheme();

        void init(Gtk::ScrolledWindow &container);

        void get_value(rapidjson::Document &d) const;

    protected:
        Gtk::Box m_VBox{Gtk::ORIENTATION_VERTICAL, 6};
        Gtk::Box m_HBox_DimTime{Gtk::ORIENTATION_HORIZONTAL, 6};
        Gtk::Box m_HBox_BrightTime{Gtk::ORIENTATION_HORIZONTAL, 6};
        Gtk::Box m_HBox_Condition{Gtk::ORIENTATION_HORIZONTAL, 6};

        Gtk::Label m_Label_DimTime;
        Gtk::Entry m_Entry_DimTime;

        Gtk::Label m_Label_BrightTime;
        Gtk::Entry m_Entry_BrightTime;

        Gtk::Label m_Label_Condition;
        Gtk::ComboBoxText m_Combo_Condition;

        rapidjson::Value setText(const std::string &value, rapidjson::Document &d) const;
};
