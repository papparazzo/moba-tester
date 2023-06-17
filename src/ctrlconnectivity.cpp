#include "ctrlconnectivity.h"

#include <string>

CtrlConnectivity::CtrlConnectivity() {
    m_VBox.append(m_HBox);
    m_HBox.append(m_Label);
    m_HBox.append(m_Combo);

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
    container.set_child(m_VBox);
   // container.show_all_children();
}
