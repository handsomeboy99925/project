#include "home.h"
#include <QApplication>
#include <QTextCodec>
#include <QStyleFactory>
#include <QDebug>
#include "my_custom/digital_scroller.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Home w;
    w.show();
    return a.exec();
}
