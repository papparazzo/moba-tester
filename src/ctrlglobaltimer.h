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

class CtrlGlobalTimer {
public:
    CtrlGlobalTimer() ;

    virtual ~CtrlGlobalTimer();

    void init(Gtk::ScrolledWindow &container);

    void get_value(rapidjson::Document &d) const;

protected:
    Gtk::Box m_VBox{Gtk::Orientation::VERTICAL, 6};
    Gtk::Box m_HBox_CurModelDay{Gtk::Orientation::HORIZONTAL, 6};
    Gtk::Box m_HBox_CurModelTime{Gtk::Orientation::HORIZONTAL, 6};
    Gtk::Box m_HBox_Multiplicator{Gtk::Orientation::HORIZONTAL, 6};

    Gtk::Entry m_Entry_CurModelDay;
    Gtk::Label m_Label_CurModelDay;

    Gtk::Entry m_Entry_CurModelTime;
    Gtk::Label m_Label_CurModelTime;

    Gtk::Entry m_Entry_Multiplicator;
    Gtk::Label m_Label_Multiplicator;
};