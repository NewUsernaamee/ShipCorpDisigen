#ifndef __SPLINELISTWDGT_H
#define __SPLINELISTWDGT_H

#include <QListWidget>
#include <qstring.h>
#include "DialogWdgt.h"
#include "DialogWindow.h"
#include <ShipSection.h>

//виджет отображающий названия сплайнов и позволяющий переключать активный слой 
class SplinesList :
    public QListWidget
{   

    Q_OBJECT
    DialogWdgt* m_NameTaker = new DialogWdgt(this->parentWidget());
    QString m_SplineName="spline 1";

    // Приватный метод для создания элементов управления.
    void createControls();

public:
    // Вызывается при загрузке модели.
    void modelLoaded(std::vector<std::string>& name);
    SplinesList(QWidget* parent = nullptr);

public slots:
    // Удаляет кривую из списка.
    void removeSpline();

    // Добавляет первую или последнюю кривую в зависимости от нажатого элемента управления.
    void addFirstLastSpline(std::string names);

    // Вызывает диалоговое окно для получения названия кривой.
    void callDialogWdgt(QListWidgetItem* item = nullptr);

signals:
    // Сигнал для вставки копии предыдущего слоя.
    void insertCopyOfprevious();

    // Сигнал для добавления нулевого слоя.
    void pushNULL(size_t x);

    // Сигнал, указывающий, что слой был создан.
    void layerCreated();
};

#endif