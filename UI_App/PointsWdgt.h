#ifndef __POINTSWDGT_H
#define __POINTSWDGT_H

#include <QTreeWidget>
#include <qstring.h>
#include "DialogWdgt.h"
#include <ShipSection.h>
#include <QListWidgetItem>
#include <ShipModel.h>
#include <qlineedit.h>
#include <customTreeItem.h>

//Виджет дерева содержащий данные о модели и позволяющий редактировать точки модели.
class PointsWdgt: public QTreeWidget
{
public:
	PointsWdgt(QWidget* parent=nullptr);
private:
	ShipModel* m_CurrentModel{nullptr};
	
	//Добавить секцию в дерево.
	void addSections();
public slots:
	
	//Синхронизировать с моделью.
	void SynchronizeWithModel(ShipModel*);
};

#endif __POINTSWDGT_H