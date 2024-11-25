#include "controller.h"

#include <math.h>
#include <cur_line3d.h>
#include <mb_axis3d.h>

#include <vsn_coordlocator.h>
#include <vsn_prchangewidget.h>
#include <vsn_widgetmodels.h>
#include <surf_plane.h>
#include <vsn_widgets.h>
#include <vsn_placement.h>
#include <last.h>

enum IdProcess { IdNone, IdZlocalMove, IdXRotate, IdYRotate };

using namespace VSN;

Widget3DModelPtr createBaseModel()
{
    Widget3DModelPtr model(new Widget3DModel());

    float l = 40.0f;
    float lc = l / 8.0f;
    float lb = l * 1.1f;
    float ld = l * 0.4f;
    float lw = l / 14.3f;
    float la = l / 6.5f;

    model->AddCylinder(IdZlocalMove, Point3DF(), Point3DF(0, 0, lb ), lw, Color(20, 140, 200));
    model->AddCone(IdZlocalMove, Point3DF(0, 0, lb ), Point3DF(0, 0, lb + ld ), la, Color(20, 140, 200));
    model->AddCube(IdZlocalMove, Point3DF(-lc, -lc ,-lc ), Point3DF(lc, lc, lc ), Color(220, 220, 220));

    model->AddTorus(IdXRotate, Point3DF(), Point3DF( l, 0, 0 ), lw, Color(140, 200, 20), 0.3f, (float)(M_PI / 2.0 - 0.3));
    model->AddCone(IdXRotate, Point3DF (0, l - lw, ld ), Point3DF( 0, l, 0 ), lc, Color(140, 200, 20));
    model->AddCone(IdXRotate, Point3DF( 0, ld, l - lw ), Point3DF( 0, 0, l ), lc, Color(140, 200, 20));
    model->AddDisk(IdNone, Color(220, 220, 220),  Placement3DF(Point3DF( 0, l, 0 ),Vector3DF::zAxis), lc, 0.5);

    model->AddTorus(IdYRotate, Point3DF(), Point3DF( 0, l, 0 ), lw, Color(200, 20, 140), - 0.3f, (float)( - M_PI / 2.0 + 0.3));
    model->AddCone(IdYRotate, Point3DF( l - lw, 0, ld ), Point3DF( l, 0, 0 ), lc, Color(200, 20, 140));
    model->AddCone(IdYRotate, Point3DF( ld, 0, l - lw ), Point3DF( 0, 0, l ), lc, Color(200, 20, 140));
    model->AddDisk(IdNone, Color(220, 220, 220), Placement3DF(Point3DF(l, 0, 0 ),Vector3DF::zAxis), lc, 0.5);

    return model;
}

Widget3DModelPtr createDirectModel()
{
    Widget3DModelPtr model(new Widget3DModel());
    float l = 40.0f;
    float ln = l * 1.4f;
    float lc = l / 8.0f;
    float lw = l / 14.3f;
    float la = l / 6.5f;
    model->AddCylinder(IdZlocalMove, Point3DF(), Point3DF( 0, 0, l ), lw, Color(20, 140, 200));
    model->AddCone(IdZlocalMove, Point3DF( 0, 0, l ), Point3DF( 0, 0, ln ), la, Color(20, 140, 200));
    model->AddCube(IdZlocalMove, Point3DF( -lc, -lc, -lc ), Point3DF( lc, lc, lc ), Color(220, 220, 220));
    return model;
}

WidgetProcessPtr createSpaceMoveProcess()
{
    std::shared_ptr<MoveWidgetProcess> process(new MoveWidgetProcess(nullptr));
    process->SetLocator(new ModelCoordLocator(process.get()));
    return process;
}

WidgetProcessPtr createCurveMoveProcess(SPtr<const MbCurve3D> curve, bool local = true)
{
    std::shared_ptr<MoveWidgetProcess> process(new MoveWidgetProcess(nullptr));
    process->SetLocator(new CurveScreenCoordLocator(curve, process.get()));
    process->SetConvertPlacement(local ? WidgetConvertPlacements::Widget : WidgetConvertPlacements::Scene);

    return process;
}

WidgetProcessPtr createSurfaceMoveProcess(SPtr<const MbSurface> surface, bool local = true)
{
    std::shared_ptr<MoveWidgetProcess> process(new MoveWidgetProcess(nullptr));
    process->SetLocator(new SurfaceScreenCoordLocator(surface, process.get()));
    process->SetConvertPlacement(local ? WidgetConvertPlacements::Widget : WidgetConvertPlacements::Scene);
    return process;
}

WidgetProcessPtr createRotateProcess(const MbAxis3D& axis)
{
    std::shared_ptr<RotateWidgetProcess> process(new RotateWidgetProcess(axis));
    return process;
}

Controller::Controller(GraphicsScene * pScene)
    : m_pScene(pScene)
{
    // creation of a three - axis controller and binding to it the processes of movement and rotation
    m_xyzWidget = new SceneWidget(pScene, DefaultControllers::xyzController);
    m_xyzWidget->AssignProcess(IdZlocalMove, DefaultProcesses::zlocalMove.get());
    m_xyzWidget->AssignProcess(IdXRotate, DefaultProcesses::xRotate.get());
    m_xyzWidget->AssignProcess(IdYRotate, DefaultProcesses::yRotate.get());
    m_xyzWidget->SetCustomKey(NodeKey(0));

    Object::Connect(m_xyzWidget, &SceneWidget::OnModified, this, &Controller::slotModify);

    // creation of a axial - arrow controller and binding to it the processes of movement by z - axis
    m_directionWidget = new SceneWidget(pScene, DefaultControllers::directController);
    m_directionWidget->AssignProcess(IdZlocalMove, DefaultProcesses::zlocalMove.get());
    m_directionWidget->SetCustomKey(NodeKey(0));

    Object::Connect(m_directionWidget, &SceneWidget::OnModified, this, &Controller::slotModify);
}

VSN::Controller::ControllerViews VSN::Controller::getControllerViewsType()
{
    return this->currentControllerView;
}

void Controller::SetSectionPlaneID(SectionPlaneId sectionPlaneID)
{
    m_curIdPlane = sectionPlaneID;
    UpdateControllerWidget();
}

void Controller::Show()
{
    switch (currentControllerView)
    {
    case Controller::NoneCtrl:
        m_xyzWidget->SetVisible(false);
        m_directionWidget->SetVisible(false);
        break;
    case Controller::xyzCtrl:
        m_xyzWidget->SetVisible(true);
        m_directionWidget->SetVisible(false);
        break;
    case Controller::directCtrl:
        m_xyzWidget->SetVisible(false);
        m_directionWidget->SetVisible(true);
        break;
    }
}

void Controller::Hide()
{
    m_xyzWidget->SetVisible(false);
    m_directionWidget->SetVisible(false);
}

void Controller::SetControllerView(ControllerViews view)
{
    currentControllerView = view;
    UpdateControllerWidget();
    Show();
    emit signalUpdate();
}

void Controller::slotModify(SceneWidget* item)
{
    if (currentControllerView != NoneCtrl)
    {       
        //changing the position of the section plane by changing the position of the controller
        MbPlacement3D placement(item->GetTransform());
        m_pScene->GetCuttingTool()->Init(m_curIdPlane, placement);
    }
}

void Controller::UpdateControllerWidget()
{
    switch (currentControllerView)
    {
    case Controller::NoneCtrl:
        break;
    case Controller::xyzCtrl:
        m_xyzWidget->Init(GetControllerMatrix());
        break;
    case Controller::directCtrl:     
        m_directionWidget->Init(GetControllerMatrix());
        break;
    }
}

MbMatrix3D Controller::GetControllerMatrix()
{
    //getting the center of the section plane for controller position
    auto tool = m_pScene->GetCuttingTool();
    auto & placement = tool->GetPlacement(m_curIdPlane);
    MbCube cube = m_pScene->GetGlobalBoundingBox();
    MbRect rect;
    cube.ProjectionRect(placement, rect);
    MbCartPoint center;
    rect.GetCenter(center);
    MbCartPoint3D center3D;
    placement.PointOn(center, center3D);
    MbVector3D controllerVector = placement.Normal();
    if (!tool->IsInverted(m_curIdPlane))
        controllerVector.Invert();
    MbPlacement3D normalCenter3D(center3D, controllerVector);
    MbMatrix3D controllerMatrix = normalCenter3D.GetMatrixFrom();

    return controllerMatrix;
}

const Widget3DModelPtr DefaultControllers::xyzController = createBaseModel();
const Widget3DModelPtr DefaultControllers::directController = createDirectModel();

const WidgetProcessPtr DefaultProcesses::spaceMove = createSpaceMoveProcess();
const WidgetProcessPtr DefaultProcesses::xlocalMove = createCurveMoveProcess(SPtr<MbCurve3D>(new MbLine3D({}, MbCartPoint3D{ 1,0,0 })));
const WidgetProcessPtr DefaultProcesses::ylocalMove = createCurveMoveProcess(SPtr<MbCurve3D>(new MbLine3D({}, MbCartPoint3D{ 0,1,0 })));
const WidgetProcessPtr DefaultProcesses::zlocalMove = createCurveMoveProcess(SPtr<MbCurve3D>(new MbLine3D({}, MbCartPoint3D{ 0,0,1 })));

const WidgetProcessPtr DefaultProcesses::xylocalMove = createSurfaceMoveProcess(SPtr<MbSurface>(new MbPlane()));
const WidgetProcessPtr DefaultProcesses::xRotate = createRotateProcess(MbAxis3D::xAxis);
const WidgetProcessPtr DefaultProcesses::yRotate = createRotateProcess(MbAxis3D::yAxis);
//const WidgetProcessPtr DefaultProcesses::zRotate = createRotateProcess(MbAxis3D::zAxis);