#include "ctrlglobaltimer.h"

#include <string>

CtrlGlobalTimer::CtrlGlobalTimer() {
    m_VBox.append(m_HBox_CurModelTime);
    m_VBox.append(m_HBox_Multiplicator);

    m_HBox_CurModelTime.append(m_Label_CurModelTime);
    m_HBox_CurModelTime.append(m_Entry_CurModelTime);

    m_HBox_Multiplicator.append(m_Label_Multiplicator);
    m_HBox_Multiplicator.append(m_Entry_Multiplicator);

            //Gtk::Entry m_Entry_CurModelDay;


    m_Label_CurModelDay.set_label("Tag");
    m_Label_CurModelTime.set_label("hh:mm (z.B. 10:00)");
    m_Label_Multiplicator.set_label("Multiplikator");
}

CtrlGlobalTimer::~CtrlGlobalTimer() {
}

void CtrlGlobalTimer::get_value(rapidjson::Document &d) const {
    std::string txt = m_Entry_CurModelTime.get_text();
    d.SetObject();
    d.AddMember("curModelDay", rapidjson::Value("SATURDAY", d.GetAllocator()), d.GetAllocator());
    d.AddMember("curModelTime", rapidjson::Value(txt.c_str(), txt.length(), d.GetAllocator()), d.GetAllocator());
    d.AddMember("multiplicator", rapidjson::Value(std::stol(m_Entry_Multiplicator.get_text())), d.GetAllocator());
}

void CtrlGlobalTimer::init(Gtk::ScrolledWindow &container) {
    m_Entry_CurModelTime.set_text("");
    m_Entry_Multiplicator.set_text("");

    container.set_child(m_VBox);
 //   container.show_all_children();
}