#include "ctrlambience.h"

#include <string>

CtrlAmbience::CtrlAmbience() {
    for(int i = 0; i < LAST_ENTRY; ++i) {
        m_HBox[i].set_orientation(Gtk::Orientation::HORIZONTAL);
        m_HBox[i].append(m_Label[i]);
        m_HBox[i].append(m_Combo[i]);
        m_VBox.append(m_HBox[i]);
        m_Combo[i].append("ON");
        m_Combo[i].append("OFF");
        m_Combo[i].append("UNSET");
    }
    m_Label[CURTAIN_UP   ].set_label("curtain (down = off)");
    m_Label[MAIN_LIGHT_ON].set_label("main-light");
}

CtrlAmbience::~CtrlAmbience() {
}

void CtrlAmbience::get_value(rapidjson::Document &d) const {
    d.SetObject();
    d.AddMember("curtainUp",   getToggleState(CURTAIN_UP, d), d.GetAllocator());
    d.AddMember("mainLightOn", getToggleState(MAIN_LIGHT_ON, d), d.GetAllocator());
}

void CtrlAmbience::init(Gtk::ScrolledWindow &container) {
    for(int i = 0; i < Entries::LAST_ENTRY; ++i) {
        m_Combo[i].set_active(1);
    }
    container.set_child(m_VBox);
    //container.show_all_children();
}

rapidjson::Value CtrlAmbience::getToggleState(CtrlAmbience::Entries entry, rapidjson::Document &d) const {
    std::string str = m_Combo[entry].get_active_text();
    return rapidjson::Value{str.c_str(), static_cast<unsigned int>(str.length()), d.GetAllocator()};

}