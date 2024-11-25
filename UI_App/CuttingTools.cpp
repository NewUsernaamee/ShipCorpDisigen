#include "CuttingTools.h"

size_t CuttingTools::count() const
{
   return m_CountOfCutingPlanes;
}

void CuttingTools::setCount(size_t count)
{
    m_CountOfCutingPlanes = count;
    CreateCutingTools();
}

CuttingTools::CuttingTools(QtVision::QtOpenGLSceneWindow* parent):parent(parent), m_controller(new Controller(parent->graphicsScene()))
{
    m_controller->SetControllerView(VSN::Controller::NoneCtrl);
}

bool CuttingTools::IsInverted()
{
    if (m_currentCuttingTool < m_planes.size())
    return parent->graphicsScene()->GetCuttingTool()->IsInverted(m_planes[m_currentCuttingTool]);
    return false;
}

bool CuttingTools::IsEnable()
{
    if (m_currentCuttingTool< m_planes.size())
    return parent->graphicsScene()->GetCuttingTool()->IsEnabled(m_planes[m_currentCuttingTool]);
    return false;
}

VSN::Controller::ControllerViews CuttingTools::ControllerType()
{
    if (m_currentCuttingTool < m_planes.size())
    return m_controller->getControllerViewsType();
    return VSN::Controller::NoneCtrl;
}

void CuttingTools::ChengeNavigationOfCuttingTool()
{
    if (m_controller->getControllerViewsType() == VSN::Controller::directCtrl)
    {
        m_controller->SetControllerView(VSN::Controller::xyzCtrl);
    }
    else
    {
        m_controller->SetControllerView(VSN::Controller::directCtrl);
    }
    parent->requestUpdate();
}
void CuttingTools::ChangeCuttingDirection()
{
    auto isInverted = parent->graphicsScene()->GetCuttingTool()->IsInverted(m_planes[m_currentCuttingTool]);
    parent->graphicsScene()->GetCuttingTool()->Invert(m_planes[m_currentCuttingTool], !isInverted);
    parent->requestUpdate();
}

void CuttingTools::ChangeEnableOfCuttingTool()
{
    bool isEnable = parent->graphicsScene()->GetCuttingTool()->IsEnabled(m_planes[m_currentCuttingTool]);
    parent->graphicsScene()->GetCuttingTool()->SetEnable(m_planes[m_currentCuttingTool], !isEnable);
    parent->requestUpdate();
}
void CuttingTools::changeCurrentCuttingToolIndex(int index)
{
    if (index >= 0 && m_planes.size()>index)
    {
        auto tool = parent->graphicsScene()->GetCuttingTool();
        tool->EnableInteractiveMode(m_planes[m_currentCuttingTool], false);

        m_currentCuttingTool = index;
        tool->EnableInteractiveMode(m_planes[m_currentCuttingTool], true);
        m_controller->SetSectionPlaneID(m_planes[m_currentCuttingTool]);
    }
    m_controller->UpdateControllerWidget();
}


void CuttingTools::CreateCutingTools()
{
    if (m_CountOfCutingPlanes > 0)
    {
        while (true)
        {

            if (m_planes.size() > count())
            {
                parent->graphicsScene()->GetCuttingTool()->DeleteSectionPlane(m_planes.back());
                m_planes.pop_back();
            }
            else if (m_planes.size() < count())
            {
                SectionPlaneId m_curIdPlane;
                std::shared_ptr<Material> m_material;
                m_material.reset(new Material());
                auto tool = parent->graphicsScene()->GetCuttingTool();
                auto id = tool->AddSectionPlane(MbPlacement3D(MbCartPoint3D(-30, -30, -30), MbVector::xAxis));
                m_planes.emplace_back(id);
                tool->SetEnable(id, false);
                m_curIdPlane = m_planes[m_currentCuttingTool];
                tool->SetSurfaceMaterial(m_curIdPlane, m_material);
                tool->EnableInteractiveMode(m_curIdPlane, true);
                m_controller->SetSectionPlaneID(m_curIdPlane);
                m_controller->SetControllerView(Controller::ControllerViews::directCtrl);
                Object::Connect(m_controller, &Controller::signalUpdate, parent, &QtVision::QtOpenGLSceneWindow::requestUpdate);
            }
            else
            {
                parent->graphicsScene()->GetCuttingTool()->SetEnable(m_planes[m_currentCuttingTool], true);
                return;
            }


        }
    
    
     }
}
