/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "glwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    glwidget *openGLWidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QRadioButton *nearestRadioButton;
    QRadioButton *linearRadioButton;
    QRadioButton *linearMipmapRadioButton;
    QGroupBox *groupBox_2;
    QRadioButton *staticRadioButton;
    QRadioButton *dynamicRadioButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(670, 571);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        openGLWidget = new glwidget(centralWidget);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(openGLWidget->sizePolicy().hasHeightForWidth());
        openGLWidget->setSizePolicy(sizePolicy1);
        openGLWidget->setMinimumSize(QSize(500, 500));

        horizontalLayout->addWidget(openGLWidget);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        nearestRadioButton = new QRadioButton(groupBox);
        nearestRadioButton->setObjectName(QStringLiteral("nearestRadioButton"));
        nearestRadioButton->setGeometry(QRect(10, 20, 82, 17));
        nearestRadioButton->setChecked(true);
        linearRadioButton = new QRadioButton(groupBox);
        linearRadioButton->setObjectName(QStringLiteral("linearRadioButton"));
        linearRadioButton->setGeometry(QRect(10, 50, 82, 17));
        linearMipmapRadioButton = new QRadioButton(groupBox);
        linearMipmapRadioButton->setObjectName(QStringLiteral("linearMipmapRadioButton"));
        linearMipmapRadioButton->setGeometry(QRect(10, 80, 82, 17));

        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        staticRadioButton = new QRadioButton(groupBox_2);
        staticRadioButton->setObjectName(QStringLiteral("staticRadioButton"));
        staticRadioButton->setGeometry(QRect(10, 20, 82, 17));
        staticRadioButton->setChecked(true);
        dynamicRadioButton = new QRadioButton(groupBox_2);
        dynamicRadioButton->setObjectName(QStringLiteral("dynamicRadioButton"));
        dynamicRadioButton->setGeometry(QRect(10, 50, 82, 17));

        verticalLayout->addWidget(groupBox_2);


        horizontalLayout->addLayout(verticalLayout);

        horizontalLayout->setStretch(0, 2);
        horizontalLayout->setStretch(1, 1);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 670, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);
        QObject::connect(linearMipmapRadioButton, SIGNAL(toggled(bool)), openGLWidget, SLOT(mipmap_toogled(bool)));
        QObject::connect(nearestRadioButton, SIGNAL(toggled(bool)), openGLWidget, SLOT(nearest_toogled(bool)));
        QObject::connect(linearRadioButton, SIGNAL(toggled(bool)), openGLWidget, SLOT(linear_toogled(bool)));
        QObject::connect(staticRadioButton, SIGNAL(toggled(bool)), openGLWidget, SLOT(static_toogled(bool)));
        QObject::connect(dynamicRadioButton, SIGNAL(toggled(bool)), openGLWidget, SLOT(dynamic_toogled(bool)));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "GroupBox", 0));
        nearestRadioButton->setText(QApplication::translate("MainWindow", "Nearest", 0));
        linearRadioButton->setText(QApplication::translate("MainWindow", "Linear", 0));
        linearMipmapRadioButton->setText(QApplication::translate("MainWindow", "Linear mipmap", 0));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "GroupBox", 0));
        staticRadioButton->setText(QApplication::translate("MainWindow", "static", 0));
        dynamicRadioButton->setText(QApplication::translate("MainWindow", "dynamic", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
