#include "ctrlenvironment.h"

#include <string>

CtrlEnvironment::CtrlEnvironment() {
    for(int i = 0; i < Entries::LAST_ENTRY; ++i) {
        m_HBox[i].set_orientation(Gtk::ORIENTATION_HORIZONTAL);
        m_HBox[i].pack_start(m_Label[i], Gtk::PACK_SHRINK);
        m_HBox[i].pack_end(m_Combo[i], Gtk::PACK_SHRINK);
        m_VBox.pack_start(m_HBox[i], Gtk::PACK_SHRINK);
        m_Combo[i].append("ON");
        m_Combo[i].append("OFF");
        m_Combo[i].append("TRIGGER");
        m_Combo[i].append("AUTO");
        m_Combo[i].append("UNSET");
    }
    m_Label[Entries::THUNDER_STORM    ].set_label("thunder-storm");
    m_Label[Entries::WIND             ].set_label("wind");
    m_Label[Entries::RAIN             ].set_label("rain");
    m_Label[Entries::ENVIRONMENT_SOUND].set_label("environment-sound");
    m_Label[Entries::AUX_01           ].set_label("aux 01");
    m_Label[Entries::AUX_02           ].set_label("aux 02");
    m_Label[Entries::AUX_03           ].set_label("aux 03");
}

CtrlEnvironment::~CtrlEnvironment() {
}

void CtrlEnvironment::get_value(rapidjson::Document &d) const {
    d.SetObject();
    d.AddMember("thunderStorm",     getSwitchState(Entries::THUNDER_STORM, d), d.GetAllocator());
    d.AddMember("wind",             getSwitchState(Entries::WIND, d), d.GetAllocator());
    d.AddMember("rain",             getSwitchState(Entries::RAIN, d), d.GetAllocator());
    d.AddMember("environmentSound", getSwitchState(Entries::ENVIRONMENT_SOUND, d), d.GetAllocator());
    d.AddMember("aux01",            getSwitchState(Entries::AUX_01, d), d.GetAllocator());
    d.AddMember("aux02",            getSwitchState(Entries::AUX_02, d), d.GetAllocator());
    d.AddMember("aux03",            getSwitchState(Entries::AUX_03, d), d.GetAllocator());
}

void CtrlEnvironment::init(Gtk::ScrolledWindow &container) {
    for(int i = 0; i < Entries::LAST_ENTRY; ++i) {
        m_Combo[i].set_active(1);
    }
    container.add(m_VBox);
    container.show_all_children();
}

rapidjson::Value CtrlEnvironment::getSwitchState(CtrlEnvironment::Entries entry, rapidjson::Document &d) const {
    std::string str = m_Combo[entry].get_active_text();
    return rapidjson::Value{str.c_str(), static_cast<unsigned int>(str.length()), d.GetAllocator()};
}
