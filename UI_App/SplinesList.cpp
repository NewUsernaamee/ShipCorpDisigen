#include "SplinesList.h"

bool LoadedModel=false;
bool IsFirstSpline = true;

SplinesList::SplinesList(QWidget* parent)  :
	QListWidget(parent) {
	QObject::connect(this->m_NameTaker, &DialogWdgt::signalToTakeFirstLast, this, &SplinesList::addFirstLastSpline);
	QObject::connect(this, &SplinesList::itemClicked, this, &SplinesList::callDialogWdgt);
	this->createControls();
}

void SplinesList::removeSpline()
{
	if (this->currentRow()==0 || this->currentRow()+1 == this->count())
	{
		return;
	}
	this->takeItem(this->currentRow());
	this->setCurrentRow(currentRow()-1);
}
void SplinesList::addFirstLastSpline(std::string name)
{
	
	if (LoadedModel)
	{
		this->insertItem(this->count() - 1, QString::fromStdString(name));
		qDebug("Added");
	}
	else{
		if (this->currentRow() == 0)
		{
			
			m_SplineName = m_NameTaker->getSplineName();
			this->insertItem(1, m_SplineName);
			if (m_NameTaker->isCopyPrevious())
			{
				emit insertCopyOfprevious();
			}
			else
				emit pushNULL(0);
			qDebug("Added");
			this->m_NameTaker->clearNameLine();
			this->setCurrentRow(currentRow() + 1);

		}
		else if (this->currentRow() == this->count()-1)
		{
			
			m_SplineName = m_NameTaker->getSplineName();
			this->insertItem(this->count()-1, m_SplineName);
			if (m_NameTaker->isCopyPrevious())
			{
				emit insertCopyOfprevious();
			}
			else
				emit pushNULL(this->count() - 2);
			qDebug("Added");
			this->m_NameTaker->clearNameLine();
			this->setCurrentRow(currentRow() -1);
			
		}
	}
	if (IsFirstSpline)
	{
		m_NameTaker->showPriviosSplineCheckBox();
		IsFirstSpline = false;
	}
	
}


void SplinesList::callDialogWdgt( QListWidgetItem* item)
{
	if (this->currentRow() == 0 ||(this->currentRow() == this->count() - 1))
	this->m_NameTaker->show();
}
void SplinesList::createControls()
{
	QListWidgetItem* first = new QListWidgetItem("add upper layer");
	QListWidgetItem* last = new QListWidgetItem("add lower layer");
	this->addItem(first);
	this->addItem(last);

}
void SplinesList::modelLoaded(std::vector<std::string>& names)
{
	clear();
	createControls();
	LoadedModel=true;
	for (size_t i = 0; i < names.size(); i++)
	{
		addFirstLastSpline(names[i]);
	}
	LoadedModel = false;
	
}