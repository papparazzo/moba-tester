#include "ctrlconnectivity.h"

#include <string>

CtrlConnectivity::CtrlConnectivity() {
    m_VBox.pack_start(m_HBox, Gtk::PACK_SHRINK);
    m_HBox.pack_start(m_Label, Gtk::PACK_SHRINK);
    m_HBox.pack_end(m_Combo, Gtk::PACK_SHRINK);

    m_Label.set_label("Connectivity");

    m_Combo.append("CONNECTED");
    m_Combo.append("ERROR");
}

CtrlConnectivity::~CtrlConnectivity() {
}

void CtrlConnectivity::get_value(rapidjson::Document &d) const {
    std::string str = m_Combo.get_active_text();
    d.SetString(str.c_str(), str.length(), d.GetAllocator());
}

void CtrlConnectivity::init(Gtk::ScrolledWindow &container) {
    m_Combo.set_active(1);
    container.add(m_VBox);
    container.show_all_children();
}
