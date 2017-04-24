#include "ctrltracklayout.h"

#include <string>

CtrlTrackLayout::CtrlTrackLayout() :
    m_HBox_Id{Gtk::ORIENTATION_HORIZONTAL, 6},
    m_HBox_Description{Gtk::ORIENTATION_HORIZONTAL, 6},
    m_HBox_Caption{Gtk::ORIENTATION_HORIZONTAL, 6},
    m_VBox{Gtk::ORIENTATION_VERTICAL, 6}
{
    m_VBox.pack_start(m_HBox_Id, Gtk::PACK_SHRINK);
    m_VBox.pack_start(m_HBox_Caption, Gtk::PACK_SHRINK);
    m_VBox.pack_start(m_HBox_Description, Gtk::PACK_SHRINK);

    m_HBox_Id.pack_start(m_Label_Id, Gtk::PACK_SHRINK);
    m_HBox_Id.pack_end(m_Entry_Id, Gtk::PACK_SHRINK);

    m_HBox_Caption.pack_start(m_Label_Caption, Gtk::PACK_SHRINK);
    m_HBox_Caption.pack_end(m_Entry_Name, Gtk::PACK_SHRINK);

    m_HBox_Description.pack_start(m_Label_Description, Gtk::PACK_SHRINK);
    m_HBox_Description.pack_end(m_Entry_Description, Gtk::PACK_SHRINK);

    m_Label_Id.set_label("Id");
    m_Label_Caption.set_label("Name");
    m_Label_Description.set_label("Description");
}

CtrlTrackLayout::~CtrlTrackLayout() {
}

moba::JsonItemPtr CtrlTrackLayout::get_value() const {
    moba::JsonObjectPtr obj(new moba::JsonObject());
    if(m_Entry_Id.get_visible()) {
        (*obj)["id"         ] = moba::toJsonNumberPtr(std::stol(m_Entry_Id.get_text()));
    }
    (*obj)["name"       ] = moba::toJsonStringPtr(m_Entry_Name.get_text());
    (*obj)["description"] = moba::toJsonStringPtr(m_Entry_Description.get_text());
    return obj;
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