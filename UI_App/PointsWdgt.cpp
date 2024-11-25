#include <PointsWdgt.h>

PointsWdgt::PointsWdgt(QWidget* parent) :
	QTreeWidget(parent)
{
	setHeaderLabel("secions");
}

void PointsWdgt::addSections()
{
	static int numberOfSections=1;
	static int numberOfPoints = 1;
	for (size_t i = 0; i < m_CurrentModel->getAllSections().size(); i++)
	{
		if (!m_CurrentModel->getSection(i).get()->isNullLayer())
		{

			QTreeWidgetItem* item = new QTreeWidgetItem(this, QStringList("sections" + QString::number(numberOfSections)), 0);
			addTopLevelItem(item);
			for (size_t x = 0; x < m_CurrentModel->getAllSections()[i].getPoints().Count(); x++)
			{
				QTreeWidgetItem* secondLvlItem = new QTreeWidgetItem(item, QStringList("point " + QString::number(numberOfPoints)), 0);
				item->addChild(secondLvlItem);
				numberOfPoints++;
				customTreeItem* childX = new customTreeItem(secondLvlItem,"x", QString::number((*(m_CurrentModel->getAllSections()[i].getPoints().begin() + x)).x),numberOfPoints-1,m_CurrentModel->getSection(i));
				customTreeItem* childY = new customTreeItem(secondLvlItem,"y", QString::number((*(m_CurrentModel->getAllSections()[i].getPoints().begin() + x)).y),numberOfPoints-1, m_CurrentModel->getSection(i));
				secondLvlItem->addChild(childX);
				secondLvlItem->addChild(childY);
			}
			numberOfPoints = 1;
			numberOfSections++;
		}
	}
	numberOfSections = 1;
}

void PointsWdgt::SynchronizeWithModel(ShipModel* model)
{
	this->clear();
	m_CurrentModel = model;
	addSections();

}
