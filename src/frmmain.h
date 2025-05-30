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

#include "moba/endpoint.h"
#include "moba/nlohmann/json.hpp"
#include "moba/systemmessages.h"
#include "moba/registry.h"
#include "moba/message.h"

#include <gtkmm/window.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/liststore.h>

#include "msgsender.h"

#include <atomic>

enum class SystemState {
    NO_CONNECT,
    ERROR,
    STANDBY,
    EMERGENCY_STOP,
    MANUEL,
    AUTOMATIC
};

class FrmMain : public Gtk::Window {
public:
    explicit FrmMain(EndpointPtr mhp);

    ~FrmMain() override = default;

protected:
    std::atomic<SystemState> systemState;

    // about
    Gtk::Button      m_Button_About{"About..."};
    Gtk::AboutDialog m_Dialog;

    // status
    Gtk::Label       m_Label_Connectivity_HW{" \xe2\x96\x84"};
    Gtk::Label       m_Label_Connectivity_SW{" \xe2\x96\x84"};

    Gtk::Box         m_HBox_Expander{Gtk::Orientation::HORIZONTAL, 6};

    Gtk::Box         m_HButtonBox{Gtk::Orientation::HORIZONTAL, 6};
    Gtk::Box         m_VBox{Gtk::Orientation::VERTICAL, 6};
    Gtk::Box         m_HBox_Status{Gtk::Orientation::HORIZONTAL, 6};

    Gtk::Button    m_Button_Emergency{"Nothalt"};

    Gtk::Paned     m_HPaned{Gtk::Orientation::HORIZONTAL};

    // Outgoing
    Gtk::Paned          m_VPaned_Outgoing{Gtk::Orientation::VERTICAL};
    Gtk::ScrolledWindow m_ScrolledWindow_Outgoing;

    Gtk::TreeView                m_TreeView_Outgoing;
    Glib::RefPtr<Gtk::TreeStore> m_refTreeModel_Outgoing;

    Gtk::ScrolledWindow m_ScrolledWindow_Outgoing_Data;
    Gtk::Box            m_VBox_Outgoing{Gtk::Orientation::VERTICAL, 6};
    Gtk::Box            m_ButtonBox_Outgoing;
    Gtk::Button         m_Button_Send{"Senden..."};

    class ModelColumnsMessages : public Gtk::TreeModelColumnRecord {
    public:
        ModelColumnsMessages() {
            add(m_col_name);
            add(m_col_id);
            add(m_col_msg_id);
        }

        Gtk::TreeModelColumn<std::string> m_col_name;
        Gtk::TreeModelColumn<std::string> m_col_id;
        Gtk::TreeModelColumn<MessageType> m_col_msg_id;
    };

    ModelColumnsMessages m_Columns_Messages;

    // Incoming
    Gtk::Paned                   m_VPaned_Incomming{Gtk::Orientation::VERTICAL};
    Gtk::ScrolledWindow          m_ScrolledWindow_Incomming;
    Gtk::TreeView                m_TreeView_Incomming;
    Glib::RefPtr<Gtk::ListStore> m_refTreeModel_Incomming;

    Gtk::Label          m_Label_Data;
    Gtk::ScrolledWindow m_ScrolledWindow_Data;

    Gtk::Box         m_VBox_Incomming{Gtk::Orientation::VERTICAL, 6};
    Gtk::Box         m_ButtonBox_Incomming;
    Gtk::Button      m_Button_ClearIncomming{"Leeren"};
    Gtk::Box         m_HBox_CheckRow{Gtk::Orientation::HORIZONTAL, 6};
    Gtk::CheckButton m_Button_AutoCheckLast;

    class ModelColumnsIncomming : public Gtk::TreeModel::ColumnRecord {
    public:
        ModelColumnsIncomming() {
            add(m_col_timestamp);
            add(m_col_grp_name);
            add(m_col_msg_name);
            add(m_col_data);
        }

        Gtk::TreeModelColumn<Glib::ustring> m_col_timestamp;
        Gtk::TreeModelColumn<int>           m_col_grp_name;
        Gtk::TreeModelColumn<int>           m_col_msg_name;
        Gtk::TreeModelColumn<std::string>   m_col_data;
    };

    ModelColumnsIncomming m_Columns_Incomming;

    void initAboutDialog();
    void initTreeModel();
    void initIncomming();
    void initOutgoing();

    void setHardwareState(const SystemHardwareStateChanged &data);

    EndpointPtr msgEndpoint;
    MsgSender   msgSender;
    Registry    registry;

    void msgHandler(std::uint32_t grpId, std::uint32_t msgId, const nlohmann::json &data);

    // Signal handlers:
    bool on_timeout(int timer_number);
    bool on_timeout_status(int);
    void on_selection_changed_incomming();
    void on_button_emergency_clicked();
    void on_button_about_clicked();
    void on_button_send_clicked();
    void on_button_clear_incomming_clicked();
    void on_about_dialog_response(int response_id);
    void on_selection_changed_outgoing();
};
