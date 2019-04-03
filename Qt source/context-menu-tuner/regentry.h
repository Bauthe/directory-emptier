#ifndef REGENTRY_H
#define REGENTRY_H

#include <QApplication>

#include <QString>
#include <QVector>
#include <QDir>
#include <QTextStream>
#include <QSettings>

class RegEntry
{
    public:

        RegEntry(QString new_name, QString new_behaviour, bool new_recursive, bool new_conservative, bool saved);
        RegEntry(QString entrycode);

        QString behaviour();
        QString explicit_name();
        QString save_code();
        bool to_save();
        bool add();
        bool remove();

        QString name;
        QChar first_behaviour;
        QChar second_behaviour;
        QChar third_behaviour;
        bool recursive;
        bool conservative;
        QString saved_entry;
};

QVector<RegEntry*> get_save();
QString get_path();

#endif // REGENTRY_H
