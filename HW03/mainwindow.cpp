#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , is_closed_(false)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

glwidget& MainWindow::glwidget()
{
    return *ui->openGLWidget;
}

bool MainWindow::is_closed()
{
    return is_closed_;
}

void MainWindow::closeEvent(QCloseEvent* /*e*/)
{
    is_closed_ = true;
}