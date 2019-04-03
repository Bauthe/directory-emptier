#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QScrollArea>

#include "guientry.h"

class MainWindow : public QWidget
{
    Q_OBJECT

    public:

        MainWindow();

    private slots:

        void add_entry();
        void delete_selection();
        void delete_entry(int index);
        void save();
        void init();
        void quit();

    private:

        QScrollArea* s_area;
        QWidget* s_big_widget;
        QVBoxLayout* s_layout;
        QVector<GuiEntry*> s_entries;
        QVector<RegEntry*> s_removed_entries;

        QPushButton* s_add_button;
        QPushButton* s_delete_button;
        QPushButton* s_save_button;
        QPushButton* s_init_button;
        QPushButton* s_quit_button;
};

#endif // MAINWINDOW_H
