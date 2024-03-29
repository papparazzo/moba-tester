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

class CtrlTrackLayout {
public:
    CtrlTrackLayout() ;

    virtual ~CtrlTrackLayout() = default;

    void init(bool enableIdSetting, Gtk::ScrolledWindow &container);

    nlohmann::json get_value() const;

protected:
    Gtk::Box m_VBox{Gtk::Orientation::VERTICAL, 6};
    Gtk::Box m_HBox_Id{Gtk::Orientation::HORIZONTAL, 6};
    Gtk::Box m_HBox_Caption{Gtk::Orientation::HORIZONTAL, 6};
    Gtk::Box m_HBox_Description{Gtk::Orientation::HORIZONTAL, 6};
    Gtk::Box m_HBox_Active{Gtk::Orientation::HORIZONTAL, 6};

    Gtk::Label m_Label_Id;
    Gtk::Entry m_Entry_Id;

    Gtk::Label m_Label_Caption;
    Gtk::Entry m_Entry_Name;

    Gtk::Label m_Label_Description;
    Gtk::Entry m_Entry_Description;

    Gtk::Label m_Label_Active;
    Gtk::CheckButton m_Check_Bool;

};
