#ifndef WAITWINDOW_H
#define WAITWINDOW_H

#include <QApplication>

#include <QTimer>
#include <QThread>
#include <QDir>

#include <QStyle>
#include <QLayout>
#include <QDesktopWidget>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>

class WaitWindow : public QWidget
{
    Q_OBJECT

    public:

        WaitWindow(int argc, char *argv[]);
        void debug_exit(QString message);
        void closeEvent(QCloseEvent *event);

    private:

        QString ask_user(int conflict_number);
        void pre_treatment(QString item_path, QString origin_directory, QString destination, QStringList& destination_items);
        void effective_treatment(QString item_path, QString new_path);
        void merge(QString item_path, QString new_path);
        void recursive_copy(QString item_path, QString new_path);

        void add_merge_button(QVector<QPushButton*>& buttons);
        void add_crush_button(QVector<QPushButton*>& buttons);
        void add_ignore_button(QVector<QPushButton*>& buttons);
        void add_rename_button(QVector<QPushButton*>& buttons);

        void usage();

    private slots:

        void body();
        void quit();
        void box_changed(int state);
        void say_merge();
        void say_crush();
        void say_ignore();
        void say_rename();

    private:

        QChar s_user_response;
        bool s_ticked_box;
        QString s_current_item;

        QGridLayout *s_layout;
        QLabel* s_status_label;
        QPushButton* s_abandon_button;
        QTimer* s_timer;

        QStringList s_origin;
        bool s_recursive;
        bool s_conservative;
        QString s_behaviour;
        QStringList s_destination_items;
};

QString cut_all_dirs(QString path);
QString parent_dir(QString path);
void add_ls(QStringList& list, QString directory, bool absolute);
QString item_type(QString path);
QPair<QString, QString> name_and_ext(QString item);
bool possible_behaviour(QString string);
QString update_behaviour(int number, QString behaviour, QString new_char);

#endif // WAITWINDOW_H
