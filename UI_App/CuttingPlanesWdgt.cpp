#include "CuttingPlanesWdgt.h"

CuttingPlanesWdgt::CuttingPlanesWdgt(QWidget* parent): QListWidget(parent)
{

}

void CuttingPlanesWdgt::addCuttingPlane()
{
	addItem("Cutting section" + QString::number(this->count()));
}
void CuttingPlanesWdgt::refil(size_t count)
{
	if (count==0)
	{
		clear();
	}
	else
	{
		while (true)
		{
			if (count > this->count())
			{
				addCuttingPlane();
			}
			else if (count < this->count())
			{
				takeItem(this->count()-1);//-1
			}
			if (count == this->count())
			{
				if (this->count()>0)
				setCurrentRow(0);
//				emit currentRowChanged(0);
				return;
			}
		}
	}
}
void CuttingPlanesWdgt::getCountOfCuttingPlanes(size_t count)
{
	if (count != this->count())
	{
		refil(count);
	}
}
