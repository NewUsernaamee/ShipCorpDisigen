#pragma once
#include "qlineedit.h"
#include <ShipSection.h>

//класс для ввода пользователем координат точек в виджете дерева
class CordLineEdit :
    public QLineEdit
{
public:
    CordLineEdit(QWidget *parent, size_t pointNumber, std::shared_ptr<ShipSection> section, QString cordname);
private:
    std::shared_ptr<ShipSection> m_currentSection;
    size_t m_pointNumber;
    QString m_cordname;
private slots:
    //перезаписать координаты точки, вызыва при завершении редактирования поля
    void rewriteCords();

};

