#include "mainwindow.h"
#include <QApplication>
#include <QTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QTime last_time = QTime::currentTime();
    while (!w.is_closed())
    {
        a.processEvents();
        QTime cur_time = QTime::currentTime();
        float dt = last_time.msecsTo(cur_time) / 1000.;
        last_time = cur_time;
        w.glwidget().do_turn(dt);
    }

    return 0;
}
