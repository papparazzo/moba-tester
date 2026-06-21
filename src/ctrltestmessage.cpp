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

#include "ctrltestmessage.h"
#include <string>
#include <cctype>

namespace {
    // Entfernt alle Zeilenumbrüche und Leerzeichen ausserhalb von String-Literalen.
    // Whitespace innerhalb von "..." bleibt erhalten; escapte Anführungszeichen
    // (\") werden korrekt übersprungen.
    std::string minifyJson(const std::string& in) {
        std::string out;
        out.reserve(in.size());

        bool inString = false;
        bool escaped = false;

        for(char c : in) {
            if(inString) {
                out += c;
                if(escaped) {
                    escaped = false;
                } else if(c == '\\') {
                    escaped = true;
                } else if(c == '"') {
                    inString = false;
                }
                continue;
            }

            if(c == '"') {
                inString = true;
                out += c;
            } else if(!std::isspace(static_cast<unsigned char>(c))) {
                out += c;
            }
        }
        return out;
    }
}

CtrlTestMessage::CtrlTestMessage() {
    for(int i = 0; i < 2; ++i) {
        m_HBox[i].set_orientation(Gtk::Orientation::HORIZONTAL);
        m_HBox[i].append(m_Label[i]);
        m_HBox[i].append(m_Entry[i]);
        m_VBox.append(m_HBox[i]);
    }
    m_LabelMessage.set_label("Message");

    m_VBox.append(m_LabelMessage);
    m_VBox.append(m_ScrolledWindow);

    m_refTextBuffer = Gtk::TextBuffer::create();
    m_TextViewMessage.set_buffer(m_refTextBuffer);

    m_ScrolledWindow.set_child(m_TextViewMessage);
    m_ScrolledWindow.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
    m_ScrolledWindow.set_expand();

    m_Label[0].set_label("Group-Id");
    m_Label[1].set_label("Message-Id");
}

void CtrlTestMessage::send(const EndpointPtr& endpoint) const {
    std::string message = minifyJson(m_refTextBuffer->get_text().raw());

    endpoint->sendMsg(
        std::stol(m_Entry[0].get_text()),
        std::stol(m_Entry[1].get_text()),
        message
    );
}

void CtrlTestMessage::init(Gtk::ScrolledWindow &container) {
    container.set_child(m_VBox);
}
