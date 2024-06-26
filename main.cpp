#include "hammockwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HammockWidget w;
    w.resize(800, 600); // Установить размер окна
    w.show();
    return a.exec();
}
