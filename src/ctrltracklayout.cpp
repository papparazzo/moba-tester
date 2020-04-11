#include "ctrltracklayout.h"

#include <string>

CtrlTrackLayout::CtrlTrackLayout() {
    m_VBox.pack_start(m_HBox_Id, Gtk::PACK_SHRINK);
    m_VBox.pack_start(m_HBox_Caption, Gtk::PACK_SHRINK);
    m_VBox.pack_start(m_HBox_Description, Gtk::PACK_SHRINK);
    m_VBox.pack_start(m_HBox_Active, Gtk::PACK_SHRINK);

    m_HBox_Id.pack_start(m_Label_Id, Gtk::PACK_SHRINK);
    m_HBox_Id.pack_end(m_Entry_Id, Gtk::PACK_SHRINK);

    m_HBox_Caption.pack_start(m_Label_Caption, Gtk::PACK_SHRINK);
    m_HBox_Caption.pack_end(m_Entry_Name, Gtk::PACK_SHRINK);

    m_HBox_Description.pack_start(m_Label_Description, Gtk::PACK_SHRINK);
    m_HBox_Description.pack_end(m_Entry_Description, Gtk::PACK_SHRINK);

    m_HBox_Active.pack_start(m_Label_Active, Gtk::PACK_SHRINK);
    m_HBox_Active.pack_end(m_Check_Bool, Gtk::PACK_SHRINK);

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
    container.add(m_VBox);
    container.show_all_children();
    m_Entry_Id.set_visible(enableIdSetting);
    m_Label_Id.set_visible(enableIdSetting);
}

rapidjson::Value CtrlTrackLayout::setText(const std::string &value, rapidjson::Document &d) const {
    return rapidjson::Value(value.c_str(), value.length(), d.GetAllocator());
}
