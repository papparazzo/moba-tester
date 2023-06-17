#include "ctrlblocklock.h"

#include <string>
#include <sstream>

CtrlBlockLock::CtrlBlockLock() {
    m_VBox.append(m_HBox_Blocks);
    m_HBox_Blocks.append(m_Label_Blocks);
    m_HBox_Blocks.append(m_Entry_Blocks);

    m_Label_Blocks.set_label("BlockIds, Kommagetrennt");
}

CtrlBlockLock::~CtrlBlockLock() {
}

void CtrlBlockLock::get_value(rapidjson::Document &d) const {
    std::string str = m_Entry_Blocks.get_text();
    d.SetArray();

    std::replace(str.begin(), str.end(), ',', ' ');

    std::stringstream ss(str);

    int temp;
    while(ss >> temp) {
        d.PushBack(temp, d.GetAllocator());
    }
}

void CtrlBlockLock::init(Gtk::ScrolledWindow &container) {
    m_Entry_Blocks.set_text("");

    container.set_child(m_VBox);
    //container.show_all_children();
}