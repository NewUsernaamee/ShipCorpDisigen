#include "customTreeItem.h"

customTreeItem::customTreeItem(QTreeWidgetItem* parent, QString cordname, QString cord, size_t pointNumber, std::shared_ptr<ShipSection> section):QTreeWidgetItem(parent)
{
	m_data = new CordLineEdit(nullptr, pointNumber, section, cordname);
	m_data->setPlaceholderText(cordname);
	m_data->setValidator(new QDoubleValidator(0, DBL_MAX, 2, m_data));
	m_data->setText(cord);
	parent->treeWidget()->setItemWidget(this, 0, m_data);
}

