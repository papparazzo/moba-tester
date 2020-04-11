#include "ctrlglobaltimer.h"

#include <string>

CtrlGlobalTimer::CtrlGlobalTimer() {
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

void CtrlGlobalTimer::get_value(rapidjson::Document &d) const {
    std::string txt = m_Entry_CurModelTime.get_text();
    d.SetObject();
    d.AddMember("curModelTime", rapidjson::Value(txt.c_str(), txt.length(), d.GetAllocator()), d.GetAllocator());
    d.AddMember("multiplicator", rapidjson::Value(std::stol(m_Entry_Multiplicator.get_text())), d.GetAllocator());
}

void CtrlGlobalTimer::init(Gtk::ScrolledWindow &container) {
    m_Entry_CurModelTime.set_text("");
    m_Entry_Multiplicator.set_text("");

    container.add(m_VBox);
    container.show_all_children();
}