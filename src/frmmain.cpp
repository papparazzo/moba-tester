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

#include <iostream>

#include <ctime>
#include <string>

#include "moba/systemmessages.h"
#include "frmmain.h"
#include "config.h"

namespace {
    const char license[] =
        "Project:    moba-systemmanager\n"
        "\n"
        "Copyright (C) 2018 Stefan Paproth <pappi-@gmx.de>\n"
        "\n"
        "This program is free software: you can redistribute it and/or modify\n"
        "it under the terms of the GNU Affero General Public License as\n"
        "published by the Free Software Foundation, either version 3 of the\n"
        "License, or (at your option) any later version.\n"
        "\n"
        "This program is distributed in the hope that it will be useful,\n"
        "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the\n"
        "GNU Affero General Public License for more details.\n"
        "\n"
        "You should have received a copy of the GNU Affero General Public License\n"
        "along with this program. If not, see <https://www.gnu.org/licenses/agpl.txt>.";

    void replace(std::string& str, const std::string& oldStr, const std::string& newStr) {
        std::string::size_type pos = 0u;
        while((pos = str.find(oldStr, pos)) != std::string::npos) {
            str.replace(pos, oldStr.length(), newStr);
            pos += newStr.length();
        }
    }
}

FrmMain::FrmMain(EndpointPtr mhp) : msgEndpoint{mhp}, msgSender{mhp} {
    sigc::slot<bool()> my_slot = sigc::bind(sigc::mem_fun(*this, &FrmMain::on_timeout), 1);
    sigc::connection conn = Glib::signal_timeout().connect(my_slot, 25); // 25 ms

    sigc::slot<bool()> my_slot2 = sigc::bind(sigc::mem_fun(*this, &FrmMain::on_timeout_status), 1);
    sigc::connection conn2 = Glib::signal_timeout().connect(my_slot2, 850, Glib::PRIORITY_DEFAULT_IDLE); // 25 ms

	m_VBox.set_margin(6);
    set_child(m_VBox);

    m_HPaned.set_expand(true);
    m_VBox.append(m_HPaned);

    m_HBox_Status.set_margin(6);
    m_HBox_Status.append(m_Label_Connectivity_SW);
    m_Label_Connectivity_SW.set_justify(Gtk::Justification::LEFT);
    m_Label_Connectivity_SW.set_markup("<span color=\"gray\"> \xe2\x96\x84</span>");
    m_Label_Connectivity_SW.set_tooltip_markup("<b>Status:</b> unbekannt");

    m_HBox_Status.append(m_Label_Connectivity_HW);
    m_Label_Connectivity_HW.set_justify(Gtk::Justification::LEFT);
    m_Label_Connectivity_HW.set_markup("<span color=\"gray\"> \xe2\x96\x84</span>");
    m_Label_Connectivity_HW.set_tooltip_markup("<b>Status:</b> unbekannt");

    m_HBox_Status.append(m_HButtonBox);

    m_HBox_Expander.set_hexpand();
    m_HButtonBox.append(m_HBox_Expander);

    // about-dialog
    m_HButtonBox.append(m_Button_About);
    m_Button_About.signal_clicked().connect(sigc::mem_fun(*this, &FrmMain::on_button_about_clicked));

    m_HButtonBox.append(m_Button_Emergency);
    m_Button_Emergency.signal_clicked().connect(sigc::mem_fun(*this, &FrmMain::on_button_emergency_clicked));

    m_Button_Emergency.set_sensitive(false);

    m_VBox.append(m_HBox_Status);

    initAboutDialog();
    initTreeModel();
    initOutgoing();
    initIncomming();

    registry.registerHandler<SystemHardwareStateChanged>([this](auto && PH1) { setHardwareState(std::forward<decltype(PH1)>(PH1)); });
    registry.registerAuxiliaryHandler(std::bind(&FrmMain::msgHandler, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    m_Button_Send.set_sensitive(false);
    //show_all_children();
}

void FrmMain::initAboutDialog() {
    m_Dialog.set_transient_for(*this);

    m_Dialog.set_program_name(PACKAGE_NAME);
    m_Dialog.set_version(PACKAGE_VERSION);
    m_Dialog.set_copyright("Stefan Paproth");
    m_Dialog.set_comments("This is just an application for testing purpose.");
    m_Dialog.set_license(license);

    //m_Dialog.set_logo(Gdk::Pixbuf::create_from_file(DATADIR "/icons/hicolor/scalable/apps/moba-tester.svg"));

    m_Dialog.set_website("<pappi-@gmx.de>");
    m_Dialog.set_website_label("pappi-@gmx.de");

    std::vector<Glib::ustring> list_authors;
    list_authors.push_back("Stefan Paproth");
    m_Dialog.set_authors(list_authors);

    //m_Dialog.signal_response().connect(sigc::mem_fun(*this, &FrmMain::on_about_dialog_response));

    m_Button_About.grab_focus();
}

void FrmMain::initTreeModel() {
    m_refTreeModel_Outgoing = Gtk::TreeStore::create(m_Columns_Messages);
    m_TreeView_Outgoing.set_model(m_refTreeModel_Outgoing);

    Gtk::TreeModel::Row row = *(m_refTreeModel_Outgoing->append());
    row[m_Columns_Messages.m_col_msg_id] = MessageType::UNSET;
    row[m_Columns_Messages.m_col_id] = "0";
    row[m_Columns_Messages.m_col_name] = "[Test]";

    Gtk::TreeModel::Row childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::TEST_MESSAGE;
    childRow[m_Columns_Messages.m_col_id] = "0.0";
    childRow[m_Columns_Messages.m_col_name] = "TEST_MESSAGE";

    row = *(m_refTreeModel_Outgoing->append());
    row[m_Columns_Messages.m_col_msg_id] = MessageType::UNSET;
    row[m_Columns_Messages.m_col_id] = "2";
    row[m_Columns_Messages.m_col_name] = "Client";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::CLIENT_VOID;
    childRow[m_Columns_Messages.m_col_id] = "2.1";
    childRow[m_Columns_Messages.m_col_name] = "CLIENT_VOID";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::CLIENT_ECHO_REQ;
    childRow[m_Columns_Messages.m_col_id] = "2.2";
    childRow[m_Columns_Messages.m_col_name] = "CLIENT_ECHO_REQ";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::CLIENT_START;
    childRow[m_Columns_Messages.m_col_id] = "2.5";
    childRow[m_Columns_Messages.m_col_name] = "CLIENT_START";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::CLIENT_CLOSE;
    childRow[m_Columns_Messages.m_col_id] = "2.7";
    childRow[m_Columns_Messages.m_col_name] = "CLIENT_CLOSE";

    row = *(m_refTreeModel_Outgoing->append());
    row[m_Columns_Messages.m_col_msg_id] = MessageType::UNSET;
    row[m_Columns_Messages.m_col_id] = "3";
    row[m_Columns_Messages.m_col_name] = "Server";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::SERVER_RESET_CLIENT;
    childRow[m_Columns_Messages.m_col_id] = "3.3";
    childRow[m_Columns_Messages.m_col_name] = "SERVER_RESET_CLIENT";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::SERVER_INFO_REQ;
    childRow[m_Columns_Messages.m_col_id] = "3.4";
    childRow[m_Columns_Messages.m_col_name] = "SERVER_INFO_REQ";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::SERVER_CON_CLIENTS_REQ;
    childRow[m_Columns_Messages.m_col_id] = "3.6";
    childRow[m_Columns_Messages.m_col_name] = "SERVER_CON_CLIENTS_REQ";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::SERVER_SELF_TESTING_CLIENT;
    childRow[m_Columns_Messages.m_col_id] = "3.8";
    childRow[m_Columns_Messages.m_col_name] = "SERVER_SELF_TESTING_CLIENT";

    row = *(m_refTreeModel_Outgoing->append());
    row[m_Columns_Messages.m_col_msg_id] = MessageType::UNSET;
    row[m_Columns_Messages.m_col_id] = "4";
    row[m_Columns_Messages.m_col_name] = "Timer";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::TIMER_GET_GLOBAL_TIMER;
    childRow[m_Columns_Messages.m_col_id] = "4.2";
    childRow[m_Columns_Messages.m_col_name] = "TIMER_GET_GLOBAL_TIMER";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::TIMER_SET_GLOBAL_TIMER;
    childRow[m_Columns_Messages.m_col_id] = "4.3";
    childRow[m_Columns_Messages.m_col_name] = "TIMER_SET_GLOBAL_TIMER";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::TIMER_GET_COLOR_THEME;
    childRow[m_Columns_Messages.m_col_id] = "4.4";
    childRow[m_Columns_Messages.m_col_name] = "TIMER_GET_COLOR_THEME";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::TIMER_SET_COLOR_THEME;
    childRow[m_Columns_Messages.m_col_id] = "4.5";
    childRow[m_Columns_Messages.m_col_name] = "TIMER_SET_COLOR_THEME";

    row = *(m_refTreeModel_Outgoing->append());
    row[m_Columns_Messages.m_col_msg_id] = MessageType::UNSET;
    row[m_Columns_Messages.m_col_id] = "5";
    row[m_Columns_Messages.m_col_name] = "Environment";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::ENVIRONMENT_GET_ENVIRONMENT;
    childRow[m_Columns_Messages.m_col_id] = "5.1";
    childRow[m_Columns_Messages.m_col_name] = "ENVIRONMENT_GET_ENVIRONMENT";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::ENVIRONMENT_SET_ENVIRONMENT;
    childRow[m_Columns_Messages.m_col_id] = "5.2";
    childRow[m_Columns_Messages.m_col_name] = "ENVIRONMENT_SET_ENVIRONMENT";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::ENVIRONMENT_SET_AMBIENCE;
    childRow[m_Columns_Messages.m_col_id] = "5.3";
    childRow[m_Columns_Messages.m_col_name] = "ENVIRONMENT_SET_AMBIENCE";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::ENVIRONMENT_SET_AMBIENT_LIGHT;
    childRow[m_Columns_Messages.m_col_id] = "5.4";
    childRow[m_Columns_Messages.m_col_name] = "ENVIRONMENT_SET_AMBIENT_LIGHT";

    row = *(m_refTreeModel_Outgoing->append());
    row[m_Columns_Messages.m_col_msg_id] = MessageType::UNSET;
    row[m_Columns_Messages.m_col_id] = "6";
    row[m_Columns_Messages.m_col_name] = "Interface";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::INTERFACE_CONNECTIVITY_STATE_CHANGED;
    childRow[m_Columns_Messages.m_col_id] = "6.1";
    childRow[m_Columns_Messages.m_col_name] = "INTERFACE_CONNECTIVITY_STATE_CHANGED";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::INTERFACE_CONTACT_TRIGGERED;
    childRow[m_Columns_Messages.m_col_id] = "6.2";
    childRow[m_Columns_Messages.m_col_name] = "INTERFACE_CONTACT_TRIGGERED";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::INTERFACE_SET_BRAKE_VECTOR;
    childRow[m_Columns_Messages.m_col_id] = "6.3";
    childRow[m_Columns_Messages.m_col_name] = "INTERFACE_SET_BRAKE_VECTOR";
/*
    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::INTERFACE_SET_LOCO_SPEED;
    childRow[m_Columns_Messages.m_col_id] = "6.4";
    childRow[m_Columns_Messages.m_col_name] = "INTERFACE_SET_LOCO_SPEED";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::INTERFACE_SET_LOCO_DIRECTION;
    childRow[m_Columns_Messages.m_col_id] = "6.5";
    childRow[m_Columns_Messages.m_col_name] = "INTERFACE_SET_LOCO_DIRECTION";
*/
    row = *(m_refTreeModel_Outgoing->append());
    row[m_Columns_Messages.m_col_msg_id] = MessageType::UNSET;
    row[m_Columns_Messages.m_col_id] = "7";
    row[m_Columns_Messages.m_col_name] = "System";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::SYSTEM_SET_AUTOMATIC_MODE;
    childRow[m_Columns_Messages.m_col_id] = "7.1";
    childRow[m_Columns_Messages.m_col_name] = "SYSTEM_SET_AUTOMATIC_MODE";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::SYSTEM_TOGGLE_AUTOMATIC_MODE;
    childRow[m_Columns_Messages.m_col_id] = "7.2";
    childRow[m_Columns_Messages.m_col_name] = "SYSTEM_TOGGLE_AUTOMATIC_MODE";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::SYSTEM_TRIGGER_EMERGENCY_STOP;
    childRow[m_Columns_Messages.m_col_id] = "7.3";
    childRow[m_Columns_Messages.m_col_name] = "SYSTEM_TRIGGER_EMERGENCY_STOP";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::SYSTEM_RELEASE_EMERGENCY_STOP;
    childRow[m_Columns_Messages.m_col_id] = "7.4";
    childRow[m_Columns_Messages.m_col_name] = "SYSTEM_RELEASE_EMERGENCY_STOP";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::SYSTEM_SET_STANDBY_MODE;
    childRow[m_Columns_Messages.m_col_id] = "7.5";
    childRow[m_Columns_Messages.m_col_name] = "SYSTEM_SET_STANDBY_MODE";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::SYSTEM_TOGGLE_STANDBY_MODE;
    childRow[m_Columns_Messages.m_col_id] = "7.6";
    childRow[m_Columns_Messages.m_col_name] = "SYSTEM_TOGGLE_STANDBY_MODE";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::SYSTEM_GET_HARDWARE_STATE;
    childRow[m_Columns_Messages.m_col_id] = "7.7";
    childRow[m_Columns_Messages.m_col_name] = "SYSTEM_GET_HARDWARE_STATE";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::SYSTEM_HARDWARE_SHUTDOWN;
    childRow[m_Columns_Messages.m_col_id] = "7.9";
    childRow[m_Columns_Messages.m_col_name] = "SYSTEM_HARDWARE_SHUTDOWN";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::SYSTEM_HARDWARE_RESET;
    childRow[m_Columns_Messages.m_col_id] = "7.10";
    childRow[m_Columns_Messages.m_col_name] = "SYSTEM_HARDWARE_RESET";

    row = *(m_refTreeModel_Outgoing->append());
    row[m_Columns_Messages.m_col_msg_id] = MessageType::UNSET;
    row[m_Columns_Messages.m_col_id] = "8";
    row[m_Columns_Messages.m_col_name] = "Layout";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::LAYOUT_GET_LAYOUTS_REQ;
    childRow[m_Columns_Messages.m_col_id] = "8.1";
    childRow[m_Columns_Messages.m_col_name] = "LAYOUT_GET_LAYOUTS_REQ";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::LAYOUT_DEL_LAYOUT;
    childRow[m_Columns_Messages.m_col_id] = "8.3";
    childRow[m_Columns_Messages.m_col_name] = "LAYOUT_DELETE_LAYOUT";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::LAYOUT_CREATE_LAYOUT_REQ;
    childRow[m_Columns_Messages.m_col_id] = "8.5";
    childRow[m_Columns_Messages.m_col_name] = "LAYOUT_CREATE_LAYOUT";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::LAYOUT_UPDATE_LAYOUT;
    childRow[m_Columns_Messages.m_col_id] = "8.7";
    childRow[m_Columns_Messages.m_col_name] = "LAYOUT_UPDATE_LAYOUT";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::LAYOUT_UNLOCK_LAYOUT;
    childRow[m_Columns_Messages.m_col_id] = "8.9";
    childRow[m_Columns_Messages.m_col_name] = "LAYOUT_UNLOCK_LAYOUT";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::LAYOUT_LOCK_LAYOUT;
    childRow[m_Columns_Messages.m_col_id] = "8.11";
    childRow[m_Columns_Messages.m_col_name] = "LAYOUT_LOCK_LAYOUT";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::LAYOUT_GET_LAYOUT_REQ;
    childRow[m_Columns_Messages.m_col_id] = "8.13";
    childRow[m_Columns_Messages.m_col_name] = "LAYOUT_GET_LAYOUT_REQ";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::LAYOUT_GET_LAYOUT_READ_ONLY_REQ;
    childRow[m_Columns_Messages.m_col_id] = "8.14";
    childRow[m_Columns_Messages.m_col_name] = "LAYOUT_GET_LAYOUT_READ_ONLY_REQ";

    row = *(m_refTreeModel_Outgoing->append());
    row[m_Columns_Messages.m_col_msg_id] = MessageType::UNSET;
    row[m_Columns_Messages.m_col_id] = "10";
    row[m_Columns_Messages.m_col_name] = "Control";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::CONTROL_GET_CONTACT_LIST_REQ;
    childRow[m_Columns_Messages.m_col_id] = "10.1";
    childRow[m_Columns_Messages.m_col_name] = "CONTROL_GET_CONTACT_LIST_REQ";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::CONTROL_GET_SWITCH_STAND_LIST_REQ;
    childRow[m_Columns_Messages.m_col_id] = "10.3";
    childRow[m_Columns_Messages.m_col_name] = "CONTROL_GET_SWITCH_STATE_LIST_REQ";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::CONTROL_GET_TRAIN_LIST_REQ;
    childRow[m_Columns_Messages.m_col_id] = "10.5";
    childRow[m_Columns_Messages.m_col_name] = "CONTROL_GET_TRAIN_LIST_REQ";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::CONTROL_LOCK_BLOCK;
    childRow[m_Columns_Messages.m_col_id] = "10.7";
    childRow[m_Columns_Messages.m_col_name] = "CONTROL_LOCK_BLOCK";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::CONTROL_LOCK_BLOCK_WAITING;
    childRow[m_Columns_Messages.m_col_id] = "10.8";
    childRow[m_Columns_Messages.m_col_name] = "CONTROL_LOCK_BLOCK_WAITING";

    childRow = *(m_refTreeModel_Outgoing->append(row.children()));
    childRow[m_Columns_Messages.m_col_msg_id] = MessageType::CONTROL_UNLOCK_BLOCK;
    childRow[m_Columns_Messages.m_col_id] = "10.11";
    childRow[m_Columns_Messages.m_col_name] = "CONTROL_UNLOCK_BLOCK";
}

void FrmMain::initIncomming() {
    m_HPaned.set_end_child(m_VPaned_Incomming);
    m_VPaned_Incomming.set_position(150);
    m_VPaned_Incomming.set_start_child(m_VBox_Incomming);
    m_VBox_Incomming.append(m_ScrolledWindow_Incomming);
    m_ScrolledWindow_Incomming.set_expand(true);
    m_ScrolledWindow_Incomming.set_child(m_TreeView_Incomming);
    m_ScrolledWindow_Incomming.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);

    m_refTreeModel_Incomming = Gtk::ListStore::create(m_Columns_Incomming);
    m_TreeView_Incomming.set_model(m_refTreeModel_Incomming);

    m_TreeView_Incomming.append_column("Timestamp", m_Columns_Incomming.m_col_timestamp);
    m_TreeView_Incomming.append_column("Gruppe",    m_Columns_Incomming.m_col_grp_name);
    m_TreeView_Incomming.append_column("Nachricht", m_Columns_Incomming.m_col_msg_name);

    m_VPaned_Incomming.set_end_child(m_ScrolledWindow_Data);

    m_ScrolledWindow_Data.set_child(m_Label_Data);
    m_ScrolledWindow_Data.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);

    m_VBox_Incomming.append(m_HBox_CheckRow);
    m_HBox_CheckRow.append(m_Button_AutoCheckLast);
    m_HBox_CheckRow.append(m_ButtonBox_Incomming);
    m_Button_AutoCheckLast.set_label("letzten Eintrag markieren");

    m_ButtonBox_Incomming.append(m_Button_ClearIncomming);
   // m_ButtonBox_Incomming.set_layout(Gtk::BUTTONBOX_END);

    Glib::RefPtr<Gtk::TreeSelection> refTreeSelection = m_TreeView_Incomming.get_selection();
    refTreeSelection->signal_changed().connect(sigc::mem_fun(*this, &FrmMain::on_selection_changed_incomming));
    m_Button_ClearIncomming.signal_clicked().connect(sigc::mem_fun(*this, &FrmMain::on_button_clear_incomming_clicked));
}

void FrmMain::initOutgoing() {
    m_HPaned.set_start_child(m_VPaned_Outgoing);
    m_VPaned_Outgoing.set_start_child(m_ScrolledWindow_Outgoing);
    m_ScrolledWindow_Outgoing.set_child(m_TreeView_Outgoing);
    m_ScrolledWindow_Outgoing.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);

    m_TreeView_Outgoing.append_column("ID", m_Columns_Messages.m_col_id);
    m_TreeView_Outgoing.append_column("Name", m_Columns_Messages.m_col_name);

    Glib::RefPtr<Gtk::TreeSelection> refTreeSelection = m_TreeView_Outgoing.get_selection();
    refTreeSelection->signal_changed().connect(sigc::mem_fun(*this, &FrmMain::on_selection_changed_outgoing));

    m_VPaned_Outgoing.set_end_child(m_VBox_Outgoing);
    m_VBox_Outgoing.append(m_ScrolledWindow_Outgoing_Data);
    m_ScrolledWindow_Outgoing_Data.set_expand(true);
    m_ScrolledWindow_Outgoing_Data.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
    m_VBox_Outgoing.append(m_ButtonBox_Outgoing);
    m_ButtonBox_Outgoing.append(m_Button_Send);
    m_Button_Send.signal_clicked().connect(sigc::mem_fun(*this, &FrmMain::on_button_send_clicked));
}

void FrmMain::on_button_send_clicked() {
    try {
        msgSender.sendActiveMessage();
    } catch(std::exception &e) {
        std::cout << e.what() << std::endl; // TODO: Make it better
    }
}

void FrmMain::on_button_about_clicked() {
    m_Dialog.show();
    m_Dialog.present();
}

void FrmMain::on_button_emergency_clicked() {
    if(m_Button_Emergency.get_label() == "Nothalt") {
        msgEndpoint->sendMsg(SystemTriggerEmergencyStop{});
    } else {
        msgEndpoint->sendMsg(SystemReleaseEmergencyStop{});
    }
}

void FrmMain::on_about_dialog_response(int) {
    m_Dialog.hide();
}

bool FrmMain::on_timeout(int) {
    static bool connected = false;

    try {
        if(!connected) {
            msgEndpoint->connect();
            m_Label_Connectivity_HW.set_markup("<span style=\"italic\">\xe2\x96\x84</span>");
            m_Label_Connectivity_HW.set_markup("<span color=\"red\"> \xe2\x96\x84</span>");
            m_Label_Connectivity_HW.set_tooltip_markup("<b>Status:</b> Keine Verbindung zur Hardware");
            m_Label_Connectivity_SW.set_markup("<span color=\"red\"> \xe2\x96\x84</span>");
            m_Label_Connectivity_SW.set_tooltip_markup("<b>Status:</b> Keine Verbindung zur Hardware");

            msgEndpoint->sendMsg(SystemGetHardwareState{});
            connected = true;
            return true;
        }
        registry.handleMsg(msgEndpoint->receiveMsg());

    } catch(std::exception &e) {
        if(connected) {
            m_Label_Connectivity_HW.set_markup("<span color=\"gray\"> \xe2\x96\x84</span>");
            m_Label_Connectivity_HW.set_tooltip_markup("<b>Status:</b> Keine Verbindung zum Server");
            m_Label_Connectivity_SW.set_markup("<span color=\"gray\"> \xe2\x96\x84</span>");
            m_Label_Connectivity_SW.set_tooltip_markup("<b>Status:</b> Keine Verbindung zum Server");
            m_Button_Emergency.set_sensitive(false);
            /*
            Gtk::MessageDialog dialog(
                *this,
                "msg-handler exception:",
                false,
                Gtk::MESSAGE_ERROR,
                Gtk::BUTTONS_OK
            );
            */
          //  dialog.set_secondary_text(e.what());
        //    dialog.run();
            connected = false;
        }
    }
    return true;
}

void FrmMain::msgHandler(std::uint32_t grpId, std::uint32_t msgId, const nlohmann::json &data) {
    std::time_t time = std::time({});
    char timeString[std::size("yyyy-mm-dd hh:mm:ss")];
    std::strftime(std::data(timeString), std::size(timeString), "%FT%TZ", std::gmtime(&time));

    Gtk::TreeModel::iterator iter = m_refTreeModel_Incomming->append();
    Gtk::TreeModel::Row row = *iter;
    row[m_Columns_Incomming.m_col_timestamp] = std::string(timeString);
    row[m_Columns_Incomming.m_col_grp_name ] = static_cast<int>(grpId);
    row[m_Columns_Incomming.m_col_msg_name ] = static_cast<int>(msgId);
    row[m_Columns_Incomming.m_col_data     ] = data.dump(2);

    if(m_Button_AutoCheckLast.get_active()) {
        Glib::RefPtr<Gtk::TreeSelection> selection = m_TreeView_Incomming.get_selection();
        selection->select(iter);
    }
}

void FrmMain::on_selection_changed_incomming() {
    Glib::RefPtr<Gtk::TreeSelection> selection = m_TreeView_Incomming.get_selection();
    Gtk::TreeModel::iterator iter = selection->get_selected();
    if(!iter) {
        return;
    }
    Gtk::TreeModel::Row row = *iter;

    std::stringstream ss;
    std::string s = row[m_Columns_Incomming.m_col_data];

    replace(s, "<", "&lt;");
    replace(s, ">", "&gt;");

    ss << "<span font_family=\"Courier New\">" << s << "</span>";
    m_Label_Data.set_markup(ss.str());
}

void FrmMain::on_selection_changed_outgoing() {
    Glib::RefPtr<Gtk::TreeSelection> selection = m_TreeView_Outgoing.get_selection();
    Gtk::TreeModel::iterator iter = selection->get_selected();

    if(!iter) {
        m_Button_Send.set_sensitive(false);
        return;
    }

    Gtk::TreeModel::Row row = *iter;

    if(row[m_Columns_Messages.m_col_msg_id] == MessageType::UNSET) {
      //  m_ScrolledWindow_Outgoing_Data.remove_with_viewport();
        m_Button_Send.set_sensitive(false);
        return;
    }

    auto str = std::string(row[m_Columns_Messages.m_col_id]);
    auto pos = str.find(".");

    m_Button_Send.set_sensitive(true);
    msgSender.setActiveMessage(
        static_cast<MessageType>(row[m_Columns_Messages.m_col_msg_id]),
        std::string(row[m_Columns_Messages.m_col_name]),
        std::stol(str.substr(0, pos)),
        std::stol(str.substr(pos + 1)),
        m_ScrolledWindow_Outgoing_Data
    );
}

void FrmMain::on_button_clear_incomming_clicked() {
    m_refTreeModel_Incomming->clear();
}

void FrmMain::setHardwareState(const SystemHardwareStateChanged &data) {
    if(data.hardwareState == SystemHardwareStateChanged::HardwareState::ERROR) {
        m_Label_Connectivity_HW.set_tooltip_markup("<b>Status:</b> Keine Verbindung zur Hardware");
        m_Label_Connectivity_SW.set_tooltip_markup("<b>Status:</b> Keine Verbindung zur Hardware");
        systemState = SystemState::ERROR;
        m_Button_Emergency.set_sensitive(false);
        return;
    }
    m_Button_Emergency.set_sensitive(true);
    if(data.hardwareState == SystemHardwareStateChanged::HardwareState::EMERGENCY_STOP) {
        m_Label_Connectivity_HW.set_tooltip_markup("<b>Status:</b> Nohalt ausgelöst");
        m_Label_Connectivity_SW.set_tooltip_markup("<b>Status:</b> Nohalt ausgelöst");
        systemState = SystemState::EMERGENCY_STOP;
        m_Button_Emergency.set_label("Freigabe");
        return;
    }
    m_Button_Emergency.set_label("Nothalt");
    if(data.hardwareState == SystemHardwareStateChanged::HardwareState::STANDBY) {
        m_Label_Connectivity_HW.set_tooltip_markup("<b>Status:</b> Energiesparmodus");
        m_Label_Connectivity_SW.set_tooltip_markup("<b>Status:</b> Energiesparmodus");
        systemState = SystemState::STANDBY;
        m_Button_Emergency.set_sensitive(false);
    }

    if(data.hardwareState == SystemHardwareStateChanged::HardwareState::MANUEL) {
        m_Label_Connectivity_HW.set_tooltip_markup("<b>Status:</b> manuell");
        m_Label_Connectivity_SW.set_tooltip_markup("<b>Status:</b> manuell");
        systemState = SystemState::MANUEL;
    }
    if(data.hardwareState == SystemHardwareStateChanged::HardwareState::AUTOMATIC) {
        m_Label_Connectivity_HW.set_tooltip_markup("<b>Status:</b> automatisch");
        m_Label_Connectivity_SW.set_tooltip_markup("<b>Status:</b> automatisch");
        systemState = SystemState::AUTOMATIC;
    }
}

bool FrmMain::on_timeout_status(int) {
    static bool on = false;

    on = !on;
    switch(systemState) {
        case SystemState::NO_CONNECT:
            if(on) {
                m_Label_Connectivity_SW.set_markup("<span color=\"red\"> \xe2\x96\x84</span>");
            } else {
                m_Label_Connectivity_SW.set_markup("<span color=\"gray\"> \xe2\x96\x84</span>");
            }
            m_Label_Connectivity_HW.set_markup("<span color=\"gray\"> \xe2\x96\x84</span>");
            break;

        case SystemState::ERROR:
            m_Label_Connectivity_SW.set_markup("<span color=\"green\"> \xe2\x96\x84</span>");
            if(on) {
                m_Label_Connectivity_HW.set_markup("<span color=\"red\"> \xe2\x96\x84</span>");
            } else {
                m_Label_Connectivity_HW.set_markup("<span color=\"gray\"> \xe2\x96\x84</span>");
            }
            break;

        case SystemState::STANDBY:
            m_Label_Connectivity_SW.set_markup("<span color=\"green\"> \xe2\x96\x84</span>");
            if(on) {
                m_Label_Connectivity_HW.set_markup("<span color=\"gold\"> \xe2\x96\x84</span>");
            } else {
                m_Label_Connectivity_HW.set_markup("<span color=\"gray\"> \xe2\x96\x84</span>");
            }
            break;

        case SystemState::EMERGENCY_STOP:
            if(on) {
                m_Label_Connectivity_HW.set_markup("<span color=\"gold\"> \xe2\x96\x84</span>");
                m_Label_Connectivity_SW.set_markup("<span color=\"gold\"> \xe2\x96\x84</span>");
            } else {
                m_Label_Connectivity_HW.set_markup("<span color=\"gray\"> \xe2\x96\x84</span>");
                m_Label_Connectivity_SW.set_markup("<span color=\"gray\"> \xe2\x96\x84</span>");
            }
            break;

        case SystemState::MANUEL:
            m_Label_Connectivity_HW.set_markup("<span color=\"green\"> \xe2\x96\x84</span>");
            if(on) {
                m_Label_Connectivity_SW.set_markup("<span color=\"green\"> \xe2\x96\x84</span>");
            } else {
                m_Label_Connectivity_SW.set_markup("<span color=\"gray\"> \xe2\x96\x84</span>");
            }
            break;

        case SystemState::AUTOMATIC:
            m_Label_Connectivity_HW.set_markup("<span color=\"green\"> \xe2\x96\x84</span>");
            m_Label_Connectivity_SW.set_markup("<span color=\"green\"> \xe2\x96\x84</span>");
            break;
    }
    return true;

    /*
                SW              HW
    -           rot / blink     grau
    ERROR       grün            rot / blink
    STANDBY     grün            gelb / blink
    EMERGENCY   gelb / blink    gelb / blink
    MANUELL     grün / blink    grün
    AUTOMATIC   grün            grün
     */
}

