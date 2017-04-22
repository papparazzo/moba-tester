#include "ctrlglobaltimer.h"

#include <string>

CtrlGlobalTimer::CtrlGlobalTimer() :
    m_HBox_CurModelTime{Gtk::ORIENTATION_HORIZONTAL, 6},
    m_HBox_Multiplicator{Gtk::ORIENTATION_HORIZONTAL, 6},
    m_VBox{Gtk::ORIENTATION_VERTICAL, 6}
{
    m_VBox.pack_start(m_HBox_CurModelTime, Gtk::PACK_SHRINK);
    m_VBox.pack_start(m_HBox_Multiplicator, Gtk::PACK_SHRINK);

    m_HBox_CurModelTime.pack_start(m_Label_CurModelTime, Gtk::PACK_SHRINK);
    m_HBox_CurModelTime.pack_end(m_Entry_CurModelTime, Gtk::PACK_SHRINK);

    m_HBox_Multiplicator.pack_start(m_Label_Multiplicator, Gtk::PACK_SHRINK);
    m_HBox_Multiplicator.pack_end(m_Entry_Multiplicator, Gtk::PACK_SHRINK);

    m_Label_CurModelTime.set_label("DD hh:mm (z.B. Sa 10:00)");
    m_Label_Multiplicator.set_label("Multiplikator");
}

CtrlGlobalTimer::~CtrlGlobalTimer() {
}

moba::JsonItemPtr CtrlGlobalTimer::get_value() const {
    moba::JsonObjectPtr obj(new moba::JsonObject());
    (*obj)["curModelTime" ] = moba::toJsonStringPtr(m_Entry_CurModelTime.get_text());
    (*obj)["multiplicator"] = moba::toJsonNumberPtr(std::stol(m_Entry_Multiplicator.get_text()));
    return obj;
}

void CtrlGlobalTimer::init(Gtk::ScrolledWindow &container) {
    m_Entry_CurModelTime.set_text("");
    m_Entry_Multiplicator.set_text("");

    container.add(m_VBox);
    container.show_all_children();
}