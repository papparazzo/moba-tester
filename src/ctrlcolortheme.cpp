#include "ctrlcolortheme.h"

#include <string>

CtrlColorTheme::CtrlColorTheme() {
    m_VBox.pack_start(m_HBox_DimTime, Gtk::PACK_SHRINK);
    m_VBox.pack_start(m_HBox_BrightTime, Gtk::PACK_SHRINK);
    m_VBox.pack_start(m_HBox_Condition, Gtk::PACK_SHRINK);

    m_HBox_DimTime.pack_start(m_Label_DimTime, Gtk::PACK_SHRINK);
    m_HBox_DimTime.pack_end(m_Entry_DimTime, Gtk::PACK_SHRINK);

    m_HBox_BrightTime.pack_start(m_Label_BrightTime, Gtk::PACK_SHRINK);
    m_HBox_BrightTime.pack_end(m_Entry_BrightTime, Gtk::PACK_SHRINK);

    m_HBox_Condition.pack_start(m_Label_Condition, Gtk::PACK_SHRINK);
    m_HBox_Condition.pack_end(m_Combo_Condition, Gtk::PACK_SHRINK);

    m_Label_DimTime.set_label("Dim-Time (hh:mm)");
    m_Label_BrightTime.set_label("Bright-Time (hh:mm)");
    m_Label_Condition.set_label("Condition");

    m_Combo_Condition.append("ON");
    m_Combo_Condition.append("OFF");
    m_Combo_Condition.append("AUTO");
    m_Combo_Condition.append("UNSET");
}

CtrlColorTheme::~CtrlColorTheme() {
}

void CtrlColorTheme::get_value(rapidjson::Document &d) const {
    d.SetObject();
    d.AddMember("dimTime", setText(m_Entry_DimTime.get_text(), d), d.GetAllocator());
    d.AddMember("brightTime", setText(m_Entry_BrightTime.get_text(), d), d.GetAllocator());
    d.AddMember("condition", setText(m_Combo_Condition.get_active_text(), d), d.GetAllocator());
}

void CtrlColorTheme::init(Gtk::ScrolledWindow &container) {
    m_Entry_DimTime.set_text("");
    m_Entry_BrightTime.set_text("");
    m_Combo_Condition.set_active(1);
    container.add(m_VBox);
    container.show_all_children();
}

rapidjson::Value CtrlColorTheme::setText(const std::string &value, rapidjson::Document &d) const {
    return rapidjson::Value(value.c_str(), value.length(), d.GetAllocator());
}
