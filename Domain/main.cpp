#include <iostream>
#include <QApplication>
#include "View/WindowMain.h"
#include "View/Canvas.h"
#include "Domain/Shader.h"

class WindowMain: public Ui_WindowMain
{ public: WindowMain() : Ui_WindowMain() {} };

WindowMain windowMain;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget *widget = new QWidget;
    windowMain.setupUi(widget);
    widget->show();

    return app.exec();
}