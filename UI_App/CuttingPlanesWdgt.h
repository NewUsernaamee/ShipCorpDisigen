#pragma once
#include <qlistwidget.h>

//класс представляет собой список для переключения текущей секущей плоскости
class CuttingPlanesWdgt :
    public QListWidget
{
    Q_OBJECT

public:
    CuttingPlanesWdgt(QWidget *parent=nullptr);
    //добавляет секущую в виджет переключения
    void addCuttingPlane();
    /*Перезаполняет виджет
    * Если текущее количество меньше присланного числа, то добавит секции
    * Иначе если текущее количество больше присланного, удалит излишки
    */
    void refil(size_t count);
public slots:
    /*метод получает количество секущих кривых
    * если оно не совпадает с текущим количеством
    * вызывает метод refil()
    */
    void getCountOfCuttingPlanes(size_t);


};

