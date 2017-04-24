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

#include "frmmain.h"

#include "config.h"

#include <ctime>
#include <sys/timeb.h>
#include <iostream>
#include <boost/algorithm/string.hpp>

namespace {
    const char license[] =
        "Project:    moba-systemmanager\n"
        "\n"
        "Copyright (C) 2016 Stefan Paproth <pappi-@gmx.de>\n"
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
        "along with this program. If not, see <http://www.gnu.org/licenses/agpl.txt>.";
}

FrmMain::FrmMain(moba::MsgEndpointPtr mhp) :
    msgEndpoint(mhp), msgSender(mhp), m_VBox(Gtk::ORIENTATION_VERTICAL, 6),
    m_HPaned(Gtk::ORIENTATION_HORIZONTAL), m_Label_Connectivity(" \xe2\x8f\xb9"),
    m_VBox_Outgoing(Gtk::ORIENTATION_VERTICAL, 6), m_Button_About("About..."), m_Button_Send("Senden..."),
    m_VPaned_Incomming(Gtk::ORIENTATION_VERTICAL), m_VPaned_Outgoing(Gtk::ORIENTATION_VERTICAL),
    m_VBox_Incomming(Gtk::ORIENTATION_VERTICAL, 6), m_Button_ClearIncomming("Leeren"),
    m_HBox(Gtk::ORIENTATION_HORIZONTAL, 6), m_HBox_CheckRow(Gtk::ORIENTATION_HORIZONTAL, 6)
{
    sigc::slot<bool> my_slot = sigc::bind(sigc::mem_fun(*this, &FrmMain::on_timeout), 1);
    sigc::connection conn = Glib::signal_timeout().connect(my_slot, 25); // 25 ms
    add(m_VBox);
    m_VBox.pack_start(m_HPaned);

    // about-dialog
    m_VBox.pack_start(m_HBox, Gtk::PACK_SHRINK);
    m_HBox.pack_end(m_ButtonBox, Gtk::PACK_SHRINK);
    m_HBox.pack_start(m_Label_Connectivity, Gtk::PACK_SHRINK);
    m_Label_Connectivity.set_justify(Gtk::JUSTIFY_LEFT);
    m_Label_Connectivity.override_color(Gdk::RGBA("green"), Gtk::STATE_FLAG_NORMAL);

    m_ButtonBox.pack_start(m_Button_About, Gtk::PACK_EXPAND_WIDGET, 5);
    m_ButtonBox.set_layout(Gtk::BUTTONBOX_END);
    m_Button_About.signal_clicked().connect(sigc::mem_fun(*this, &FrmMain::on_button_about_clicked));

    m_ButtonBox.pack_start(m_Button_Emegerency, Gtk::PACK_EXPAND_WIDGET, 5);
    m_Button_Emegerency.signal_clicked().connect(sigc::mem_fun(*this, &FrmMain::on_button_emegency_clicked));
    m_Button_Emegerency.set_label("Nothalt");

    m_HPaned.set_position(400);
    initAboutDialog();
    initTreeModel();
    initOutgoing();
    initIncomming();

    m_Button_Send.set_sensitive(false);
    msgEndpoint->sendMsg(moba::Message::MT_GET_EMERGENCY_STOP_STATE);
    show_all_children();
}

void FrmMain::initAboutDialog() {
    m_Dialog.set_transient_for(*this);

    m_Dialog.set_program_name(PACKAGE_NAME);
    m_Dialog.set_version(PACKAGE_VERSION);
    m_Dialog.set_copyright("Stefan Paproth");
    m_Dialog.set_comments("This is just an application for testing purpose.");
    m_Dialog.set_license(license);

    m_Dialog.set_website("<pappi-@gmx.de>");
    m_Dialog.set_website_label("pappi-@gmx.de");

    std::vector<Glib::ustring> list_authors;
    list_authors.push_back("Stefan Paproth");
    m_Dialog.set_authors(list_authors);

    m_Dialog.signal_response().connect(sigc::mem_fun(*this, &FrmMain::on_about_dialog_response));

    m_Button_About.grab_focus();
}

void FrmMain::initTreeModel(){
    m_refTreeModel_Outgoing = Gtk::TreeStore::create(m_Columns_Messages);
    m_TreeView_Outgoing.set_model(m_refTreeModel_Outgoing);

    Gtk::TreeModel::Row row = *(m_refTreeModel_Outgoing->append());
    row[m_Columns_Messages.m_col_msg_id] = -1;
    row[m_Columns_Messages.m_col_id] = "2";
    row[m_Columns_Messages.m_col_name] = "Client";

    Gtk::TreeModel::Row childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_VOID;
    childrow[m_Columns_Messages.m_col_id] = "2.1";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_VOID);

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_ECHO_REQ;
    childrow[m_Columns_Messages.m_col_id] = "2.2";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_ECHO_REQ);

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_CLIENT_START;
    childrow[m_Columns_Messages.m_col_id] = "2.5";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_CLIENT_START);

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_CLIENT_CLOSE;
    childrow[m_Columns_Messages.m_col_id] = "2.7";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_CLIENT_CLOSE);

    row = *(m_refTreeModel_Outgoing->append());
    row[m_Columns_Messages.m_col_msg_id] = -1;
    row[m_Columns_Messages.m_col_id] = "3";
    row[m_Columns_Messages.m_col_name] = "Server";

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_RESET_CLIENT;
    childrow[m_Columns_Messages.m_col_id] = "3.4";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_RESET_CLIENT);

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_SERVER_INFO_REQ;
    childrow[m_Columns_Messages.m_col_id] = "3.5";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_SERVER_INFO_REQ);

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_CON_CLIENTS_REQ;
    childrow[m_Columns_Messages.m_col_id] = "3.7";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_CON_CLIENTS_REQ);

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_SELF_TESTING_CLIENT;
    childrow[m_Columns_Messages.m_col_id] = "3.9";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_SELF_TESTING_CLIENT);

    row = *(m_refTreeModel_Outgoing->append());
    row[m_Columns_Messages.m_col_msg_id] = -1;
    row[m_Columns_Messages.m_col_id] = "4";
    row[m_Columns_Messages.m_col_name] = "Timer";

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_GET_GLOBAL_TIMER;
    childrow[m_Columns_Messages.m_col_id] = "4.2";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_GET_GLOBAL_TIMER);

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_SET_GLOBAL_TIMER;
    childrow[m_Columns_Messages.m_col_id] = "4.3";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_SET_GLOBAL_TIMER);

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_GET_AUTO_MODE;
    childrow[m_Columns_Messages.m_col_id] = "4.4";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_GET_AUTO_MODE);

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_SET_AUTO_MODE;
    childrow[m_Columns_Messages.m_col_id] = "4.5";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_SET_AUTO_MODE);

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_GET_COLOR_THEME;
    childrow[m_Columns_Messages.m_col_id] = "4.6";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_GET_COLOR_THEME);

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_SET_COLOR_THEME;
    childrow[m_Columns_Messages.m_col_id] = "4.7";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_SET_COLOR_THEME);

    row = *(m_refTreeModel_Outgoing->append());
    row[m_Columns_Messages.m_col_msg_id] = -1;
    row[m_Columns_Messages.m_col_id] = "5";
    row[m_Columns_Messages.m_col_name] = "Env";

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_GET_ENVIRONMENT;
    childrow[m_Columns_Messages.m_col_id] = "5.1";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_GET_ENVIRONMENT);

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_SET_ENVIRONMENT;
    childrow[m_Columns_Messages.m_col_id] = "5.2";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_SET_ENVIRONMENT);

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_GET_AMBIENCE;
    childrow[m_Columns_Messages.m_col_id] = "5.3";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_GET_AMBIENCE);

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_SET_AMBIENCE;
    childrow[m_Columns_Messages.m_col_id] = "5.4";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_SET_AMBIENCE);

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_GET_AMBIENT_LIGHT;
    childrow[m_Columns_Messages.m_col_id] = "5.5";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_GET_AMBIENT_LIGHT);

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_SET_AMBIENT_LIGHT;
    childrow[m_Columns_Messages.m_col_id] = "5.6";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_SET_AMBIENT_LIGHT);

    row = *(m_refTreeModel_Outgoing->append());
    row[m_Columns_Messages.m_col_msg_id] = -1;
    row[m_Columns_Messages.m_col_id] = "6";
    row[m_Columns_Messages.m_col_name] = "System";

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_GET_EMERGENCY_STOP_STATE;
    childrow[m_Columns_Messages.m_col_id] = "6.1";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_GET_EMERGENCY_STOP_STATE);

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_EMERGENCY_STOP;
    childrow[m_Columns_Messages.m_col_id] = "6.2";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_EMERGENCY_STOP);

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_EMERGENCY_STOP_CLEARING;
    childrow[m_Columns_Messages.m_col_id] = "6.3";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_EMERGENCY_STOP_CLEARING);

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_GET_HARDWARE_STATE;
    childrow[m_Columns_Messages.m_col_id] = "6.4";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_GET_HARDWARE_STATE);

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_SET_HARDWARE_STATE;
    childrow[m_Columns_Messages.m_col_id] = "6.5";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_SET_HARDWARE_STATE);

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_HARDWARE_SHUTDOWN;
    childrow[m_Columns_Messages.m_col_id] = "6.7";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_HARDWARE_SHUTDOWN);

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_HARDWARE_RESET;
    childrow[m_Columns_Messages.m_col_id] = "6.8";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_HARDWARE_RESET);

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_HARDWARE_SWITCH_STANDBY;
    childrow[m_Columns_Messages.m_col_id] = "6.9";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_HARDWARE_SWITCH_STANDBY);

    row = *(m_refTreeModel_Outgoing->append());
    row[m_Columns_Messages.m_col_msg_id] = -1;
    row[m_Columns_Messages.m_col_id] = "7";
    row[m_Columns_Messages.m_col_name] = "Layouts";

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_GET_LAYOUTS_REQ;
    childrow[m_Columns_Messages.m_col_id] = "7.1";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_GET_LAYOUTS_REQ);

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_DEL_LAYOUT;
    childrow[m_Columns_Messages.m_col_id] = "7.3";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_DEL_LAYOUT);

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_CREATE_LAYOUT_REQ;
    childrow[m_Columns_Messages.m_col_id] = "7.5";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_CREATE_LAYOUT_REQ);

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_UPDATE_LAYOUT;
    childrow[m_Columns_Messages.m_col_id] = "7.8";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_UPDATE_LAYOUT);

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_UNLOCK_LAYOUT;
    childrow[m_Columns_Messages.m_col_id] = "7.10";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_UNLOCK_LAYOUT);

    row = *(m_refTreeModel_Outgoing->append());
    row[m_Columns_Messages.m_col_msg_id] = -1;
    row[m_Columns_Messages.m_col_id] = "8";
    row[m_Columns_Messages.m_col_name] = "Layout";

    childrow = *(m_refTreeModel_Outgoing->append(row.children()));
    childrow[m_Columns_Messages.m_col_msg_id] = moba::Message::MT_GET_LAYOUT_REQ;
    childrow[m_Columns_Messages.m_col_id] = "8.1";
    childrow[m_Columns_Messages.m_col_name] = *moba::Message::convertToString(moba::Message::MT_GET_LAYOUT_REQ);
}

void FrmMain::initIncomming() {
    m_HPaned.add2(m_VPaned_Incomming);
    m_VPaned_Incomming.set_position(150);
    m_VPaned_Incomming.add1(m_VBox_Incomming);
    m_VBox_Incomming.pack_start(m_ScrolledWindow_Incomming);
    m_ScrolledWindow_Incomming.add(m_TreeView_Incomming);
    m_ScrolledWindow_Incomming.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

    m_refTreeModel_Incomming = Gtk::ListStore::create(m_Columns_Incomming);
    m_TreeView_Incomming.set_model(m_refTreeModel_Incomming);

    m_TreeView_Incomming.append_column("Timestamp", m_Columns_Incomming.m_col_timestamp);
    m_TreeView_Incomming.append_column("ID",        m_Columns_Incomming.m_col_id);
    m_TreeView_Incomming.append_column("Name",      m_Columns_Incomming.m_col_name);

    m_VPaned_Incomming.add2(m_ScrolledWindow_Data);

    m_ScrolledWindow_Data.add(m_Label_Data);
    m_ScrolledWindow_Data.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

    m_VBox_Incomming.pack_end(m_HBox_CheckRow, Gtk::PACK_SHRINK);
    m_HBox_CheckRow.pack_start(m_Button_AutoCheckLast, Gtk::PACK_SHRINK);
    m_HBox_CheckRow.pack_end(m_ButtonBox_Incomming, Gtk::PACK_SHRINK);
    m_Button_AutoCheckLast.set_label("letzten Eintrag markieren");

    m_ButtonBox_Incomming.pack_start(m_Button_ClearIncomming, Gtk::PACK_EXPAND_WIDGET, 5);
    m_ButtonBox_Incomming.set_layout(Gtk::BUTTONBOX_END);

    Glib::RefPtr<Gtk::TreeSelection> refTreeSelection = m_TreeView_Incomming.get_selection();
    refTreeSelection->signal_changed().connect(sigc::mem_fun(*this, &FrmMain::on_selection_changed_incomming));
    m_Button_ClearIncomming.signal_clicked().connect(sigc::mem_fun(*this, &FrmMain::on_button_clear_incomming_clicked));
}

void FrmMain::initOutgoing() {
    m_HPaned.add1(m_VPaned_Outgoing);
    m_VPaned_Outgoing.set_position(150);
    m_VPaned_Outgoing.add1(m_ScrolledWindow_Outgoing);
    m_ScrolledWindow_Outgoing.add(m_TreeView_Outgoing);
    m_ScrolledWindow_Outgoing.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

    m_TreeView_Outgoing.append_column("ID", m_Columns_Messages.m_col_id);
    m_TreeView_Outgoing.append_column("Name", m_Columns_Messages.m_col_name);

    Glib::RefPtr<Gtk::TreeSelection> refTreeSelection = m_TreeView_Outgoing.get_selection();
    refTreeSelection->signal_changed().connect(sigc::mem_fun(*this, &FrmMain::on_selection_changed_outgoing));

    m_VPaned_Outgoing.add2(m_VBox_Outgoing);
    m_VBox_Outgoing.pack_start(m_ScrolledWindow_Outgoing_Data);
    m_ScrolledWindow_Outgoing_Data.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    m_VBox_Outgoing.pack_start(m_ButtonBox_Outgoing, Gtk::PACK_SHRINK);
    m_ButtonBox_Outgoing.pack_start(m_Button_Send, Gtk::PACK_EXPAND_WIDGET, 5);
    m_ButtonBox_Outgoing.set_layout(Gtk::BUTTONBOX_END);
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

void FrmMain::on_button_emegency_clicked() {
    if(m_Button_Emegerency.get_label() == "Nothalt") {
        msgEndpoint->sendMsg(moba::Message::MT_EMERGENCY_STOP);
    } else {
        msgEndpoint->sendMsg(moba::Message::MT_EMERGENCY_STOP_CLEARING);
    }
}

void FrmMain::on_about_dialog_response(int) {
    m_Dialog.hide();
}

bool FrmMain::on_timeout(int) {
   static bool connected = true;
    moba::MessagePtr msg;

    try {
        if(!connected) {
            msgEndpoint->connect();
            m_Label_Connectivity.override_color(Gdk::RGBA("green"), Gtk::STATE_FLAG_NORMAL);
            connected = true;
            return true;
        }
        msg = msgEndpoint->recieveMsg();
    } catch(std::exception &e) {
        if(connected) {
            m_Label_Connectivity.override_color(Gdk::RGBA("red"), Gtk::STATE_FLAG_NORMAL);
            Gtk::MessageDialog dialog(
                *this,
                "msg-handler exception:",
                false,
                Gtk::MESSAGE_ERROR,
                Gtk::BUTTONS_OK
            );
            dialog.set_secondary_text(e.what());
            dialog.run();
            connected = false;
        }
        return true;
    }

    if(!msg) {
        return true;
    }

    switch(msg->getMsgType()) {
        case moba::Message::MT_EMERGENCY_STOP:
            m_Button_Emegerency.set_label("Freigabe");
            break;

        case moba::Message::MT_EMERGENCY_STOP_CLEARING:
            m_Button_Emegerency.set_label("Nothalt");
            break;
    }

    std::stringstream ss;
    timeb sTimeB;
    char buffer[25] = "";

    ftime(&sTimeB);
    strftime(buffer, 21, "%d.%m.%Y %H:%M:%S", localtime(&sTimeB.time));
    moba::prettyPrint(msg->getData(), ss);

    Gtk::TreeModel::iterator iter = m_refTreeModel_Incomming->append();
    Gtk::TreeModel::Row row = *iter;
    row[m_Columns_Incomming.m_col_timestamp] = std::string(buffer);
    row[m_Columns_Incomming.m_col_id       ] = msg->getMsgType();
    row[m_Columns_Incomming.m_col_name     ] = msg->msgTypeAsString();
    row[m_Columns_Incomming.m_col_data     ] = ss.str();

    if(m_Button_AutoCheckLast.get_active()) {
        Glib::RefPtr<Gtk::TreeSelection> selection = m_TreeView_Incomming.get_selection();
        selection->select(iter);
    }
    return true;
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
    boost::replace_all(s, "<", "&lt;");
    boost::replace_all(s, ">", "&gt;");
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

    if(row[m_Columns_Messages.m_col_msg_id] == -1) {
        m_ScrolledWindow_Outgoing_Data.remove_with_viewport();
        m_Button_Send.set_sensitive(false);
        return;

    }
    m_Button_Send.set_sensitive(true);
    msgSender.setActiveMessage(
        static_cast<moba::Message::MessageType>((int)row[m_Columns_Messages.m_col_msg_id]),
        m_ScrolledWindow_Outgoing_Data
    );
}

void FrmMain::on_button_clear_incomming_clicked() {
    m_refTreeModel_Incomming->clear();
}