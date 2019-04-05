#include "mainwindow.h"

MainWindow::MainWindow()
{
    setFixedSize(1000, 500);
    setWindowTitle("Éditeur de menu contextuel");

    QLabel* name_label = new QLabel(this);
    name_label->setText("nom");
    name_label->setToolTip("Nom de l'entrée de menu contextuel");
    name_label->setGeometry(50, 6, 200, 15);

    QLabel* first_label = new QLabel(this);
    first_label->setText("par défaut");
    first_label->setToolTip("Action à effectuer lors d'un conflit");
    first_label->setGeometry(490, 6, 100, 15);

    QLabel* second_label = new QLabel(this);
    second_label->setText("dossier-dossier");
    second_label->setToolTip("Action à effectuer lors d'un conflit entre dossiers (possibilité de fusion)");
    second_label->setGeometry(590, 6, 100, 15);

    QLabel* third_label = new QLabel(this);
    third_label->setText("avec la source");
    third_label->setToolTip("Action à effectuer lors d'un conflit avec le dossier en cours de vidage (pas d'écrasement possible)");
    third_label->setGeometry(690, 6, 100, 15);

    QLabel* recursive_label = new QLabel(this);
    recursive_label->setText("arborescence");
    recursive_label->setToolTip("Option déterminant si toute l'arborescence du dossier est vidée (plus aucun dossier)");
    recursive_label->setGeometry(808, 6, 100, 15);

    QLabel* conservative_label = new QLabel(this);
    conservative_label->setText("conserver");
    conservative_label->setToolTip("Option déterminant si l'on doit conserver le dossier vidé et tout son contenu (usage de la copie)");
    conservative_label->setGeometry(915, 6, 100, 15);

    s_area = new QScrollArea(this);
    s_area->setGeometry(0, 24, 1000, 430);

    s_removed_entries = QVector<RegEntry*>();

    s_entries = QVector<GuiEntry*>();
    s_big_widget = new QWidget();
    s_layout = new QVBoxLayout();
    s_layout->setMargin(0);

    QVector<RegEntry*> entries(get_save());
    for (int i(0); i < entries.size(); ++i)
    {
        GuiEntry* entry = new GuiEntry(*entries[i]);
        s_entries.push_back(entry);
        s_layout->addWidget(entry);
    }

    s_big_widget->setLayout(s_layout);
    s_big_widget->setGeometry(0, 0, 960, 50*s_entries.size());
    s_area->setWidget(s_big_widget);

    QWidget* button_widget = new QWidget(this);
    button_widget->setGeometry(0, 452, 1000, 50);
    QHBoxLayout* button_layout = new QHBoxLayout();
    button_widget->setLayout(button_layout);

    s_add_button = new QPushButton("Ajouter une entrée de menu");
    s_add_button->setToolTip("Les changements ne sont effectifs qu'après une sauvegarde");
    connect(s_add_button, SIGNAL(clicked()), this, SLOT(add_entry()));
    button_layout->addWidget(s_add_button);

    s_delete_button = new QPushButton("Supprimer la sélection");
    s_delete_button->setToolTip("Les changements ne sont effectifs qu'après une sauvegarde");
    connect(s_delete_button, SIGNAL(clicked()), this, SLOT(delete_selection()));
    button_layout->addWidget(s_delete_button);

    s_save_button = new QPushButton("Sauvegarder");
    s_save_button->setToolTip("Les doublons (entrées présentant les mêmes options) seront supprimés, mêmes s'ils ont un nom différent");
    connect(s_save_button, SIGNAL(clicked()), this, SLOT(save()));
    button_layout->addWidget(s_save_button);

    s_init_button = new QPushButton("Réinitialiser");
    s_init_button->setToolTip("Sauvegarder les entrées par défaut");
    connect(s_init_button, SIGNAL(clicked()), this, SLOT(init()));
    button_layout->addWidget(s_init_button);

    s_quit_button = new QPushButton("Quitter");
    s_quit_button->setToolTip("Pas de sauvegarde automatique en quittant");
    connect(s_quit_button, SIGNAL(clicked()), this, SLOT(quit()));
    button_layout->addWidget(s_quit_button);
}

void MainWindow::add_entry()
{
    GuiEntry* new_entry = new GuiEntry();
    s_entries.push_back(new_entry);
    s_layout->addWidget(new_entry);
    s_big_widget->setGeometry(0, 0, 960, 50*s_entries.size());
}

void MainWindow::delete_selection()
{
    QVector<GuiEntry*> new_entries;
    for (int i(0); i < s_entries.size(); ++i)
    {
        GuiEntry* current_entry(s_entries[i]);
        if (current_entry->is_selected())
        {
            s_layout->removeWidget(current_entry);
            s_removed_entries.push_back(current_entry->s_entry);
            delete current_entry;
        }
        else {new_entries.push_back(current_entry);}
    }
    s_entries = new_entries;
    s_big_widget->setGeometry(0, 0, 960, 50*s_entries.size());
}

void MainWindow::delete_entry(int index)
{
    QVector<GuiEntry*> new_entries;
    for (int i(0); i < s_entries.size(); ++i)
    {
        GuiEntry* current_entry(s_entries[i]);
        if (i == index)
        {
            s_layout->removeWidget(current_entry);
            s_removed_entries.push_back(current_entry->s_entry);
            delete current_entry;
        }
        else {new_entries.push_back(current_entry);}
    }
    s_entries = new_entries;
    s_big_widget->setGeometry(0, 0, 960, 50*s_entries.size());
}

void MainWindow::save()
{
    int i(0);
    while (i < s_entries.size())
    {
        if (! s_entries[i]->save())
        {delete_entry(i);}
        else {++i;}
    }
    for (int i(0); i < s_removed_entries.size(); ++i)
    {
        QString saved(s_removed_entries[i]->saved_entry);
        if (saved != "")
        {RegEntry(saved).remove();}
    }
    s_removed_entries = QVector<RegEntry*>();
}

void MainWindow::init()
{
    while (0 < s_entries.size())
    {
        GuiEntry* current_entry(s_entries[0]);
        s_layout->removeWidget(current_entry);
        s_removed_entries.push_back(current_entry->s_entry);
        delete current_entry;
    }
    QVector<GuiEntry*> new_entries;
    new_entries.push_back(new GuiEntry("Vider le dossier iciiii00"));
    new_entries.push_back(new GuiEntry("Vider l'arborescence du dossier iciiii10"));
    s_entries = new_entries;
    for (int i(0); i < 2; ++i)
    {
        s_entries[i]->s_entry->saved_entry = "";
        s_layout->addWidget(s_entries[i]);
    }
    s_big_widget->setGeometry(0, 0, 960, 50*s_entries.size());
    save();
}

void MainWindow::quit()
{
    exit(0);
}
