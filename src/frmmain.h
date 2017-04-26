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

#include <moba/msgendpoint.h>
#include <moba/systemhandler.h>

#include <gtkmm/window.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/liststore.h>

#include "msgsender.h"

class FrmMain : public Gtk::Window {
    public:
        FrmMain(moba::MsgEndpointPtr mhp);
        virtual ~FrmMain();

    protected:
        Gtk::Box       m_VBox{Gtk::ORIENTATION_VERTICAL, 6};
        Gtk::Paned     m_HPaned{Gtk::ORIENTATION_HORIZONTAL};
        Gtk::ButtonBox m_ButtonBox;
        Gtk::Button    m_Button_Emegerency;
        Gtk::Box       m_HBox{Gtk::ORIENTATION_HORIZONTAL, 6};
        Gtk::Label     m_Label_Connectivity{" \xe2\x8f\xb9"};

        // about
        Gtk::Button      m_Button_About{"About..."};
        Gtk::AboutDialog m_Dialog;

        // Outgoing
        Gtk::Paned          m_VPaned_Outgoing{Gtk::ORIENTATION_VERTICAL};
        Gtk::ScrolledWindow m_ScrolledWindow_Outgoing;

        Gtk::TreeView                m_TreeView_Outgoing;
        Glib::RefPtr<Gtk::TreeStore> m_refTreeModel_Outgoing;

        Gtk::ScrolledWindow m_ScrolledWindow_Outgoing_Data;
        Gtk::Box            m_VBox_Outgoing{Gtk::ORIENTATION_VERTICAL, 6};
        Gtk::ButtonBox      m_ButtonBox_Outgoing;
        Gtk::Button         m_Button_Send{"Senden..."};

        class ModelColumnsMessages : public Gtk::TreeModelColumnRecord {
            public:
                ModelColumnsMessages() {
                    add(m_col_name);
                    add(m_col_id);
                    add(m_col_msg_id);
                }

                Gtk::TreeModelColumn<Glib::ustring> m_col_name;
                Gtk::TreeModelColumn<Glib::ustring> m_col_id;
                Gtk::TreeModelColumn<int>           m_col_msg_id;
        };

        ModelColumnsMessages m_Columns_Messages;

        // Incoming
        Gtk::Paned                   m_VPaned_Incomming{Gtk::ORIENTATION_VERTICAL};
        Gtk::ScrolledWindow          m_ScrolledWindow_Incomming;
        Gtk::TreeView                m_TreeView_Incomming;
        Glib::RefPtr<Gtk::ListStore> m_refTreeModel_Incomming;

        Gtk::Label          m_Label_Data;
        Gtk::ScrolledWindow m_ScrolledWindow_Data;

        Gtk::Box         m_VBox_Incomming{Gtk::ORIENTATION_VERTICAL, 6};
        Gtk::ButtonBox   m_ButtonBox_Incomming;
        Gtk::Button      m_Button_ClearIncomming{"Leeren"};
        Gtk::Box         m_HBox_CheckRow{Gtk::ORIENTATION_HORIZONTAL, 6};
        Gtk::CheckButton m_Button_AutoCheckLast;

        class ModelColumnsIncomming : public Gtk::TreeModel::ColumnRecord {
            public:
                ModelColumnsIncomming() {
                    add(m_col_timestamp);
                    add(m_col_id);
                    add(m_col_name);
                    add(m_col_data);
                }

                Gtk::TreeModelColumn<Glib::ustring> m_col_timestamp;
                Gtk::TreeModelColumn<int>           m_col_id;
                Gtk::TreeModelColumn<Glib::ustring> m_col_name;
                Gtk::TreeModelColumn<std::string>   m_col_data;
        };

        ModelColumnsIncomming m_Columns_Incomming;

        void initAboutDialog();
        void initTreeModel();
        void initIncomming();
        void initOutgoing();

        moba::MsgEndpointPtr msgEndpoint;
        MsgSender            msgSender;

        // Signal handlers:
        bool on_timeout(int timer_number);
        void on_selection_changed_incomming();
        void on_button_emegency_clicked();
        void on_button_about_clicked();
        void on_button_send_clicked();
        void on_button_clear_incomming_clicked();
        void on_about_dialog_response(int response_id);
        void on_selection_changed_outgoing();
};

