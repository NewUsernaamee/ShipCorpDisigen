#include "CordLineEdit.h"

CordLineEdit::CordLineEdit(QWidget* parent, size_t pointNumber, std::shared_ptr<ShipSection> section, QString cordname) :QLineEdit(parent)
{
	m_pointNumber = pointNumber;
	m_currentSection = section;
	this->m_cordname = cordname;
	QObject::connect(this,&QLineEdit::editingFinished,this,&CordLineEdit::rewriteCords);
}
void CordLineEdit::rewriteCords()
{
	auto str = this->text().replace(',', QString("."));
	if (m_cordname == QString("x"))
	{
		m_currentSection.get()->movePointByX(m_pointNumber, str.toDouble());
		return;
	}
	else if (m_cordname == QString("y"))
	{
		m_currentSection.get()->movePointByY(m_pointNumber, str.toDouble());
		return;
	}
}