#ifndef __DRAWINGWDGT_H
#define __DRAWINGWDGT_H

#include "ShipSection.h"
#include <QtWidgets/QWidget>
#include <QPainter>
#include <QColor>
#include <qpushbutton>
#include <qevent.h>
#include <qvector>
#include <QWheelEvent>
#include <iostream>
#include <algorithm>
#include <shipmodel.h>
#include <curve.h>
#include <templ_sptr.h>
#include <cur_polycurve.h>

//виджет для рисования и изменения кривых
class DrawingWdgt : public QWidget
{
    Q_OBJECT

public:
    DrawingWdgt(QWidget* parent = nullptr, int new_drawX = 150, int new_drawY = 150);
    std::vector<MbCartPoint> m_coordinatesArray;

    // Очищает массив точек предназнаяенный для создания кривой.
    void clearMutablePoints();

    // Изменяет режим сетки.
    void changeGridMod(bool mod);

    //создать секцию из полученных точек.
    void SetLayer();

protected:
    // Отрисовка.
    void paintEvent(QPaintEvent*);

    // Преобразование координат мыши в координаты сцены.
    QPointF calculateCoordinates(QPointF mouse_cords);

    // Преобразование координат сцены в пиксельные координаты.
    QPoint calculatePixels(QPointF point, double ShiftX = 0.f, double ShiftY = 0.f);

    // Создание QVector из массива точек MbCartPoint.
    QVector<QPointF> createQVector(SArray<MbCartPoint>* saray);

    // Проверяет, какая точка соответствует координатам мыши.
    void whichPointMatchMouseCords(QMouseEvent* event);

private slots:

    // Обработка события нажатия мыши.
    void mousePressEvent(QMouseEvent* e) override;

    // Обработка события перемещения мыши.
    void mouseMoveEvent(QMouseEvent* event) override;

    // Обработка события отпускания мыши.
    void mouseReleaseEvent(QMouseEvent* event) override;

public slots:
    // Включить режим удаления точки.
    void isDelitingMod();

    // Установить индекс активного слоя.
    void setActiveLayerIndex(size_t index);

    // Разблокировать рисование.
    void unlockDrawing();

    void drawChosenLayer(size_t shipSection);

    // Изменить активную модель.
    void changeActiveModel(ShipModel*);

    // Обновить виджет после загрузки данных.
    void loadedDraw();

    // Заблокировать рисование.
    void lockDrawing();

    // Поставить флаг m_IsModelLoaded в положение true
    void LoadedModel();

    // Поставить флаг m_IsModelLoaded в положение false
    void UnlockModelLoaded();

private:

    QPointF m_mousePos;
    QVector<QPointF> m_pointsArray;
    QVector<QPointF> m_hotpointsArray;
    double m_scale = 1.;
    int m_drawX;
    int m_drawY;
    bool m_gotEndPoint = false;
    ShipModel* m_modelPointer{ nullptr };

    // Обработать нажатие правой кнопки мыши.
    void processRightMouseButton();

    // Обработать нажатие левой кнопки мыши.
    void processLeftMouseButton(QMouseEvent* e);

    // Обработать действия с левой кнопкой мыши.
    void processActionsLeftMouseButton(QMouseEvent* e);

    // Рисовать точки для удаления и добавления точек.
    void drawDeletingAndAddingPoints(QPainter& painter);

    // Добавить точку в локальную модель.
    void AddPointToLocalModel(QPointF point);

    // Рисовать кривые из модели.
    void DrawModelsCurves(QPainter& painter);

signals:

    // Сигнал для передачи секцмм.
    void PushLayer(std::shared_ptr<ShipSection> shipSection);

    // Сигнал для перемещения точки модели.
    void MoveModelPoint(double x, double y, size_t indexOfPoint, size_t indexOfSection);

    // Сигнал для ввода координат OZ.
    void EnterOZCord();

    // Сигнал для удаления точки.
    void deletePoint(size_t);

    // Сигнал для добавления точки.
    void addPoint(MbCartPoint point);
};

#endif