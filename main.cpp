#include "widgetsfwd.h"

#include <QApplication>
#include <QLocale>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile qss(":/style.qss");
    if (qss.open(QIODevice::ReadOnly))
    {
        qApp->setStyleSheet(qss.readAll());
        qss.close();
    }

    loginWidget w;
    w.show();
    return a.exec();
}
