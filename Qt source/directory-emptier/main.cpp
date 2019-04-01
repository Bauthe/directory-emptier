//this source code is done exclusively for windows
//it may be quite dirty Qt-wise because it is an adaptation of a console app

//exit codes: 0 (normal exit), 1 (exit due to user's error), 2 (exit due to a program failure)

#include <QApplication>

#include "waitwindow.h"

int main(int argc, char *argv[])

{
    //doing Qt stuff
    QApplication app(argc, argv);

    WaitWindow window(argc, argv);
    window.show();

    return app.exec();
}
