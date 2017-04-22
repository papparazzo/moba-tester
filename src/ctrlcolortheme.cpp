#include "ctrlcolortheme.h"

#include <string>

CtrlColorTheme::CtrlColorTheme() :
    m_HBox_DimTime{Gtk::ORIENTATION_HORIZONTAL, 6},
    m_HBox_BrightTime{Gtk::ORIENTATION_HORIZONTAL, 6},
    m_HBox_Condition{Gtk::ORIENTATION_HORIZONTAL, 6},
    m_VBox{Gtk::ORIENTATION_VERTICAL, 6}
{
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

    m_Combo_Condition.append("on");
    m_Combo_Condition.append("off");
    m_Combo_Condition.append("auto");
    m_Combo_Condition.append("unset");
}

CtrlColorTheme::~CtrlColorTheme() {
}

moba::JsonItemPtr CtrlColorTheme::get_value() const {
    moba::JsonObjectPtr obj(new moba::JsonObject());
    (*obj)["dimTime"   ] = moba::toJsonStringPtr(m_Entry_DimTime.get_text());
    (*obj)["brightTime"] = moba::toJsonStringPtr(m_Entry_BrightTime.get_text());
    (*obj)["condition" ] = getThreeState();
    return obj;
}

void CtrlColorTheme::init(Gtk::ScrolledWindow &container) {
    m_Entry_DimTime.set_text("");
    m_Entry_BrightTime.set_text("");
    m_Combo_Condition.set_active(1);
    container.add(m_VBox);
    container.show_all_children();
}

moba::JsonThreeStatePtr CtrlColorTheme::getThreeState() const {
    std::string str = m_Combo_Condition.get_active_text();
    if(str == "on") {
        return moba::toJsonThreeStatePtr(moba::JsonThreeState::ON);
    }
    if(str == "off") {
        return moba::toJsonThreeStatePtr(moba::JsonThreeState::OFF);
    }
    if(str == "auto") {
        return moba::toJsonThreeStatePtr(moba::JsonThreeState::AUTO);
    }
    return moba::toJsonThreeStatePtr(moba::JsonThreeState::UNSET);
}
