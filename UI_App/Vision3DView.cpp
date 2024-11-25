#include "Vision3DView.h"

typedef  SPtr<MbContour> PlaneContourSPtr;

void Vision3DView::addSceneSegment(GeometryRep* pShapeRep, const MbVector3D& vecMove, const Color& color)
{
        SceneSegment* pSegment = new SceneSegment(pShapeRep, this->sceneContent()->GetRootSegment());
        if (color.IsValid())
            pSegment->SetColorPresentationMaterial(color);
        pSegment->GetTransform().Move(vecMove);
        m_segmentKeys.push_back(  pSegment->GetUniqueKey());
}

Vision3DView::Vision3DView(QWindow* parent ) : QtOpenGLSceneWindow(parent),m_isVisible{false}, m_isSurface{true}, m_cuttingTools(new CuttingTools(this))
{
    Light* pLight = this->graphicsScene()->CreateExtraLight();
    pLight->SetType(Light::DirectionLight);
    pLight->SetPosition(-100.0, -1.0, -1.0);
    pLight->SetDiffuseColor(Color(0.2f, 0.2f, 0.2f, 1.0f));
    pLight->SetDoubleSided(false);
    this->viewport()->SetGradientBackgroundColour(QtVision::topColor, QtVision::bottomColor);
    SceneSegment* pTopSegment = this->sceneContent()->GetRootSegment();
    Q_ASSERT(pTopSegment != nullptr);
    PrCameraOrbit* pProcess = new PrCameraOrbit(this->graphicsScene(), this->graphicsEngine()->GetTopEssence());
    pProcess->SetMouseButtonsActivate({ VSN::mb_RButton });
    auto CamPos = this->camera()->GetPosition();
    this->camera()->SetPosition(CamPos);
    
}

void Vision3DView::synchronizeWithModel(ShipModel* model)
{
    this->sceneContent().get()->Clear();
    
    MbSurface* surface=CreateSurfaseByModel(model);
    if (surface!=nullptr)
    {
        SPtr<MbItem> item{ new MbSpaceInstance(*surface) };
        addSceneSegment(GeometryFactory::Instance()->CreateMathRep(item.get()), MbVector3D(), Color(255, 0, 0));
        this->requestUpdate();
    }
    
}

void Vision3DView::addSolidToScene(ShipModel* model)
{
    if (model->sectionsCount()>=2)
    {
        c3d::SolidSPtr solid = CreateSolidByModel(model);

        if (solid != nullptr)
        {
            this->sceneContent().get()->Clear();
            SPtr<MbItem> item{ solid };
            addSceneSegment(GeometryFactory::Instance()->CreateMathRep(item.get()), MbVector3D(), Color(255, 0, 0));
            viewport()->ZoomToFit(sceneContent()->GetBoundingBox());
            MbPlacement3D place;
            solid->GetPlacement(place);
            place.GetOrigin();
            viewport()->GetCamera()->SetPosition(MbCartPoint3D(place.GetOrigin().x, place.GetOrigin().y + 30, place.GetOrigin().z + 800));
            this->requestUpdate();
        }
    }
}
//rework
MbSurface* Vision3DView::CreateSurfaseByModel(ShipModel* model)
{
    if (model->sectionsCount() - model->countOfNullSections() >= 2)
    {
        SArray<MbPlacement3D> placesS = model->getAllPlasements();
        std::vector< std::pair<MbPlacement3D, MbCurve*>> CurvesPlacements;
      
        for (size_t i = 0; i < model->sectionsCount(); i++)
        {
            
            CurvesPlacements.push_back(std::pair<MbPlacement3D, MbCurve*>(placesS[i], model->getSection(i).get()->getCurve().get()));

        }
        std::sort(CurvesPlacements.begin(), CurvesPlacements.end(), [](std::pair<MbPlacement3D, MbCurve*>& lhs, std::pair<MbPlacement3D, MbCurve*>& rhs) {return lhs.first.GetOrigin().z < rhs.first.GetOrigin().z; });

        MbSurface* result = nullptr;
        RPArray< MbCurve3D > curves;
        for (size_t i = 0; i < CurvesPlacements.size(); i++)
        {
            if (!model->getSection(i).get()->isNullLayer())
            {

                curves.Add(MakeCurve3D(*(CurvesPlacements[i].second), CurvesPlacements[i].first));


                SPtr<MbItem> item{ new MbPlaneInstance(*(CurvesPlacements[i].second),CurvesPlacements[i].first)};
                addSceneSegment(GeometryFactory::Instance()->CreateMathRep(item.get()), MbVector3D(), Color(0, 255, 0));
            }

        }
        
        LoftedSurface(curves, false, MbVector3D(), MbVector3D(), result);
        return result;
    }
    else return nullptr;
}

c3d::SolidSPtr Vision3DView::CreateSolidByModel(ShipModel* model)
{  

    std::vector< std::pair<MbPlacement3D, MbCurve*>> CurvesPlacements;

    bool firstSpline = true;
    std::vector<MbPlacement3D>  placesOriginal;
    SArray<MbPlacement3D> placesS = model->getAllPlasements();

    std::vector<MbPlacement3D>  reflectedPlaces;
    std::vector<PlaneContourSPtr> fullContours;
    for (size_t i = 0; i < model->sectionsCount(); i++)
    {
        MbCartPoint startPoint;
        MbCartPoint endPoint;

        model->getSection(i).get()->getCurve().get()->GetStartPoint(startPoint);
        model->getSection(i).get()->getCurve().get()->GetEndPoint(endPoint);
        if (startPoint.x > endPoint.x)
            model->getSection(i).get()->getCurve().get()->Inverse();
        CurvesPlacements.push_back(std::pair<MbPlacement3D, MbCurve*>(placesS[i], model->getSection(i).get()->getCurve().get()));

    }
    std::sort(CurvesPlacements.begin(), CurvesPlacements.end(), [](std::pair<MbPlacement3D, MbCurve*>& lhs, std::pair<MbPlacement3D, MbCurve*>& rhs) {return lhs.first.GetOrigin().z < rhs.first.GetOrigin().z; });

    for (size_t i = 0; i < CurvesPlacements.size(); i++)
    {
        std::vector<MbCurve*> currentContur;
        placesOriginal.push_back(CurvesPlacements[i].first);
        MbMatrix3D matrixReflected;
        placesOriginal[i].Symmetry(matrixReflected);
        MbCurve* refCurve = CurvesPlacements[i].second->InverseDuplicate();
        MbPlacement place;
        place.InvertAxisX();
        MbMatrix matrix{ place };
        refCurve->Transform(matrix);
        currentContur.push_back(refCurve);

        MbCartPoint startPointRef;
        MbCartPoint endPoint;
        refCurve->GetStartPoint(startPointRef);
        CurvesPlacements[i].second->GetEndPoint(endPoint);
        endPoint.x -= 1.0;
        startPointRef.x += 1.0;
        MbCurve* conectionCurve = nullptr;
        ::Segment(endPoint, startPointRef, conectionCurve);

        conectionCurve->GetEndPoint(endPoint);
        conectionCurve->GetStartPoint(startPointRef);
        currentContur.push_back(conectionCurve);
        currentContur.push_back(CurvesPlacements[i].second);

        MbCartPoint startPoint;
        MbCartPoint endPointRef;
        CurvesPlacements[i].second->GetStartPoint(startPoint);
        refCurve->GetEndPoint(endPointRef);
        startPoint.x -= 1.0;
        endPointRef.x += 1.0;
        MbCurve* upperCurve = nullptr;
        ::Segment(endPointRef, startPoint, upperCurve);
        currentContur.push_back(upperCurve);
        SPtr<MbContour> contorPtr{ MakeContour(currentContur,2.f) };
        fullContours.push_back(contorPtr);
    }

    MbLoftedSolidParams params(placesOriginal, fullContours, nullptr, LoftedValues(), nullptr, nullptr, MbSNameMaker(), c3d::SNamesMakerSPtrVector());
    c3d::SolidSPtr solidSptr{ nullptr };
    auto res = LoftedSolid(params, solidSptr);
    if (res == rt_Success)
    {
        return solidSptr;
    }
    else if (res == rt_SelfIntersection)
    {
        QMessageBox::critical(nullptr, "ERROR", "SelfIntersection");
    }
    else if (res == rt_ParameterError)
    {
        QMessageBox::critical(nullptr, "ERROR", "ParameterError");
    }
    else
    {
        char errmasg[128];
        sprintf(errmasg, "GENERAL ERROR %d", int(res));
        QMessageBox::critical(nullptr, "critical error", errmasg);
    }

    return nullptr;
}

void Vision3DView::nowVisible()
{
    m_isVisible = true;
}

void Vision3DView::nowInvisible()
{
    m_isVisible = false;
}

void Vision3DView::displaySolid()
{
    m_isSurface = false;
}

void Vision3DView::displaySurface()
{
    m_isSurface = true;
}

CuttingTools* Vision3DView::GetCuttingTools()
{
    return m_cuttingTools;
}

void Vision3DView::setCuttingCount(size_t count)
{
    m_cuttingTools->setCount(count);
}

void Vision3DView::Refresh(ShipModel* model)
{
    if (m_isVisible && m_isSurface) 
    {
        synchronizeWithModel(model);
        this->renderNow();
        this->requestUpdate();
    }
    else if (m_isVisible && !m_isSurface)
    {

        addSolidToScene(model);
        this->renderNow();
        this->requestUpdate();
        this->focusObject();
    }
}





