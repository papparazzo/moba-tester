#include "ctrlambientlight.h"

#include <string>

CtrlAmbientLight::CtrlAmbientLight() :
    m_VBox{Gtk::ORIENTATION_VERTICAL, 6}
{
    for(int i = 0; i < LAST_ENTRY; ++i) {
        m_HBox[i].set_orientation(Gtk::ORIENTATION_HORIZONTAL);
        m_HBox[i].pack_start(m_Label[i], Gtk::PACK_SHRINK);
        m_HBox[i].pack_end(m_Entry[i], Gtk::PACK_SHRINK);
        m_VBox.pack_start(m_HBox[i], Gtk::PACK_SHRINK);
    }
    m_Label[RED  ].set_label("red");
    m_Label[BLUE ].set_label("blue");
    m_Label[GREEN].set_label("green");
    m_Label[WHITE].set_label("white");
}

CtrlAmbientLight::~CtrlAmbientLight() {
}

moba::JsonItemPtr CtrlAmbientLight::get_value() const {
    moba::JsonObjectPtr obj(new moba::JsonObject());
    (*obj)["red"  ] = moba::toJsonNumberPtr(std::stol(m_Entry[RED  ].get_text()));
    (*obj)["blue" ] = moba::toJsonNumberPtr(std::stol(m_Entry[BLUE ].get_text()));
    (*obj)["green"] = moba::toJsonNumberPtr(std::stol(m_Entry[GREEN].get_text()));
    (*obj)["white"] = moba::toJsonNumberPtr(std::stol(m_Entry[WHITE].get_text()));
    return obj;
}

void CtrlAmbientLight::init(Gtk::ScrolledWindow &container) {
    for(int i = 0; i < LAST_ENTRY; ++i) {
        m_Entry[i].set_text("");
    }
    container.add(m_VBox);
    container.show_all_children();
}

