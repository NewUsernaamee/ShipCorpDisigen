#pragma once
#include "qdialog"
#include "qlineedit.h"
#include "qwidget.h"
#include <qlabel.h>
#include <qpushbutton.h>
#include <qevent.h>
#include <qmessagebox.h>
#include <qvalidator.h>

//класс диалогового окна предназначенный для получения координаты OZ
class OZDialog :public QDialog
{
    Q_OBJECT
    QLineEdit* m_OZLineEdit=new QLineEdit(this);
    QLabel* m_label = new QLabel("Enter Z cord for this spline",this);
    QPushButton* m_button = new QPushButton("OK", this );
    // Переопределенный метод для обработки события закрытия окна, не позволяет закрыть окно если не введена координата.
    void closeEvent(QCloseEvent* event) override;

    // Переопределенный метод для отклонения события, не позволяет закрыть окно если не введена координата.
    void reject() override;

signals:
    // Сигнал для получения координат OZ.
    void TakeOzCord();

public:
    // Возвращает значение координаты OZ.
    double getOzCords();

    OZDialog(QWidget* parent=nullptr);
};

