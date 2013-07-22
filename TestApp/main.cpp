#include <QtGui/QApplication>
#include "TestApp.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    TestApp ta;
    ta.show();
    return app.exec();
}
