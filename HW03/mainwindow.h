#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <glwidget.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    glwidget& glwidget();
    bool is_closed();
    void closeEvent(QCloseEvent *event) override;

private:
    bool is_closed_;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
