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

class CtrlAmbientLight {
public:
    CtrlAmbientLight() ;

    virtual ~CtrlAmbientLight() = default;

    void init(Gtk::ScrolledWindow &container);

    nlohmann::json get_value() const;

protected:
    enum Entries {
        RED,
        BLUE,
        GREEN,
        WHITE,
        LAST_ENTRY
    };

    Gtk::Box m_VBox{Gtk::Orientation::VERTICAL, 6};
    Gtk::Box m_HBox[LAST_ENTRY];
    Gtk::Label m_Label[LAST_ENTRY];
    Gtk::SpinButton m_SpinButton[LAST_ENTRY];

    Glib::RefPtr<Gtk::Adjustment> m_adjustment[LAST_ENTRY];
};
