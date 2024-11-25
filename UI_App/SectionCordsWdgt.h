#pragma once
#include <qwidget.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <QDoubleValidator>
#include <limits.h>
#include <mb_cart_point3d.h>

//Виджет позволяющий изменять координаты выбраной кривой  
class SectionCordsWdgt :
    public QWidget
{
    Q_OBJECT

    QLabel*     m_OXLabel;
    QLabel*     m_OYLabel;
    QLabel*     m_OZLabel;
    QLineEdit*  m_OXEdit;
    QLineEdit*  m_OYEdit;
    QLineEdit*  m_OZEdit;
    bool m_isCurrentlayerNull;
    
    //настройка конектов.
    void connections();

public:
    SectionCordsWdgt(QWidget* parent);

    // Устанавливает значение координаты Z.
    void setZCord(double x);

public slots:
    
    // Синхронизирует координаты сечения с полученными данными.
    void SynchronizeSectionCords(MbCartPoint3D ChosenSectionCords, bool isNullLayer);

    // Перезаписывает значение координаты X.
    void RewriteOXCord();

    // Перезаписывает значение координаты Y.
    void RewriteOYCord();

    // Перезаписывает значение координаты Z.
    void RewriteOZCord();

signals:
    // Сигнал для отправки координат.
    void SendCords(double, char);


};

