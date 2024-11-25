#ifndef __CONTROLLER_H
#define __CONTROLLER_H

#include <QObject>

#include <vsn_prchangewidget.h>
#include <vsn_widgetmodels.h>
#include <vsn_graphicsscene.h>
#include <mb_cube.h>

namespace VSN {

class DefaultControllers
{
public:
    static const Widget3DModelPtr xyzController;
    static const Widget3DModelPtr directController;
};

class DefaultProcesses
{
public:
    static const WidgetProcessPtr spaceMove;
    static const WidgetProcessPtr xlocalMove;
    static const WidgetProcessPtr ylocalMove;
    static const WidgetProcessPtr zlocalMove;

    static const WidgetProcessPtr xylocalMove;
    static const WidgetProcessPtr xRotate;
    static const WidgetProcessPtr yRotate;
    static const WidgetProcessPtr zRotate;
};


//class SceneWidget;

class Controller : public  VSN::Object
{
    VSN_OBJECT(Controller);
public:
    Controller(GraphicsScene * pScene);
public:
    enum ControllerViews { NoneCtrl, xyzCtrl, directCtrl };
    ControllerViews getControllerViewsType();
    void SetSectionPlaneID(SectionPlaneId sectionPlaneID);
    void Show();
    void Hide();
    void SetControllerView(ControllerViews view);
    void UpdateControllerWidget();
public:
    VSN_SLOT(Public, slotModify, void slotModify(SceneWidget*));
    VSN_SIGNAL(Public, signalUpdate, void signalUpdate());
private:
    SceneWidget* m_xyzWidget = nullptr;
    SceneWidget* m_directionWidget = nullptr;
    ControllerViews currentControllerView = NoneCtrl;
    GraphicsScene* m_pScene = nullptr;
    SectionPlaneId m_curIdPlane; // current section plane id for edit
    MbMatrix3D GetControllerMatrix();
};

} // namespace VSN

#endif // __CONTROLLER_H
