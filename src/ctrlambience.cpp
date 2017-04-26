#include "ctrlambience.h"

#include <string>

CtrlAmbience::CtrlAmbience() {
    for(int i = 0; i < LAST_ENTRY; ++i) {
        m_HBox[i].set_orientation(Gtk::ORIENTATION_HORIZONTAL);
        m_HBox[i].pack_start(m_Label[i], Gtk::PACK_SHRINK);
        m_HBox[i].pack_end(m_Combo[i], Gtk::PACK_SHRINK);
        m_VBox.pack_start(m_HBox[i], Gtk::PACK_SHRINK);
        m_Combo[i].append("on");
        m_Combo[i].append("off");
        m_Combo[i].append("unset");
    }
    m_Label[CURTAIN_UP   ].set_label("Rollo rauf / runter");
    m_Label[MAIN_LIGHT_ON].set_label("Hauptlicht an / aus");
}

CtrlAmbience::~CtrlAmbience() {
}

moba::JsonItemPtr CtrlAmbience::get_value() const {
    moba::JsonObjectPtr obj(new moba::JsonObject());
    (*obj)["curtainUp"   ] = getToggleState(CURTAIN_UP);
    (*obj)["mainLightOn" ] = getToggleState(MAIN_LIGHT_ON);
    return obj;
}

void CtrlAmbience::init(Gtk::ScrolledWindow &container) {
    for(int i = 0; i < LAST_ENTRY; ++i) {
        m_Combo[i].set_active(1);
    }
    container.add(m_VBox);
    container.show_all_children();
}

moba::JsonToggleStatePtr CtrlAmbience::getToggleState(CtrlAmbience::Entries entry) const {
    std::string str = m_Combo[entry].get_active_text();
    if(str == "on") {
        return moba::toJsonToggleStatePtr(moba::JsonToggleState::ON);
    }
    if(str == "off") {
        return moba::toJsonToggleStatePtr(moba::JsonToggleState::OFF);
    }
    return moba::toJsonToggleStatePtr(moba::JsonToggleState::UNSET);
}
