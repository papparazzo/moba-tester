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

#include "ctrlconnectivity.h"

CtrlConnectivity::CtrlConnectivity() {
    m_VBox.append(m_HBox);
    m_HBox.append(m_Label);
    m_HBox.append(m_Combo);

    m_Label.set_label("Connectivity");

    m_Combo.append("CONNECTED");
    m_Combo.append("ERROR");
}

nlohmann::json CtrlConnectivity::get_value() const {
    return nlohmann::json{m_Combo.get_active_text()};
}

void CtrlConnectivity::init(Gtk::ScrolledWindow &container) {
    m_Combo.set_active(1);
    container.set_child(m_VBox);
   // container.show_all_children();
}
