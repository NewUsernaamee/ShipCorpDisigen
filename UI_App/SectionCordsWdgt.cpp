#include "SectionCordsWdgt.h"

constexpr auto DIGITS_COUNT = 2;
constexpr auto GRID_STEP = 20;

void SectionCordsWdgt::RewriteOXCord()
{
	if(!m_isCurrentlayerNull)
	emit SendCords(m_OXEdit->text().toDouble(), 'x');
}
void SectionCordsWdgt::RewriteOYCord()
{
	if (!m_isCurrentlayerNull)
	emit SendCords(m_OYEdit->text().toDouble(), 'y');
}
void SectionCordsWdgt::RewriteOZCord()
{
	if (!m_isCurrentlayerNull)
	emit SendCords(m_OZEdit->text().toDouble(), 'z');
}

SectionCordsWdgt::SectionCordsWdgt(QWidget* parent): QWidget(parent),
m_OXLabel (new QLabel("X = ", this)),
m_OYLabel (new QLabel("Y = ", this)),
m_OZLabel (new QLabel("Z = ", this)),
m_OXEdit (new QLineEdit(this)),
m_OYEdit (new QLineEdit(this)),
m_OZEdit (new QLineEdit(this))
{
	m_OXEdit->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, DIGITS_COUNT, m_OXEdit));
	m_OYEdit->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, DIGITS_COUNT, m_OYEdit));
	m_OZEdit->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, DIGITS_COUNT, m_OZEdit));

	m_OXEdit->setFixedWidth(50);
	m_OYEdit->setFixedWidth(50);
	m_OZEdit->setFixedWidth(50);
	m_OXLabel->move(10, 5);
	m_OYLabel->move(m_OXLabel->pos().x(), m_OXLabel->pos().y() + m_OXLabel->height() + 10);
	m_OZLabel->move(m_OXLabel->pos().x(), m_OYLabel->pos().y() + m_OYLabel->height() + 10);
	m_OXEdit->move(m_OXLabel->pos().x() + GRID_STEP, m_OXLabel->pos().y() - 2); 
	m_OYEdit->move(m_OYLabel->pos().x() + GRID_STEP, m_OYLabel->pos().y() - 2);
	m_OZEdit->move(m_OZLabel->pos().x() + GRID_STEP, m_OZLabel->pos().y() - 2);
	connections();

}
void SectionCordsWdgt::connections()
{
	QObject::connect(m_OXEdit, &QLineEdit::editingFinished, this, &SectionCordsWdgt::RewriteOXCord);
	QObject::connect(m_OYEdit, &QLineEdit::editingFinished, this, &SectionCordsWdgt::RewriteOYCord);
	QObject::connect(m_OZEdit, &QLineEdit::editingFinished, this, &SectionCordsWdgt::RewriteOZCord);
}

void SectionCordsWdgt::setZCord(double x)
{
	this->m_OZEdit->setText(QString::number(x));
}

void SectionCordsWdgt::SynchronizeSectionCords(MbCartPoint3D ChosenSection, bool isNullLayer)
{
	m_isCurrentlayerNull = isNullLayer;
	m_OXEdit->setText(QString::number(ChosenSection.x));
	m_OYEdit->setText(QString::number(ChosenSection.y));
	m_OZEdit->setText(QString::number(ChosenSection.z));
}




