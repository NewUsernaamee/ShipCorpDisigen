#pragma once
#include <qwidget.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <controller.h>
#include <qlineedit.h>
#include <qvalidator.h>

//класс предоставляющий возможность изменять настройки секущих плоскостей,
//представляет виизуальный интерфейс в виде 3 кнопок и одного изменяемого текстового поля
class CuttingPlaneSettingsWdgt :
    public QWidget
{
    Q_OBJECT

    QPushButton* m_isInversedDirection;
    QPushButton* m_IsEnable;
    QPushButton* m_ChangePlaneNavigationType;
    QLineEdit* countOfCuttingPlaces;
public:
    CuttingPlaneSettingsWdgt(QWidget* parent=nullptr);
    // отослать тип навигации
    void SendNavigationTypeChanged();

public slots:
    //метод устанавливает текущие настройки режущей плоскости
    void getActiveCutterSettings(bool isInverted, bool isEnable, VSN::Controller::ControllerViews type);
signals:
    // отослать количесвто режущих плоскостей
    void SendCuttingLayerCount(size_t);
    //сигнал для смены направления секущей плоскости 
    void DirectionChanged();
    //сигнал для смены типа управления секущей плоскости
    void NavigationTypeChanged();
    //сигнал для включения/выключения секущей плоскости
    void EnableChanged();
};

