#include "header_downloader.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    header_downloader w;
    w.show();

    return a.exec();
}
