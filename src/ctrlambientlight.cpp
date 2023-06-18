#include "ctrlambientlight.h"

#include <string>

CtrlAmbientLight::CtrlAmbientLight() {
    for(int i = 0; i < LAST_ENTRY; ++i) {
        m_adjustment[i] = Gtk::Adjustment::create(100.0, 0.0, 4095.0, 1.0, 1.0, 0.0);
        m_SpinButton[i].set_adjustment(m_adjustment[i]);
        m_HBox[i].set_orientation(Gtk::Orientation::HORIZONTAL);
        m_HBox[i].append(m_Label[i]);
        m_HBox[i].append(m_SpinButton[i]);
        m_VBox.append(m_HBox[i]);
    }
    m_Label[RED  ].set_label("red");
    m_Label[BLUE ].set_label("blue");
    m_Label[GREEN].set_label("green");
    m_Label[WHITE].set_label("white");
}

CtrlAmbientLight::~CtrlAmbientLight() {
}

void CtrlAmbientLight::get_value(rapidjson::Document &d) const {
    d.SetObject();
    d.AddMember("red",   m_SpinButton[RED  ].get_value_as_int(), d.GetAllocator());
    d.AddMember("blue",  m_SpinButton[BLUE ].get_value_as_int(), d.GetAllocator());
    d.AddMember("green", m_SpinButton[GREEN].get_value_as_int(), d.GetAllocator());
    d.AddMember("white", m_SpinButton[WHITE].get_value_as_int(), d.GetAllocator());
}

void CtrlAmbientLight::init(Gtk::ScrolledWindow &container) {
    container.set_child(m_VBox);
    //container.show_all_children();
}

