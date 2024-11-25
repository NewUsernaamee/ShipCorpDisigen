/********************************************************************************
** Form generated from reading UI file 'MainWnd.ui'
**
** Created by: Qt User Interface Compiler version 6.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWND_H
#define UI_MAINWND_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *save;
    QAction *saveAs;
    QAction *open;
    QAction *action_3D;
    QAction *action_2D;
    QAction *loadBackground;
    QAction *loadcustoBack;
    QAction *action_3D_2;
    QWidget *centralwidget;
    QMenuBar *menuBar;
    QMenu *fileMenu;
    QMenu *menu;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(900, 700);
        MainWindow->setMinimumSize(QSize(900, 700));
        MainWindow->setCursor(QCursor(Qt::ArrowCursor));
        save = new QAction(MainWindow);
        save->setObjectName(QString::fromUtf8("save"));
        saveAs = new QAction(MainWindow);
        saveAs->setObjectName(QString::fromUtf8("saveAs"));
        open = new QAction(MainWindow);
        open->setObjectName(QString::fromUtf8("open"));
        action_3D = new QAction(MainWindow);
        action_3D->setObjectName(QString::fromUtf8("action_3D"));
        action_2D = new QAction(MainWindow);
        action_2D->setObjectName(QString::fromUtf8("action_2D"));
        loadBackground = new QAction(MainWindow);
        loadBackground->setObjectName(QString::fromUtf8("loadBackground"));
        loadcustoBack = new QAction(MainWindow);
        loadcustoBack->setObjectName(QString::fromUtf8("loadcustoBack"));
        action_3D_2 = new QAction(MainWindow);
        action_3D_2->setObjectName(QString::fromUtf8("action_3D_2"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setBaseSize(QSize(650, 648));
        centralwidget->setCursor(QCursor(Qt::CrossCursor));
        MainWindow->setCentralWidget(centralwidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 900, 22));
        fileMenu = new QMenu(menuBar);
        fileMenu->setObjectName(QString::fromUtf8("fileMenu"));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(fileMenu->menuAction());
        menuBar->addAction(menu->menuAction());
        fileMenu->addAction(save);
        fileMenu->addAction(open);
        fileMenu->addAction(loadBackground);
        fileMenu->addAction(loadcustoBack);
        menu->addAction(action_3D);
        menu->addAction(action_3D_2);
        menu->addAction(action_2D);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\320\237\321\200\320\270\320\273\320\276\320\266\320\265\320\275\320\270\320\265", nullptr));
        save->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", nullptr));
        saveAs->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\272\320\260\320\272...", nullptr));
        open->setText(QCoreApplication::translate("MainWindow", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214", nullptr));
        action_3D->setText(QCoreApplication::translate("MainWindow", "\320\237\321\200\320\265\320\264\321\201\321\202\320\260\320\262\320\270\321\202\321\214 \320\262 3D \320\272\320\260\320\272 \320\277\320\276\320\262\320\265\321\200\321\205\320\275\320\276\321\201\321\202\321\214", nullptr));
        action_2D->setText(QCoreApplication::translate("MainWindow", "\320\237\320\265\321\200\320\265\320\271\321\202\320\270 \320\262 2D \321\200\320\265\320\264\320\260\320\272\321\202\320\276\321\200", nullptr));
        loadBackground->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214 \321\201\320\265\321\202\320\272\321\203", nullptr));
        loadcustoBack->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214 \320\267\320\260\320\274\320\265\320\275\321\203 \321\201\320\265\321\202\320\272\320\270", nullptr));
        action_3D_2->setText(QCoreApplication::translate("MainWindow", "\320\237\321\200\320\265\320\264\321\201\321\202\320\260\320\262\320\270\321\202\321\214 \320\262 3D \320\272\320\260\320\272 \320\272\320\276\321\200\320\277\321\203\321\201", nullptr));
        fileMenu->setTitle(QCoreApplication::translate("MainWindow", "\320\244\320\260\320\271\320\273", nullptr));
        menu->setTitle(QCoreApplication::translate("MainWindow", "\320\237\321\200\320\265\320\264\321\201\321\202\320\260\320\262\320\270\321\202\321\214 \320\262", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWND_H
