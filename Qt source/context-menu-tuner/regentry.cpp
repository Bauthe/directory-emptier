#include "regentry.h"

RegEntry::RegEntry(QString new_name, QString new_behaviour, bool new_recursive, bool new_conservative, bool saved)
{
    name = new_name;
    first_behaviour = new_behaviour[0];
    second_behaviour = new_behaviour[1];
    third_behaviour = new_behaviour[2];
    recursive = new_recursive;
    conservative = new_conservative;
    saved_entry = "";
    if (saved)
    {saved_entry = save_code();}
}

RegEntry::RegEntry(QString code)
{
    saved_entry = code;

    if (code.back() == '0')
    {conservative = false;}
    else {conservative = true;}
    code.chop(1);

    if (code.back() == '0')
    {recursive = false;}
    else {recursive = true;}
    code.chop(1);

    third_behaviour = code.back();
    code.chop(1);
    second_behaviour = code.back();
    code.chop(1);
    first_behaviour = code.back();
    code.chop(1);

    name = code;
}

QString RegEntry::behaviour()
{
    return QString(first_behaviour) + QString(second_behaviour) + QString(third_behaviour);
}

QString RegEntry::explicit_name()
{
    QString result("directory_emptier_" + behaviour());
    if (recursive)
    {result += "_r";}
    if (conservative)
    {result += "_c";}
    return result;
}

QString RegEntry::save_code()
{
    QString result(name);
    result += behaviour();
    if (recursive)
    {result += "1";}
    else {result += "0";}
    if (conservative)
    {result += "1";}
    else {result += "0";}
    return result;
}

bool RegEntry::to_save()
{return (save_code() != saved_entry);}

bool RegEntry::add()
{
    //checking redundancy
    QString exname(explicit_name());
    QVector<RegEntry*> entries(get_save());
    bool existing(false);
    for (int i(0); i < entries.size(); ++i)
    {
        if (entries[i]->explicit_name() == exname)
        {existing = true;}
    }
    if (existing)
    {return false;}

    //removing old version
    if (saved_entry != "")
    {RegEntry(saved_entry).remove();}

    //editing the registry
    QString path_to_key("HKEY_CLASSES_ROOT\\Directory\\shell\\" + exname);
    QSettings settings(path_to_key, QSettings::NativeFormat);
    settings.setValue(".", name);
    settings.setValue("icon", "\"" + get_path() + "\\directory-emptier.exe\",0");
    QString options("\"-b\" \"" + behaviour() + "\"");
    if (recursive)
    {options += " \"-r\"";}
    if (conservative)
    {options += " \"-c\"";}
    QSettings command_settings(path_to_key + "\\command", QSettings::NativeFormat);
    command_settings.setValue(".", "\"" + get_path() + "\\directory-emptier.exe\" " + options + " \"%V\"");

    //saving in local file
    QFile file(get_path() + "/regentries");
    file.open(QIODevice::Append | QIODevice::Text);

    QTextStream stream(&file);
    stream << save_code() << '\n';

    //acknowledging the change
    saved_entry = save_code();

    return true;
}

bool RegEntry::remove()
{
    //editing the registry
    QString path_to_key("HKEY_CLASSES_ROOT\\Directory\\shell");
    QSettings(path_to_key, QSettings::NativeFormat).remove(explicit_name());

    //removing in local file
    QFile file(get_path() + "/regentries");
    file.open(QIODevice::ReadWrite | QIODevice::Text);

    QString new_content;
    QString content_to_remove(save_code());
    QTextStream stream(&file);
    while(!stream.atEnd())
    {
        QString line = stream.readLine();
        if(line != content_to_remove)
        {new_content.append(line + "\n");}
    }
    file.resize(0);
    stream << new_content;

    //acknowledging the change
    saved_entry = "";

    return true;
}

QVector<RegEntry*> get_save()
{
    QVector<RegEntry*> result;

    QFile file(get_path() + "/regentries");
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream stream(&file);
    QString current_entry;
    while(! stream.atEnd())
    {
        current_entry = stream.readLine();
        RegEntry* address = new RegEntry(current_entry);
        result.push_back(address);
    };
    return result;
}

QString get_path()
{
    return QDir::toNativeSeparators(QApplication::applicationDirPath());
}
