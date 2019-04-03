#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setEffectEnabled(Qt::UI_AnimateCombo, false);
    MainWindow w;
    w.show();

    return a.exec();
}
