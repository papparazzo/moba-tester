#include "ctrlhardwarestate.h"

#include <string>

CtrlHardwarestate::CtrlHardwarestate() :
    m_VBox{Gtk::ORIENTATION_VERTICAL, 6},
    m_HBox{Gtk::ORIENTATION_HORIZONTAL, 6}
{
    m_VBox.pack_start(m_HBox, Gtk::PACK_SHRINK);
    m_HBox.pack_start(m_Label, Gtk::PACK_SHRINK);
    m_HBox.pack_end(m_Combo, Gtk::PACK_SHRINK);

    m_Label.set_label("Hardwarestate");

    m_Combo.append("ERROR");
    m_Combo.append("STANDBY");
    m_Combo.append("POWER_OFF");
    m_Combo.append("READY");
}

CtrlHardwarestate::~CtrlHardwarestate() {
}

moba::JsonStringPtr CtrlHardwarestate::get_value() const {
    return moba::toJsonStringPtr(m_Combo.get_active_text());
}

void CtrlHardwarestate::init(Gtk::ScrolledWindow &container) {
    m_Combo.set_active(1);
    container.add(m_VBox);
    container.show_all_children();
}
