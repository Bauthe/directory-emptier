#include "guientry.h"

QString GuiEntry::enter_name_here = "Entrez un nom ici                                                                               ";

GuiEntry::GuiEntry(): GuiEntry("iii00")
{
    s_entry->saved_entry = "";
}

GuiEntry::GuiEntry(QString code): QWidget()
{
    s_entry = new RegEntry(code);

    s_layout = new QHBoxLayout();
    s_layout->setMargin(8);
    setLayout(s_layout);

    s_checkbox = new QCheckBox();
    s_layout->addWidget(s_checkbox);

    s_stacked = new QStackedWidget();
    s_button = new QPushButton(enter_name_here);
    s_button->setFlat(true);
    s_line_edit = new QLineEdit(s_entry->name);
    connect(s_line_edit, SIGNAL(editingFinished()), this, SLOT(edit_name_finished()));
    connect(s_button, SIGNAL(clicked()), this, SLOT(start_edit_name()));
    s_stacked->addWidget(s_line_edit);
    s_stacked->addWidget(s_button);
    s_stacked->setCurrentWidget(s_button);
    s_layout->addWidget(s_stacked);
    edit_name_finished();

    s_first_behaviour = new QComboBox();
    s_first_behaviour->addItem("Demander");
    s_first_behaviour->addItem("Écraser");
    s_first_behaviour->addItem("Ignorer");
    s_first_behaviour->addItem("Renommer");
    s_first_behaviour->setCurrentText(behaviour_to_string(s_entry->first_behaviour));
    connect(s_first_behaviour, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(update_first_behaviour(const QString&)));
    s_layout->addWidget(s_first_behaviour);

    s_second_behaviour = new QComboBox();
    s_second_behaviour->addItem("Demander");
    s_second_behaviour->addItem("Fusionner");
    s_second_behaviour->addItem("Écraser");
    s_second_behaviour->addItem("Ignorer");
    s_second_behaviour->addItem("Renommer");
    s_second_behaviour->setCurrentText(behaviour_to_string(s_entry->second_behaviour));
    connect(s_second_behaviour, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(update_second_behaviour(const QString&)));
    s_layout->addWidget(s_second_behaviour);

    s_third_behaviour = new QComboBox();
    s_third_behaviour->addItem("Demander");
    s_third_behaviour->addItem("Ignorer");
    s_third_behaviour->addItem("Renommer");
    s_third_behaviour->setCurrentText(behaviour_to_string(s_entry->third_behaviour));
    connect(s_third_behaviour, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(update_third_behaviour(const QString&)));
    s_layout->addWidget(s_third_behaviour);

    s_layout->addSpacing(50);

    s_recursive = new QCheckBox();
    set_check_box(*s_recursive, s_entry->recursive);
    connect(s_recursive, SIGNAL(stateChanged(int)), this, SLOT(update_recursive(int)));
    s_layout->addWidget(s_recursive);

    s_layout->addSpacing(74);

    s_conservative = new QCheckBox();
    set_check_box(*s_conservative, s_entry->conservative);
    connect(s_conservative, SIGNAL(stateChanged(int)), this, SLOT(update_conservative(int)));
    s_layout->addWidget(s_conservative);
}

GuiEntry::GuiEntry(RegEntry& entry): GuiEntry(entry.save_code()) {}

bool GuiEntry::is_selected()
{
    return (s_checkbox->checkState() == Qt::CheckState::Checked);
}

bool GuiEntry::save()
{
    if (s_entry->to_save())
    {return s_entry->add();}
    return true;
}

void GuiEntry::start_edit_name()
{
    s_stacked->setCurrentWidget(s_line_edit);
}

void GuiEntry::edit_name_finished()
{
    QFontMetrics metric(QFont("defaut"));
    QString lots_of_spaces;
    QString text(s_line_edit->text());
    for (int i(0); metric.width(text + lots_of_spaces) < metric.width(enter_name_here); ++i)
    {lots_of_spaces += " ";}
    lots_of_spaces.chop(1);
    s_button->setText(s_line_edit->text() + lots_of_spaces);
    if (text == "")
    {s_button->setText(enter_name_here);}
    s_stacked->setCurrentWidget(s_button);

    s_entry->name = text;
}

void GuiEntry::update_first_behaviour(const QString& string)
{s_entry->first_behaviour = string_to_behaviour(string);}

void GuiEntry::update_second_behaviour(const QString& string)
{s_entry->second_behaviour = string_to_behaviour(string);}

void GuiEntry::update_third_behaviour(const QString& string)
{s_entry->third_behaviour = string_to_behaviour(string);}

void GuiEntry::update_recursive(int state)
{
    if (state == 2)
    {s_entry->recursive = true;}
    else {s_entry->recursive = false;}
}
void GuiEntry::update_conservative(int state)
{
    if (state == 2)
    {s_entry->conservative = true;}
    else {s_entry->conservative = false;}
}

void set_check_box(QCheckBox& box, bool value)
{
    if (value)
    {box.setCheckState(Qt::CheckState::Checked);}
    else {box.setCheckState(Qt::CheckState::Unchecked);}
}

QString behaviour_to_string(QChar behaviour)
{
    if (behaviour == 'i')
    {return "Demander";}
    else if (behaviour == 'm')
    {return "Fusionner";}
    else if (behaviour == 'c')
    {return "Écraser";}
    else if (behaviour == 'n')
    {return "Ignorer";}
    else if (behaviour == 'r')
    {return "Renommer";}
    return "";
}

QChar string_to_behaviour(QString string)
{
    if (string == "Demander")
    {return 'i';}
    else if (string == "Fusionner")
    {return 'm';}
    else if (string == "Écraser")
    {return 'c';}
    else if (string == "Ignorer")
    {return 'n';}
    else if (string == "Renommer")
    {return 'r';}
    return '\0';
}
