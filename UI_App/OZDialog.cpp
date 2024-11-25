#include "OZDialog.h"


OZDialog::OZDialog(QWidget* parent) : QDialog(parent)
{
	this->setFixedSize(300, 70);
	this->m_OZLineEdit->setFixedWidth(200);
	this->m_OZLineEdit->move(50,15);
	m_button->setFixedWidth(60);
	this->m_button->move(120, 40);
	this->m_label->move(m_OZLineEdit->pos().x()+30, m_OZLineEdit->pos().y()-15);
	this->hide();
	QObject::connect(m_OZLineEdit, &QLineEdit::editingFinished, [this]
		{
			this->hide();
			emit TakeOzCord(); 
		});

	this->setWindowModality(Qt::WindowModality::WindowModal);
	m_OZLineEdit->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 2, m_OZLineEdit));
}

void OZDialog::closeEvent(QCloseEvent* event)
{
	event->ignore();
}
void OZDialog::reject() {}

double OZDialog::getOzCords()
{
	auto str =this->m_OZLineEdit->text().toDouble();
	m_OZLineEdit->clear();
	return str;
}