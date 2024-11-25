#pragma once
#include "qtreewidget.h"
#include <qlineedit.h>
#include <ShipSection.h>
#include <CordLineEdit.h>

//Класс кастомного предмета для виджета дерева
class customTreeItem :
    public QTreeWidgetItem
{
    CordLineEdit* m_data;
public:
    customTreeItem(QTreeWidgetItem* parent, QString cordname, QString cord, size_t pointNumber, std::shared_ptr<ShipSection> section);
};

