#include "waitwindow.h"

using namespace std;

//this function initializes all pertinent variables and sets up interface
WaitWindow::WaitWindow(int argc, char *argv[]) : QWidget()
{
    //setting up useful variables
    s_origin = QStringList();

    s_recursive = false;
    s_conservative = false;
    s_behaviour = "iii"; //first letter is for default behaviour, second for directory-directory behaviour, and last for conflict with origin behaviour

    //handling args given to the program
    //mainly the args that give the path of the directories to empty
    //but also the options explained in usage()
    if (argc > 1)
    {
        QString current_arg;
        for (int i(1); i < argc; ++i)
        {
            current_arg = argv[i];
            if (current_arg == "-r" || current_arg == "--recursive")
            {s_recursive = true;}
            else if (current_arg == "-c" || current_arg == "--conservative")
            {s_conservative = true;}
            else if (current_arg == "-b" || current_arg == "--behaviour")
            {
                if (i != argc - 1)
                {
                    QString arg(argv[i+1]);
                    if (possible_behaviour(arg))
                    {s_behaviour = arg;}
                }
                else {exit(1);}
            }
            else if (current_arg == "-h" || current_arg == "--help")
            {
                usage();
                return;
            }
            else
            {
                if (item_type(current_arg) == "directory")
                {s_origin.push_back(QDir(current_arg).path());}
            }
        }
    }
    else {exit(1);}

    //setting up graphical stuff
    setWindowTitle(tr("Vidage en cours..."));
    setFixedSize(500, 100);
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), qApp->desktop()->availableGeometry()));

    s_user_response = '\0';
    s_ticked_box = false;

    s_layout = new QGridLayout(this);

    s_status_label = new QLabel;
    s_status_label->setFont(QFont("default",8));
    s_layout->addWidget(s_status_label, 0, 0, 1, 5);

    s_abandon_button = new QPushButton(tr("Abandonner"));
    s_layout->addWidget(s_abandon_button, 2, 4, 1, 1);
    connect(s_abandon_button, SIGNAL(clicked()), this, SLOT(quit()));

    s_timer = new QTimer(this);
    s_timer->setSingleShot(true);
    connect(s_timer, SIGNAL(timeout()), this, SLOT(body()));
    s_timer->start(1);
}

void WaitWindow::debug_exit(QString message)
{
    s_status_label->setText(message);
    while (true)
    {QApplication::processEvents();}
}

//this overwrite of slot closeEvent method makes the app be left when user clicks on the X button in title bar
void WaitWindow::closeEvent(QCloseEvent *event)
{quit();}

//this function adds buttons to ask user about conflicts
QString WaitWindow::ask_user(int conflict_number)
{
    //conflict_number is 0 if default, 1 if directory-directory, 2 if with origin directory

    //loading appropriate message
    QString current_label(s_status_label->text());
    s_status_label->setText(tr("L'élément \"", "Contexte : L'élément \"[nom_de_l'élément]\" existe déjà dans la destination.") + s_current_item + tr("\" existe déjà dans la destination.", "Contexte : L'élément \"[nom_de_l'élément]\" existe déjà dans la destination."));
    if (conflict_number == 2)
    {s_status_label->setText(tr("L'élément \"", "Contexte : L'élément \"[nom_de_l'élément]\" porte le même nom que le dossier vidé.") + s_current_item + tr("\" porte le même non que le dossier vidé.", "Contexte : L'élément \"[nom_de_l'élément]\" porte le même nom que le dossier vidé."));}

    //Creation of the buttons
    QVector<QPushButton*> buttons(0);
    if (conflict_number == 1)
    {add_merge_button(buttons);}
    if (conflict_number != 2)
    {add_crush_button(buttons);}
    add_ignore_button(buttons);
    add_rename_button(buttons);
    for (int i(0); i < buttons.size(); ++i)
    {s_layout->addWidget(buttons[i], 2, i, 1, 1);}

    QCheckBox* checkbox = new QCheckBox(tr("Appliquer à tous les autres éléments"));
    connect(checkbox, SIGNAL(stateChanged(int)), this, SLOT(box_changed(int)));
    s_layout->addWidget(checkbox, 1, 0, 1, 5);

    //acquiring answer
    while (s_user_response == '\0')
    {
        QApplication::processEvents();
        QThread::msleep(1);
    }
    QString result(s_user_response);
    if (s_ticked_box)
    {result += "1";}
    else {result += "0";}

    s_user_response = '\0';
    s_ticked_box = false;

    //resetting interface
    for (int i(0); i < buttons.size(); ++i)
    {delete buttons[i];}
    delete checkbox;
    s_status_label->setText(current_label);

    return result;
}

//this function manages the treatment of one item
//it also updates the list of contents in destination
void WaitWindow::pre_treatment(QString item_path, QString origin_directory, QString destination, QStringList& destination_items)
{
    //getting the name of the item to process
    QString item_name(cut_all_dirs(item_path));

    //creating the new path of the item
    QString new_path(destination + "/" + item_name);

    //update the interface
    s_current_item = item_name;
    s_status_label->setText(tr("Traitement de \"", "Contexte : Traitement de \"[nom_de_l'élément]\"...") + s_current_item + tr("\"...", "Contexte : Traitement de \"[nom_de_l'élément]\"..."));
    QApplication::processEvents();

    //detecting conflict
    bool conflict(false);
    for (int i(0); i < destination_items.size(); ++i)
    {
        if (destination_items[i].toLower() == item_name.toLower())
        {conflict = true;}
    }

    //handling conflict events
    if (conflict)
    {
        //getting conflict number
        int conflict_number(0);
        if (item_type(new_path) == "directory" && item_type(item_path) == "directory")
        {conflict_number = 1;}
        if (new_path.toLower() == origin_directory.toLower())
        {conflict_number = 2;}

        //getting related behaviour
        QString temp_behaviour(s_behaviour[conflict_number]);

        //asking user if behaviour set to i
        if (temp_behaviour == "i")
        {
            //taking into account user's decision about conflict
            temp_behaviour = ask_user(conflict_number);
            QChar permanent = temp_behaviour[temp_behaviour.size() - 1];
            temp_behaviour.chop(1);
            if (permanent == '1') {s_behaviour = update_behaviour(conflict_number, s_behaviour, temp_behaviour);}
        }

        //process a merge if needed
        if (temp_behaviour == "m")
        {
            merge(item_path, new_path);
            return;
        }

        //changing name of the item if needed
        if (temp_behaviour == "r")
        {
            QString name(item_name);
            QString ext("");
            if (item_type(item_path) != "directory")
            {
                QPair<QString, QString> pair(name_and_ext(item_name));
                name = pair.first;
                ext = pair.second;
            }

            int file_number(0);
            while (conflict)
            {
                item_name = name + QString::number(file_number) + ext;
                conflict = false;
                for (int i(0); i < destination_items.size(); ++i)
                {
                    if (destination_items[i] == item_name)
                    {conflict = true;}
                }
                ++file_number;
            }
            new_path = destination + "/" + item_name;
        }

        //cancelling operation if needed
        if (temp_behaviour == "n")
        {return;}
    }

    //performing action on the item
    effective_treatment(item_path, new_path);

    //updating the list of items in destination
    if (conflict)
    {destination_items.push_back(item_name);}

    return;
}

//this function performs the required operation
void WaitWindow::effective_treatment(QString item_path, QString new_path)
{
    //making sure there is nothing with the same name in destination
    while (item_type(new_path) != "nothing")
    {
        if (item_type(new_path) == "directory")
        {
            QDir dir(new_path);
            dir.removeRecursively();
        }
        else if (item_type(new_path) == "file")
        {QFile::remove(new_path);}
        else {exit(2);}
    }

    //doing the treatment itself
    if (s_conservative)
    {
        //making copy if original directory is to be kept
        if (item_type(item_path) == "file")
        {
            QFile::copy(item_path, new_path);
        }
        else if (item_type(item_path) == "directory")
        {
            recursive_copy(item_path, new_path);
        }
    }
    else
    {
        //moving item otherwise

        if (item_type(item_path) == "file")
        {
            QFile file(item_path);
            file.rename(new_path);
        }
        else if (item_type(item_path) == "directory")
        {
            QDir directory;
            directory.rename(item_path, new_path);
        }
    }
}

void WaitWindow::merge(QString item_path, QString new_path)
{
    QStringList destination_items;
    add_ls(destination_items, new_path, false);

    QStringList origin_items;
    add_ls(origin_items, item_path, true);

    for (int i(0); i < origin_items.size(); ++i)
    {pre_treatment(origin_items[i], item_path, new_path, destination_items);}
}

void WaitWindow::recursive_copy(QString item_path, QString new_path)
{
    QDir parent(parent_dir(new_path));
    parent.mkdir(cut_all_dirs(new_path));
    merge(item_path, new_path);
}

void WaitWindow::add_merge_button(QVector<QPushButton*>& buttons)
{
    QPushButton* button = new QPushButton(tr("Fusionner"));
    connect(button, SIGNAL(clicked()), this, SLOT(say_merge()));
    buttons.push_back(button);
}

void WaitWindow::add_crush_button(QVector<QPushButton*>& buttons)
{
    QPushButton* button = new QPushButton(tr("Écraser"));
    connect(button, SIGNAL(clicked()), this, SLOT(say_crush()));
    buttons.push_back(button);
}

void WaitWindow::add_ignore_button(QVector<QPushButton*>& buttons)
{
    QPushButton* button = new QPushButton(tr("Ignorer"));
    connect(button, SIGNAL(clicked()), this, SLOT(say_ignore()));
    buttons.push_back(button);
}

void WaitWindow::add_rename_button(QVector<QPushButton*>& buttons)
{
    QPushButton* button = new QPushButton(tr("Renommer"));
    connect(button, SIGNAL(clicked()), this, SLOT(say_rename()));
    buttons.push_back(button);
}

//this function shows the help and exits after 1min
void WaitWindow::usage()
{
    QLabel label;
    label.setText("Usage: directory-emptier.exe {OPTIONS} [DIRECTORY]:\n");
    label.setText(label.text() + "\tMoves the contents of DIRECTORY into its parent directory and deletes it\n");
    label.setText(label.text() + "\nOPTIONS:\n");
    label.setText(label.text() + "\t-r --recursive : empty recursively\n");
    label.setText(label.text() + "\t-c --conservative : keep the original directory unchanged\n");
    label.setText(label.text() + "\t-b --behaviour BEHAVIOUR : specify behaviour\n");
    label.setText(label.text() + "\t-h --help : show this and quit after 1min\n");
    label.setText(label.text() + "\nBEHAVIOURS:\n");
    label.setText(label.text() + "Three char string defining behaviour in case of conflict :\n");
    label.setText(label.text() + "\t- first char is about default behaviour : i c n r\n");
    label.setText(label.text() + "\t- second char is about directory-directory conflicts : i m c n r\n");
    label.setText(label.text() + "\t- last char is about conflicts with origin directory : i n r\n");
    label.setText(label.text() + "\nletter code:\n");
    label.setText(label.text() + "\ti : ask user\n");
    label.setText(label.text() + "\tc : crush\n");
    label.setText(label.text() + "\tn : do nothing\n");
    label.setText(label.text() + "\tr : rename\n");
    label.setText(label.text() + "\tm : merge\n");
    label.show();

    QTimer timer;
    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), this, SLOT(quit()));
    timer.start(60000);

    while (true)
    {
        QApplication::processEvents();
        QThread::msleep(10);
    }
}


//this slot is processing the core action of this program
void WaitWindow::body()
{
    for (int index(0); index < s_origin.size(); ++index)
    {
        QString directory(s_origin[index]);
        if (item_type(directory) == "directory")
        {
            //defining the destination
            QString destination = parent_dir(directory);

            //initializing a list of all items in destination, in order to handle copy conflicts
            s_destination_items = QStringList();
            add_ls(s_destination_items, destination, false);

            //setting up a list of all items to visit
            QStringList items_to_visit;
            add_ls(items_to_visit, directory, true);

            if (! s_recursive)
            {
                //non-recursive copy process
                //merely copy any item to visit
                for (int i(0); i < items_to_visit.size(); ++i)
                {pre_treatment(items_to_visit[i], directory, destination, s_destination_items);}
            }
            else
            {
                //recursive copy process
                while (items_to_visit.size() != 0)
                {
                    QString item(items_to_visit[items_to_visit.size() - 1]);
                    items_to_visit.pop_back();

                    //if the item is a file, copy it
                    //if it is a directory, update the list of items to visit
                    if (item_type(item) == "file")
                    {pre_treatment(item, directory, destination, s_destination_items);}
                    else if (item_type(item) == "directory")
                    {add_ls(items_to_visit, item, true);}
                }
            }

            //removing original if required
            if (! s_conservative && item_type(directory) == "directory")
            {
                QDir dir(directory);
                dir.removeRecursively();
            }
        }
    }
    exit(0);
}

//this slot exits the program
void WaitWindow::quit()
{exit(0);}

//this slot handles the checkbox
void WaitWindow::box_changed(int state)
{
    if (state == 0)
    {s_ticked_box = false;}
    else {s_ticked_box = true;}
}

//these four slots handle user's answers
void WaitWindow::say_merge()
{s_user_response = 'm';}

void WaitWindow::say_crush()
{s_user_response = 'c';}

void WaitWindow::say_ignore()
{s_user_response = 'n';}

void WaitWindow::say_rename()
{s_user_response = 'r';}


//this function returns the name of the item designated by the path given in arg
QString cut_all_dirs(QString path)
{
    int last_slash(-1);
    int previous_slash(last_slash);
    for (int i(0); i < path.size(); ++i)
    {
        if (path[i] == '/')
        {
            previous_slash = last_slash;
            last_slash = i;
        }
    }
    int end(path.size());
    if (last_slash == path.size() - 1)
    {
        end = end - 1;
        last_slash = previous_slash;
    }
    if (last_slash == -1) {last_slash = 0;}

    QString result("");
    for (int i(last_slash + 1); i < end; ++i)
    {result.push_back(path[i]);}
    return result;
}

//this function gives the path of the parent directory of a path
QString parent_dir(QString path)
{
    int last_slash(-1);
    int previous_slash(last_slash);
    for (int i(0); i < path.size(); ++i)
    {
        if (path[i] == '/')
        {
            previous_slash = last_slash;
            last_slash = i;
        }
    }

    if (last_slash == path.size() - 1)
    {last_slash = previous_slash;}
    if (last_slash == -1) {last_slash = path.size() - 1;}

    QString result("");
    for (int i(0); i < last_slash; ++i)
    {result.push_back(path[i]);}
    return result;
}

//this function adds the elements of a directory to a list of paths
void add_ls(QStringList& list, QString directory, bool absolute)
{
    QDir dir(directory);
    if (dir.exists())
    {
        QStringList new_list(dir.entryList());
        for (int i(0); i < new_list.size(); ++i)
        {
            QString name(new_list[i]);
            if (name != "." && name != "..")
            {
                if (absolute)
                {list.push_back(dir.path() + "/" + name);}
                else {list.push_back(name);}
            }
        }
    }
    else {exit(2);}
}

//this function returns the type of data a given path points to
QString item_type(QString path)
{
    QFileInfo item(path);
    if (item.isDir())
    {return "directory";}
    else if (item.isFile())
    {return "file";}
    else if (! item.exists())
    {return "nothing";}
    return "other";
}

//this function separates name and extension from an item name
QPair<QString, QString> name_and_ext(QString item)
{
    int i(0);
    QString first("");
    while (item[i] != "." && i < item.size())
    {
        first.push_back(item[i]);
        ++i;
    }

    QString second("");
    if (i != item.size())
    {
        while (i < item.size())
        {
            second.push_back(item[i]);
            ++i;
        }
    }

    return QPair<QString, QString>(first, second);
}

//this function tells if a given string can be a behaviour
bool possible_behaviour(QString string)
{
    bool result(false);
    if (string.size() == 3)
    {
        QChar first = string[0];
        QChar second = string[1];
        QChar third = string[2];
        if (first == 'i' || first == 'c' || first == 'n' || first == 'r')
        {
            if (second == 'i' || second == 'c' || second == 'n' || second == 'r' || second == 'm')
            {
                if (third == 'i' || third == 'n' || third == 'r')
                {result = true;}
            }
        }
    }
    return result;
}

//this function return an updated behaviour string
QString update_behaviour(int number, QString behaviour, QString new_char)
{
    QString result("");
    int i(0);
    for (; i != number; ++i)
    {result.push_back(behaviour[i]);}
    ++i;
    result += new_char;
    for (; i < 3; ++i)
    {result.push_back(behaviour[i]);}
    return result;
}
