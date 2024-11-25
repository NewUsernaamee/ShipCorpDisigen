#ifndef __DIALOGWDGT_H
#define __DIALOGWDGT_H
#include <qdialog>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qstring.h>
#include <QObject>
#include <qevent.h>
#include <qlabel.h>
#include <qcheckbox.h>

// Класс диалогового окна предназначенного для получения названия кривой, также узнаёт у пользователя нужно ли копировать предыдущую кривую.
class DialogWdgt:public QDialog
{
	Q_OBJECT
	
	QLabel* m_label;
	QLineEdit* m_NameLine;
	QPushButton* m_EndRedactingButton;
	QString m_WrotedName;
	QCheckBox* m_copyPreviousSpline ;

public:

	
	DialogWdgt(QWidget* parent=nullptr);
	//получить название сплайна
	QString getSplineName();
	//очистить поле для ввода названия
	void clearNameLine();
	//возвращает занчения true если нужно скопировать предыдущий сплайн
	bool isCopyPrevious();
	//показывает копирование сплайна если существует хотя бы один сплайн
	void showPriviosSplineCheckBox();

public slots:
	//прячет самого себя
	void selfHide();
	
signals:
	//посылает название сплайна
	void signalToTakeFirstLast(std::string);
};
#endif