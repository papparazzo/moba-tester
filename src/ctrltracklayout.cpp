#include "ctrltracklayout.h"

#include <string>

CtrlTrackLayout::CtrlTrackLayout() {
    m_VBox.append(m_HBox_Id);
    m_VBox.append(m_HBox_Caption);
    m_VBox.append(m_HBox_Description);
    m_VBox.append(m_HBox_Active);

    m_HBox_Id.append(m_Label_Id);
    m_HBox_Id.append(m_Entry_Id);

    m_HBox_Caption.append(m_Label_Caption);
    m_HBox_Caption.append(m_Entry_Name);

    m_HBox_Description.append(m_Label_Description);
    m_HBox_Description.append(m_Entry_Description);

    m_HBox_Active.append(m_Label_Active);
    m_HBox_Active.append(m_Check_Bool);

    m_Label_Id.set_label("Id");
    m_Label_Caption.set_label("Name");
    m_Label_Description.set_label("Description");
    m_Label_Active.set_label("Active");
}

CtrlTrackLayout::~CtrlTrackLayout() {
}

void CtrlTrackLayout::get_value(rapidjson::Document &d) const {
    d.SetObject();
    if(m_Entry_Id.get_visible()) {
        d.AddMember("id", std::stol(m_Entry_Id.get_text()), d.GetAllocator());
    }
    d.AddMember("name", setText(m_Entry_Name.get_text(), d), d.GetAllocator());
    d.AddMember("description", setText(m_Entry_Description.get_text(), d), d.GetAllocator());
    d.AddMember("active", m_Check_Bool.get_active(), d.GetAllocator());
}

void CtrlTrackLayout::init(bool enableIdSetting, Gtk::ScrolledWindow &container) {
    m_Entry_Id.set_text("");
    m_Entry_Name.set_text("");
    m_Entry_Description.set_text("");
    container.set_child(m_VBox);
   // container.show_all_children();
    m_Entry_Id.set_visible(enableIdSetting);
    m_Label_Id.set_visible(enableIdSetting);
}

rapidjson::Value CtrlTrackLayout::setText(const std::string &value, rapidjson::Document &d) const {
    return rapidjson::Value(value.c_str(), value.length(), d.GetAllocator());
}
