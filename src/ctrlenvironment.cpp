#include "ctrlenvironment.h"

#include <string>

CtrlEnvironment::CtrlEnvironment() :
    m_VBox{Gtk::ORIENTATION_VERTICAL, 6}
{
    for(int i = 0; i < LAST_ENTRY; ++i) {
        m_HBox[i].set_orientation(Gtk::ORIENTATION_HORIZONTAL);
        m_HBox[i].pack_start(m_Label[i], Gtk::PACK_SHRINK);
        m_HBox[i].pack_end(m_Combo[i], Gtk::PACK_SHRINK);
        m_VBox.pack_start(m_HBox[i], Gtk::PACK_SHRINK);
        m_Combo[i].append("on");
        m_Combo[i].append("off");
        m_Combo[i].append("trigger");
        m_Combo[i].append("auto");
        m_Combo[i].append("unset");
    }
    m_Label[THUNDER_STORM    ].set_label("thunder-storm");
    m_Label[WIND             ].set_label("wind");
    m_Label[RAIN             ].set_label("rain");
    m_Label[ENVIRONMENT_SOUND].set_label("environment-sound");
    m_Label[AUX_01           ].set_label("aux 01");
    m_Label[AUX_02           ].set_label("aux 02");
    m_Label[AUX_03           ].set_label("aux 03");
}

CtrlEnvironment::~CtrlEnvironment() {
}

moba::JsonItemPtr CtrlEnvironment::get_value() const {
    moba::JsonObjectPtr obj(new moba::JsonObject());
    (*obj)["thunderStorm"      ] = getSwitchState(THUNDER_STORM);
    (*obj)["wind"              ] = getSwitchState(WIND);
    (*obj)["rain"              ] = getSwitchState(RAIN);
    (*obj)["environmentSound"  ] = getSwitchState(ENVIRONMENT_SOUND);
    (*obj)["aux01"             ] = getSwitchState(AUX_01);
    (*obj)["aux02"             ] = getSwitchState(AUX_02);
    (*obj)["aux03"             ] = getSwitchState(AUX_03);
    return obj;
}

void CtrlEnvironment::init(Gtk::ScrolledWindow &container) {
    for(int i = 0; i < LAST_ENTRY; ++i) {
        m_Combo[i].set_active(1);
    }
    container.add(m_VBox);
    container.show_all_children();
}

moba::JsonSwitchPtr CtrlEnvironment::getSwitchState(CtrlEnvironment::Entries entry) const {
    std::string str = m_Combo[entry].get_active_text();
    if(str == "on") {
        return moba::toJsonSwitchPtr(moba::JsonSwitch::ON);
    }
    if(str == "auto") {
        return moba::toJsonSwitchPtr(moba::JsonSwitch::AUTO);
    }
    if(str == "trigger") {
        return moba::toJsonSwitchPtr(moba::JsonSwitch::TRIGGER);
    }
    if(str == "off") {
        return moba::toJsonSwitchPtr(moba::JsonSwitch::OFF);
    }
    return moba::toJsonSwitchPtr(moba::JsonSwitch::UNSET);
}
