#ifndef GUIENTRY_H
#define GUIENTRY_H

#include <QWidget>
#include <QLayout>
#include <QCheckBox>
#include <QStackedWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>

#include "regentry.h"

class GuiEntry: public QWidget
{
    Q_OBJECT

    public:

        GuiEntry();
        GuiEntry(QString code);
        GuiEntry(RegEntry& entry);

        bool is_selected();

        bool save();

        RegEntry* s_entry;

    public slots:

        void start_edit_name();
        void edit_name_finished();

        void update_first_behaviour(const QString& string);
        void update_second_behaviour(const QString& string);
        void update_third_behaviour(const QString& string);

        void update_recursive(int state);
        void update_conservative(int state);

    private:

        static QString enter_name_here;

        QHBoxLayout* s_layout;
        QCheckBox* s_checkbox;
        QStackedWidget* s_stacked;
        QPushButton* s_button;
        QLineEdit* s_line_edit;
        QComboBox* s_first_behaviour;
        QComboBox* s_second_behaviour;
        QComboBox* s_third_behaviour;
        QCheckBox* s_recursive;
        QCheckBox* s_conservative;
};

void set_check_box(QCheckBox& box, bool value);

QString behaviour_to_string(QChar behaviour);
QChar string_to_behaviour(QString string);

#endif // GUIENTRY_H
