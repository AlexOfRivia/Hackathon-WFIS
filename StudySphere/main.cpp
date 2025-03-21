#include "StudySphere.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StudySphere w;
    w.show();
    return a.exec();
}
