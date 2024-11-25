#include "DialogWdgt.h"



DialogWdgt::DialogWdgt(QWidget* parent): QDialog (parent),
	m_label(new QLabel("Enter spline name", this)),
	m_NameLine(new QLineEdit(this)),
	m_EndRedactingButton(new QPushButton("End editing", this)),
	m_WrotedName(QString("sline")),
	m_copyPreviousSpline(new QCheckBox("Copy Previous Section?", this))
{
    connect(this->m_EndRedactingButton, &QPushButton::clicked, this, &DialogWdgt::selfHide);
	this->setFixedSize(200,100);
	m_NameLine->setFixedWidth(200);
	m_NameLine->move(0,20);
	m_label->move(m_NameLine->x(), m_NameLine->y() - 15);
	m_EndRedactingButton->move(60, 65);
	this->setWindowModality(Qt::WindowModality::WindowModal);
	m_copyPreviousSpline->move(m_NameLine->pos().x()+2,m_NameLine->pos().y()+22);
	m_copyPreviousSpline->hide();
}

QString DialogWdgt::getSplineName()
{
	return this->m_WrotedName;
}
void DialogWdgt::clearNameLine()
{
	this->m_NameLine->clear();
}
bool DialogWdgt::isCopyPrevious()
{
	auto isChecked = m_copyPreviousSpline->isChecked();
	m_copyPreviousSpline->setChecked(false);
	return isChecked;
}
void DialogWdgt::showPriviosSplineCheckBox()
{
	m_copyPreviousSpline->show();
}
void DialogWdgt::selfHide()
{	
	if(this->m_NameLine->text().simplified()!="")
	{
		this->m_WrotedName=this->m_NameLine->text();
		this->hide();
		emit signalToTakeFirstLast(std::string{});
	}
}
