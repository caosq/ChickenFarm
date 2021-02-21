#include "mainform.h"
#include "global.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainForm::initButton();
    MainForm w;
    w.show();

    return a.exec();
}
