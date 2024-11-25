#include "CuttingPlaneSettingsWdgt.h"

const QString m_threeDemensions = "Controller 3D";
const QString m_twoDemensions = "Controller 2D";

CuttingPlaneSettingsWdgt::CuttingPlaneSettingsWdgt(QWidget* parent):QWidget(parent),
	m_isInversedDirection(new QPushButton("invert direction",this)),
	m_ChangePlaneNavigationType(new QPushButton("Controller 2D", this)),
	m_IsEnable(new QPushButton("Enable Cutting plane",this)),
	countOfCuttingPlaces(new QLineEdit(this))
{
	m_isInversedDirection->move(0,50);
	m_IsEnable->move(0, 75);
	m_ChangePlaneNavigationType->move(0,100);
	countOfCuttingPlaces->move(0,25);
	countOfCuttingPlaces->setPlaceholderText("Count of Cutting sections");
	countOfCuttingPlaces->setValidator(new QIntValidator(0, INT_MAX, countOfCuttingPlaces));
	connect(this->countOfCuttingPlaces, &QLineEdit::returnPressed, [this] {emit SendCuttingLayerCount(countOfCuttingPlaces->text().toInt()); });
	connect(this->m_isInversedDirection, &QPushButton::clicked, [this] {emit DirectionChanged(); });
	connect(this->m_IsEnable, &QPushButton::clicked, this, &CuttingPlaneSettingsWdgt::EnableChanged);
	connect(this->m_ChangePlaneNavigationType, &QPushButton::clicked, this, &CuttingPlaneSettingsWdgt::SendNavigationTypeChanged);

}

void CuttingPlaneSettingsWdgt::SendNavigationTypeChanged()
{
	if (m_ChangePlaneNavigationType->text() == m_threeDemensions)
	{
		m_ChangePlaneNavigationType->setText(m_twoDemensions);
	}
	else
	{
		m_ChangePlaneNavigationType->setText(m_threeDemensions);
	}

	emit NavigationTypeChanged();
}

void CuttingPlaneSettingsWdgt::getActiveCutterSettings(bool isInverted, bool isEnable, VSN::Controller::ControllerViews type)
{
	if (type == VSN::Controller::ControllerViews::directCtrl)
	{
		m_ChangePlaneNavigationType->setText(m_twoDemensions);
	}
	else
	{
		m_ChangePlaneNavigationType->setText(m_threeDemensions);
	}

}