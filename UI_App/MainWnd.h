#ifndef __MAINWND_H
#define __MAINWND_H
#include <QComboBox>
#include <ShipSection.h>
#include <FileManipulation.h>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QHBoxLayout>
#include "ui_MainWnd.h"
#include "DrawingWdgt.h"
#include <PointsWdgt.h>
#include <SectionCordsWdgt.h>
#include <qwindow>
#include <qpushbutton>
#include <QScrollArea>
#include <QToolBar>
#include <QMenuBar>
#include <QActionGroup>
#include <QFileDialog>
#include <QMessageBox>
#include "SplinesList.h"
#include "DialogWdgt.h"
#include "OZDialog.h"
#include <ShipModel.h>
#include <attr_product.h>
#include "curve.h"
#include "conv_model_exchange.h"
#include "assembly.h"
#include "plane_instance.h"
#include <iostream>
#include <string>
#include <filesystem>
#include <tool_cstring.h>
#include <cur_polyline3d.h>
#include "ShipModel.h"
#include <Vision3DView.h>
#include <QImage>
#include <QPalette>
#include <qcheckbox.h>
#include <CuttingPlaneSettingsWdgt.h>
#include <CuttingPlanesWdgt.h>
#include <CuttingTools.h>

//класс главного окна отвечает за управление другими окнами и управление моделью
class MainWnd : public QMainWindow
{
    Q_OBJECT

public:
    MainWnd(QWidget* parent = nullptr);

protected:

    // Настройка перед созданием окна.
    void preConstructSetup();

    // Обработка события изменения размера окна.
    void resizeEvent(QResizeEvent* event) override;

    // Открыть файл.
    void onOpenFile();

    // Сохранить файл.
    void onSaveFile();

    // Переключиться в 2D-режим.
    void SwitchTo2D();

    // Переключиться в 3D-режим с поверхностью.
    void SwitchTo3DWithSurface();

    // Переключиться в 3D-режим с телом.
    void SwitchTo3DWithSolid();

    // Загрузить пользовательский фоновый рисунок.
    void loadCustomBackgroundImage();

    // Загрузить заднюю сетку.
    void loadBackNet();

private:
    Ui::MainWindow m_ui;
    QPushButton* m_RemovingLayerButton;
    DrawingWdgt* m_DrawingWidget;
    QScrollArea* m_Scroller;
    QWidget* m_VisionContainer;
    SplinesList* m_SplinesListWidget;
    Vision3DView* m_visionPointer;
    PointsWdgt* m_pointsWidet;
    SectionCordsWdgt* m_SectionCordsWidget;
    QComboBox* m_InterpolationType;
    OZDialog* m_OZDialogWiget;
    QPushButton* m_IsDelitingPointMode;
    QPushButton* m_GridButton;
    CuttingPlaneSettingsWdgt* m_CuttingPlaneSettings;
    CuttingPlanesWdgt* m_CuttingPlanesList;
    ShipModel* m_ShipModel=new ShipModel();
 
    // Загрузить модель из файла.
    bool getModelFromFile(const c3d::path_string& filePath);

    // Сохранить модель в файл.
    bool saveModelToFile(ShipModel& model, const c3d::path_string& filePath);

private slots:

    // Изменить тип интерполяции.
    void changeInterpolationType(int x);

    // Сигнал для начала отрисовки.
    void signalToDraw();

    // Вставить копию предыдущего слоя.
    void insertCopyOfprevious();

    // Изменить режим сетки.
    void changeGridMod();

    // Установить слой как NULL.
    void pushNULL(size_t x);

    // Установить слой.
    void setLayer(std::shared_ptr<ShipSection> shipSection);

    // Удалить выбранный слой.
    void deleteChosenLayer();

    // Переместить точку модели.
    void MovePointOfModel(double x, double y, size_t indexOfPoint, size_t indexOfSection);

    // Установить новые координаты сечения.
    void SetNewSectionsCords(double, char);

    // Установить визуальный тип интерполяции.
    void SetVisualInterpolationType();

    // Вызвать диалог OZ.
    void CallOZDialog();

    // Получить координаты OZ.
    void GetOzCord();

    // Удалить точку.
    void deletePoint(int x);

    // Добавить точку.
    void addPoint(MbCartPoint point);

    // Обработать событие нажатия клавиш.
    void keyPressEvent(QKeyEvent* event) override;

    // Передать настройки секущей плоскости.
    void handOverCuttingPlaneSettings(int index);

signals:
    
    // Сигнал для разблокировки рисования.
    void unlockDrawingI();

    // Сигнал для изменения активного сечения.
    void changeActiveSection();

    // Сигнал для отрисовки переданного слоя.
    void drawSendedLayer(size_t);

    // Сигнал для передачи указателя на координаты активного сечения.
    void SendActiveSectionCordsPointer(MbCartPoint3D SectionCords, bool isNullLayer);

    // Сигнал предупреждающий о изменения модели.
    void modelChanged(ShipModel*);

    // Сигнал для передачи количества сечений.
    void giveCountOfSections(size_t);

    // Сигнал предупреждающий о удалении последнего слоя.
    void LastLayerDeliting();

    // Сигнал для уведомления о загрузке модели.
    void ModelLoaded();
    
};
#endif
