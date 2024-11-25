#include "MainWnd.h"

#define RowMinusOne m_SplinesListWidget->currentRow() - 1


const QRect m_defaultSplinesListSize(650, 0, 250, 140);
const int drawingWidth = 188;
const int drawingHeight = 10;
QImage CurrentBackGroundImage;
bool isBasicBackground=true;
bool isIn2D=true;

static SPtr<MbItem> CreateC3DSection(MbCurve& section) {
    return SPtr<MbItem>{ new MbPlaneInstance(MbPlacement3D::global, section) };
}


void MainWnd::preConstructSetup() {

    m_InterpolationType->addItem("NURBS");
    m_InterpolationType->addItem("Hermit");
    m_InterpolationType->addItem("CubicSpline");
    m_InterpolationType->addItem("Polyline");
    m_SplinesListWidget->setGeometry(m_defaultSplinesListSize);
    m_Scroller->setWidget(m_DrawingWidget);
    m_CuttingPlaneSettings->hide();
    m_CuttingPlanesList->hide();
    connect(m_ui.save, &QAction::triggered, this, &MainWnd::onSaveFile);
    connect(m_ui.open, &QAction::triggered, this, &MainWnd::onOpenFile);
    connect(m_ui.action_2D, &QAction::triggered, this, &MainWnd::SwitchTo2D);
    connect(m_ui.action_3D, &QAction::triggered, this, &MainWnd::SwitchTo3DWithSurface);
    connect(m_ui.action_3D_2, &QAction::triggered, this, &MainWnd::SwitchTo3DWithSolid);
    connect(m_ui.loadcustoBack, &QAction::triggered, this, &MainWnd::loadCustomBackgroundImage);
    connect(m_ui.loadBackground, &QAction::triggered, this, &MainWnd::loadBackNet);
    connect(m_ui.loadBackground, &QAction::triggered, this, &MainWnd::loadBackNet);
    QObject::connect(this, &MainWnd::drawSendedLayer, this->m_DrawingWidget, &DrawingWdgt::drawChosenLayer);
    QObject::connect(this->m_SplinesListWidget, &SplinesList::currentRowChanged, this, &MainWnd::signalToDraw);
    QObject::connect(this->m_DrawingWidget, &DrawingWdgt::PushLayer, this, &MainWnd::setLayer);
    QObject::connect(this->m_SplinesListWidget, &SplinesList::itemClicked, this->m_DrawingWidget, &DrawingWdgt::unlockDrawing);
    QObject::connect(this->m_SplinesListWidget, &SplinesList::pushNULL, this, &MainWnd::pushNULL);
    QObject::connect(this, &MainWnd::modelChanged, this->m_DrawingWidget, &DrawingWdgt::changeActiveModel);
    QObject::connect(this, &MainWnd::giveCountOfSections, this->m_DrawingWidget, &DrawingWdgt::loadedDraw);
    QObject::connect(this->m_RemovingLayerButton, &QPushButton::clicked, this, &MainWnd::deleteChosenLayer);
    QObject::connect(this, &MainWnd::LastLayerDeliting, this->m_DrawingWidget, &DrawingWdgt::lockDrawing);
    QObject::connect(this->m_DrawingWidget, &DrawingWdgt::MoveModelPoint, this, &MainWnd::MovePointOfModel);
    QObject::connect(this, &MainWnd::modelChanged, this->m_pointsWidet, &PointsWdgt::SynchronizeWithModel);
    QObject::connect(this, &MainWnd::SendActiveSectionCordsPointer, this->m_SectionCordsWidget, &SectionCordsWdgt::SynchronizeSectionCords);
    QObject::connect(this->m_SectionCordsWidget, &SectionCordsWdgt::SendCords, this, &MainWnd::SetNewSectionsCords);
    QObject::connect(this->m_InterpolationType, &QComboBox::currentIndexChanged, this, &MainWnd::changeInterpolationType);
    QObject::connect(this->m_SplinesListWidget, &SplinesList::itemClicked, this, &MainWnd::SetVisualInterpolationType);
    QObject::connect(this, &MainWnd::ModelLoaded, this->m_DrawingWidget, &DrawingWdgt::LoadedModel);
    QObject::connect(this->m_SplinesListWidget, &SplinesList::itemClicked, this->m_DrawingWidget, &DrawingWdgt::UnlockModelLoaded);
    QObject::connect(this, &MainWnd::modelChanged, this->m_visionPointer, &Vision3DView::Refresh);
    QObject::connect(this->m_DrawingWidget, &DrawingWdgt::EnterOZCord, this, &MainWnd::CallOZDialog);
    QObject::connect(this->m_OZDialogWiget, &OZDialog::TakeOzCord, this, &MainWnd::GetOzCord);
    QObject::connect(this->m_SplinesListWidget, &SplinesList::currentRowChanged, this->m_DrawingWidget, &DrawingWdgt::setActiveLayerIndex);
    QObject::connect(this->m_IsDelitingPointMode, &QPushButton::clicked, this->m_DrawingWidget, &DrawingWdgt::isDelitingMod);
    QObject::connect(this->m_DrawingWidget, &DrawingWdgt::deletePoint, this, &MainWnd::deletePoint);
    QObject::connect(this->m_DrawingWidget, &DrawingWdgt::addPoint, this, &MainWnd::addPoint);
    QObject::connect(this->m_GridButton, &QPushButton::clicked, this, &MainWnd::changeGridMod);
    QObject::connect(this->m_SplinesListWidget, &SplinesList::insertCopyOfprevious, this, &MainWnd::insertCopyOfprevious);
    QObject::connect(this->m_CuttingPlaneSettings, &CuttingPlaneSettingsWdgt::SendCuttingLayerCount, this->m_CuttingPlanesList, &CuttingPlanesWdgt::refil);
    QObject::connect(this->m_CuttingPlanesList, &CuttingPlanesWdgt::currentRowChanged, this->m_visionPointer->GetCuttingTools(), &CuttingTools::changeCurrentCuttingToolIndex);
    QObject::connect(this->m_CuttingPlaneSettings, &CuttingPlaneSettingsWdgt::SendCuttingLayerCount, this->m_visionPointer, &Vision3DView::setCuttingCount);
    QObject::connect(this->m_CuttingPlaneSettings, &CuttingPlaneSettingsWdgt::DirectionChanged, this->m_visionPointer->GetCuttingTools(), &CuttingTools::ChangeCuttingDirection);
    QObject::connect(this->m_CuttingPlaneSettings, &CuttingPlaneSettingsWdgt::NavigationTypeChanged, this->m_visionPointer->GetCuttingTools(), &CuttingTools::ChengeNavigationOfCuttingTool);
    QObject::connect(this->m_CuttingPlaneSettings, &CuttingPlaneSettingsWdgt::EnableChanged, this->m_visionPointer->GetCuttingTools(), &CuttingTools::ChangeEnableOfCuttingTool);
    QObject::connect(this->m_CuttingPlanesList, &CuttingPlanesWdgt::currentRowChanged, this, &MainWnd::handOverCuttingPlaneSettings);
}


MainWnd::MainWnd(QWidget* parent) :
    QMainWindow(parent),
    m_SplinesListWidget(new SplinesList(this)),
    m_DrawingWidget(new DrawingWdgt(this, 250, 250)), 
    m_RemovingLayerButton(new QPushButton("REMOVE", this)),
    m_pointsWidet(new PointsWdgt(this)),
    m_OZDialogWiget(new OZDialog (this)),
    m_visionPointer(new Vision3DView()),
    m_SectionCordsWidget(new SectionCordsWdgt(this)),
    m_InterpolationType(new QComboBox(this)),
    m_IsDelitingPointMode(new QPushButton("Delete Point mod", this)),
    m_GridButton(new QPushButton("Grid OFF", this)),
    m_Scroller(new QScrollArea()),
    m_CuttingPlaneSettings(new CuttingPlaneSettingsWdgt(this)),
    m_CuttingPlanesList(new CuttingPlanesWdgt(this))
{
    m_ui.setupUi(this);
    setWindowIcon(QIcon(":/new/prefix1/Icons/icon.ico"));
    m_VisionContainer = QWidget::createWindowContainer(m_visionPointer,this);
    m_VisionContainer->hide();
    m_VisionContainer->move(m_DrawingWidget->pos().x(), m_VisionContainer->pos().y()+20);
    preConstructSetup();
    setCentralWidget(m_Scroller);
    std::cout << "MainWnd.size() = " << this->size().width() << "x" << this->size().height() << std::endl;
    setFocus();
    loadBackNet();
    resizeEvent(nullptr);
}  

void MainWnd::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        m_DrawingWidget->SetLayer();
        QWidget::keyPressEvent(event);
    }
    else if(event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Delete)
    {
        deleteChosenLayer();
    }
    else if (event->key() == Qt::Key_N)
    {
        m_SplinesListWidget->setCurrentRow(0);
        m_SplinesListWidget->callDialogWdgt();
        m_DrawingWidget->unlockDrawing();
    }


}
void MainWnd::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
    m_VisionContainer->setFixedSize(this->size().width() - drawingWidth, this->size().height() - drawingHeight);
   m_DrawingWidget->setFixedSize(this->size().width() - drawingWidth, this->size().height() - drawingHeight);
   m_Scroller->setFixedSize(m_DrawingWidget->size());
   m_SplinesListWidget->setFixedSize((this->size().width() - m_DrawingWidget->size().width()), 200);
   m_pointsWidet->setFixedSize((this->size().width() - m_DrawingWidget->size().width())+1,  300);
   m_SectionCordsWidget->setFixedSize(m_pointsWidet->width(),150);
   m_InterpolationType->setFixedSize(m_RemovingLayerButton->width()-5, 30);
   m_GridButton->setFixedSize(m_RemovingLayerButton->width() - 5, 30);
   m_CuttingPlaneSettings->setFixedSize(m_SectionCordsWidget->size());
   m_CuttingPlanesList->setFixedSize(m_SplinesListWidget->size());
   
   m_VisionContainer->move(m_DrawingWidget->pos().x(), m_VisionContainer->pos().y());
   m_SplinesListWidget->move(m_DrawingWidget->size().width(), 20);
   m_RemovingLayerButton->move(m_SplinesListWidget->pos().x(), m_SplinesListWidget->pos().y() + m_SplinesListWidget->size().height());
   m_pointsWidet->move(m_RemovingLayerButton->pos().x(), m_RemovingLayerButton->pos().y() + m_RemovingLayerButton->height()+180);
   m_SectionCordsWidget->move(m_RemovingLayerButton->pos().x(), m_RemovingLayerButton->pos().y()+ m_RemovingLayerButton->height()+50);
   m_InterpolationType->move(m_RemovingLayerButton->pos().x() + m_RemovingLayerButton->width() + 5, m_RemovingLayerButton->y());
   m_IsDelitingPointMode->move(m_SplinesListWidget->pos().x(), m_RemovingLayerButton->pos().y() + m_RemovingLayerButton->height() + 5);
   m_GridButton->move(m_IsDelitingPointMode->pos().x() + m_IsDelitingPointMode->width() + 5,m_IsDelitingPointMode->pos().y());
   m_CuttingPlaneSettings->move(m_SectionCordsWidget->pos().x()+5, m_SectionCordsWidget->pos().y()-100);
   m_CuttingPlanesList->move(m_SplinesListWidget->pos());
   if ( !isBasicBackground)
   {
    QImage image = CurrentBackGroundImage.scaled(this->m_DrawingWidget->width(), this->m_DrawingWidget->height());//Qt::KeepAspectRatio
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(image));
    m_Scroller->setPalette(palette);
   }
}

void MainWnd::changeInterpolationType(int type)
{
    if (this->m_SplinesListWidget->currentRow()!=0 && this->m_SplinesListWidget->currentRow()+1 != this->m_SplinesListWidget->count() && m_ShipModel->sectionsCount()>0){
    switch (type)
    {
    case 0:
        this->m_ShipModel->getSection(RowMinusOne).get()->ChangeInterpolationType(pt_Nurbs);
        break;
    case 1:
        this->m_ShipModel->getSection(RowMinusOne).get()->ChangeInterpolationType(pt_Hermit);
        break;
    case 2:
        this->m_ShipModel->getSection(RowMinusOne).get()->ChangeInterpolationType(pt_CubicSpline);
        break;
    case 3:
        this->m_ShipModel->getSection(RowMinusOne).get()->ChangeInterpolationType(pt_Polyline);
        break;
    }
     emit modelChanged(this->m_ShipModel);
    }
}

void MainWnd::SwitchTo2D()
{
    m_Scroller->show();
    m_DrawingWidget->show();
    m_VisionContainer->hide();
    m_visionPointer->nowInvisible();
    m_IsDelitingPointMode->show();
    m_RemovingLayerButton->show();
    m_SectionCordsWidget->show();
    m_InterpolationType->show();
    m_GridButton->show();
    m_CuttingPlaneSettings->hide();
    m_CuttingPlanesList->hide();
    isIn2D = true;
}

void MainWnd::SwitchTo3DWithSurface()
{
    if (isIn2D)
    { 
        m_visionPointer->synchronizeWithModel(this->m_ShipModel);
        m_Scroller->hide();
        m_DrawingWidget->hide();
        m_IsDelitingPointMode->hide();
        m_RemovingLayerButton->hide();
        m_InterpolationType->hide();
        m_SectionCordsWidget->hide();
        m_GridButton->hide();
        m_VisionContainer->show();
        m_visionPointer->nowVisible();
        m_visionPointer->displaySurface();
        m_CuttingPlaneSettings->show();
        m_CuttingPlanesList->show();
    }
    else
    { 
        m_visionPointer->synchronizeWithModel(this->m_ShipModel);
        m_visionPointer->nowVisible();
        m_visionPointer->displaySurface();
    }
    isIn2D = false;
}

void MainWnd::SwitchTo3DWithSolid()
{
    if (isIn2D)
    {
        m_visionPointer->addSolidToScene(this->m_ShipModel);
        m_Scroller->hide();
        m_DrawingWidget->hide();
        m_IsDelitingPointMode->hide();
        m_RemovingLayerButton->hide();
        m_InterpolationType->hide();
        m_GridButton->hide();
        m_SectionCordsWidget->hide();
        m_VisionContainer->show();
        m_visionPointer->nowVisible();
        m_visionPointer->displaySolid();
        m_CuttingPlaneSettings->show();
        m_CuttingPlanesList->show();
    }
    else
    {
        m_visionPointer->addSolidToScene(this->m_ShipModel);
        m_visionPointer->nowVisible();
        m_visionPointer->displaySolid();

    }
    isIn2D = false;
}


void MainWnd::loadCustomBackgroundImage()
{
   
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open img"), "", tr("*.png"));
    if (fileName.isEmpty())
        return;
    else {
        QPalette palet;
        QImage image(fileName);
        CurrentBackGroundImage = image;
        image = image.scaled(this->m_DrawingWidget->width(), this->m_DrawingWidget->height());
        QPalette palette;
        palette.setBrush(QPalette::Window, QBrush(image));
        m_Scroller->setPalette(palette);
        isBasicBackground = false;
    }
}

void MainWnd::loadBackNet()
{
    m_Scroller->setPalette(QPalette());
    isBasicBackground = true;

}

void MainWnd::changeGridMod()
{
    if (m_GridButton->text() == "Grid OFF")
    {
        m_GridButton->setText("Grid ON");
        m_DrawingWidget->changeGridMod(true);

    }
    else {
        m_GridButton->setText("Grid OFF");
        m_DrawingWidget->changeGridMod(false);
    }
}

void MainWnd::pushNULL(size_t x)
{   
    SArray<MbCartPoint> saray;
    saray.Add(MbCartPoint(1,1));
    saray.Add(MbCartPoint(1, 0));
    saray.Add(MbCartPoint(0, 1));
    auto sar = &saray;
    auto SharedPointer = ShipSection::createCubicSpline(sar);
    SharedPointer.get()->setNullLayer();
    if (m_ShipModel->sectionsCount()>=x)
    {
        m_ShipModel->insertSection(x, SharedPointer);
    }
    else
    {
        m_ShipModel->pushSection(SharedPointer);
    }
    emit modelChanged(m_ShipModel);
}

void MainWnd::setLayer(std::shared_ptr<ShipSection> shipSection)
{
    m_ShipModel->insertSection(RowMinusOne, shipSection);
    m_ShipModel->deleteNull(this->m_SplinesListWidget->currentRow());
    emit modelChanged(m_ShipModel);
    SetVisualInterpolationType();
}

void MainWnd::deleteChosenLayer()
{
    if( this->m_SplinesListWidget->currentRow()!=0 && (this->m_SplinesListWidget->currentRow()+1)!= this->m_SplinesListWidget->count())
    {
        if (this->m_ShipModel->sectionsCount() == 1)
        {
            emit LastLayerDeliting();
            this->m_ShipModel->eraseSection(0);
            this->m_SplinesListWidget->removeSpline();
            this->m_DrawingWidget->clearMutablePoints();
        }
        else if (this->m_ShipModel->sectionsCount() != 0)
        {

            this->m_ShipModel->eraseSection(RowMinusOne);
            
            this->m_SplinesListWidget->removeSpline();
            
            this->m_DrawingWidget->clearMutablePoints();
        }
        else { return; }
            
        if (this->m_ShipModel->sectionsCount() != 0)
            emit modelChanged(m_ShipModel);
    }
}

void MainWnd::MovePointOfModel(double x, double y, size_t indexOfPoint, size_t indexOfSection)
{
    m_ShipModel->getSection(indexOfSection).get()->movePoint(indexOfPoint,x,y);
    emit modelChanged(m_ShipModel);
}

void MainWnd::SetNewSectionsCords(double cord, char CordName)
{
    switch (CordName)
    {
    case 'x':
        this->m_ShipModel->getSection(RowMinusOne).get()->setSplineX(cord);
        emit modelChanged(this->m_ShipModel);
        break;
    case 'y':
        this->m_ShipModel->getSection(RowMinusOne).get()->setSplineY(cord);
        emit modelChanged(this->m_ShipModel);
        break;
    case 'z':
        this->m_ShipModel->getSection(RowMinusOne).get()->setSplineZ(cord);
        emit modelChanged(this->m_ShipModel);
        break;
    }
}

void MainWnd::SetVisualInterpolationType()
{
    if (m_SplinesListWidget->currentRow() != 0 && m_SplinesListWidget->currentRow() != m_SplinesListWidget->count() - 1)
    {
        MbePlaneType InterpolationType = this->m_ShipModel->getSection(RowMinusOne).get()->GetInterpolationType();

        switch (InterpolationType) 
        {
        case(MbePlaneType::pt_Polyline):
            this->m_InterpolationType->setCurrentIndex(3);
            break;

        case(MbePlaneType::pt_CubicSpline):
            this->m_InterpolationType->setCurrentIndex(2);
            break;

        case(MbePlaneType::pt_Hermit):
            this->m_InterpolationType->setCurrentIndex(1);
            break;

        case(MbePlaneType::pt_Nurbs):
            this->m_InterpolationType->setCurrentIndex(0);
            break;
        }

    }
}

void MainWnd::CallOZDialog()
{
    
    m_OZDialogWiget->show();
    
}

void MainWnd::GetOzCord()
{   
    double GottedZCord = this->m_OZDialogWiget->getOzCords();
   
    
    if(abs(GottedZCord) <0.01)
    {
        double ZCord = 0.0;
        std::vector <double> ZCords;
        for (size_t i = 0; i < m_ShipModel->sectionsCount(); i++)
        {
            if (i+1 != m_SplinesListWidget->currentRow())
                ZCords.push_back(this->m_ShipModel->getSection(i).get()->GetPlacement().GetOrigin().z);

        }

        bool flag = true;
        while (flag)
        {
            auto iterator = std::find(ZCords.begin(), ZCords.end(), ZCord);


            if (iterator != ZCords.end())
            {
                ZCord += 20;
                continue;

            }
            else
            {
                flag = false;
                m_ShipModel->getSection(RowMinusOne).get()->setSplineZ(ZCord);
            }
        }
    }
    else 
        this->m_ShipModel->getSection(RowMinusOne).get()->setSplineZ(GottedZCord);

    this->m_SectionCordsWidget->setZCord(this->m_ShipModel->getSection(RowMinusOne).get()->getSplineCord().z);



}

void MainWnd::deletePoint(int x)
{
    m_ShipModel->getSection(RowMinusOne).get()->deletePoint(x);
    emit modelChanged(m_ShipModel);
}

void MainWnd::addPoint(MbCartPoint point)
{
    m_ShipModel->getSection(RowMinusOne).get()->addPoint(point);
    emit modelChanged(m_ShipModel);
    qDebug("adding point");
}

void MainWnd::handOverCuttingPlaneSettings(int index)
{
    this->m_CuttingPlaneSettings->getActiveCutterSettings(m_visionPointer->GetCuttingTools()->IsInverted(), m_visionPointer->GetCuttingTools()->IsEnable(), m_visionPointer->GetCuttingTools()->ControllerType());
}

void MainWnd::signalToDraw()
{
    if (m_SplinesListWidget->currentRow()>0 && m_SplinesListWidget->currentRow() != m_SplinesListWidget->count()-1)
    {
        emit drawSendedLayer(RowMinusOne);
        emit SendActiveSectionCordsPointer((this->m_ShipModel->getSection(RowMinusOne).get()->getSplineCord()), this->m_ShipModel->getSection(RowMinusOne).get()->isNullLayer());
    }
}

void MainWnd::insertCopyOfprevious()
{
    if (!m_ShipModel->empty())
    {

        if (m_SplinesListWidget->currentRow() == 0) {
            auto x = m_SplinesListWidget->currentRow();
            std::shared_ptr<ShipSection> newSection = ShipSection::createSectionByCurve(m_ShipModel->getSection(m_SplinesListWidget->currentRow())->getCurve());
            m_ShipModel->insertSection(this->m_SplinesListWidget->currentRow(), newSection);
            emit modelChanged(this->m_ShipModel);
            SetVisualInterpolationType();
            CallOZDialog();
        }
        else
        {
            std::shared_ptr<ShipSection> newSection = ShipSection::createSectionByCurve(m_ShipModel->getSection(m_SplinesListWidget->currentRow() - 3)->getCurve());
            m_ShipModel->insertSection(this->m_SplinesListWidget->currentRow() - 3, newSection);
            emit modelChanged(this->m_ShipModel);
            SetVisualInterpolationType();
            CallOZDialog();
        }
    }
}

SPtr<MbAssembly> CreateSectionSet(SPtr<MbItem>& assemblyItem) {
    SPtr<MbAssembly> a;
    if (assemblyItem != nullptr) {
        a.assign(new MbAssembly());
        a->AddItem(*assemblyItem);
    }
    return a;
}

bool MainWnd::saveModelToFile(ShipModel& model, const c3d::path_string& filePath)
{
    std::vector <ShipSection>sections = model.getAllSections();
    if (!sections.empty())
    {
        SPtr<MbItem> item1 = CreateC3DSection(*(sections[0].getCurve().get()));
        SPtr<MbAssembly> assembly = CreateSectionSet(item1);
        for (size_t  x = 1; x < sections.size(); x++)
        {

            SPtr<MbItem> item {new MbPlaneInstance(*sections[x].getCurve().get(), MbPlacement3D::global)};
            assembly.get()->AddItem(*item.get());
        }
        SPtr<MbItem> Saveitem{ assembly.get() };
        const auto exportRes = c3d::ExportIntoFile(*Saveitem, filePath);
        if (exportRes == cnv_Success) {
            return true;
        }
        else
            return false;
    }
    return false;
}

bool MainWnd::getModelFromFile(const c3d::path_string& filePath) {
    SPtr<MbItem> it;
    auto importres = c3d::ImportFromFile(it, filePath);
    if (importres == 0) {
        std::string path="";
        for (size_t i = 0; i < filePath.size()-4; i++)
        {
            path += *(filePath.begin()+i);
        }
        std::ifstream file(path+".dat");
        
        m_SplinesListWidget->setCurrentRow(1);
        m_DrawingWidget->lockDrawing();
        this->m_ShipModel->clear();
        MbAssembly* assembly = dynamic_cast<MbAssembly*>(it.get());
        RPArray< MbItem > items;
        auto type = assembly->IsA();
        assembly->GetItems(items);
        std::vector<std::string> names;
        int ZCord = 0;
        for (size_t i = 0; i < assembly->ItemsCount(); i++)
        {
            
            MbPlaneInstance* itemtype = dynamic_cast<MbPlaneInstance*> (items[i]);
            const MbPlaneItem* planeItem = itemtype->GetPlaneItem();
            auto NotConstPlaneItem = const_cast <MbPlaneItem*> (planeItem);
            MbCurve* curve = dynamic_cast<MbCurve*>(NotConstPlaneItem);
            SPtr<MbCurve> SptrCurve;
            SptrCurve.assign(curve);
            std::shared_ptr<ShipSection> section = ShipSection::createSectionByCurve(SptrCurve);
            section.get()->setSplineZ(ZCord);
            ZCord += 30;
            
            std::string name;
            getline(file, name);
            names.push_back(name);
            std::string xCord;
            std::string yCord;
            std::string zCord;
            getline(file, xCord);
            getline(file, yCord);
            getline(file, zCord);
            section->setSplineCord(QString::fromStdString(xCord).toDouble(), QString::fromStdString(yCord).toDouble(), QString::fromStdString(zCord).toDouble());
            this->m_ShipModel->pushSection(section);
            //frr
        }

        this->m_SplinesListWidget->clear();
        this->m_SplinesListWidget->modelLoaded(names);
        emit giveCountOfSections(this->m_ShipModel->sectionsCount());
        emit modelChanged(m_ShipModel);
        emit ModelLoaded();
        m_DrawingWidget->unlockDrawing();
        return true;  
    }
    return false;
    
}

void MainWnd::onSaveFile() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."), "", tr(".c3d"));
    if (!fileName.isEmpty())
    {
       auto C3dfileName = fileName + ".c3d";
        saveModelToFile(*this->m_ShipModel, c3d::StdToPathstring(C3dfileName.toStdString()));
       std::ofstream file(fileName.toStdString()+".dat");

        if (file.is_open()) {
            for (size_t i = 1; i <= this->m_ShipModel->sectionsCount(); i++) {
                file << m_SplinesListWidget->item(i)->text().toStdString() << std::endl;
                file << m_ShipModel->getSection(i - 1)->GetPlacement().GetOrigin().x << std::endl;
                file << m_ShipModel->getSection(i - 1)->GetPlacement().GetOrigin().y << std::endl;
                file << m_ShipModel->getSection(i - 1)->GetPlacement().GetOrigin().z << std::endl;
            }
            file.close();
            return;
        }
    }
}

void MainWnd::onOpenFile() {

    QString fileName = QFileDialog::getOpenFileName(this, tr("Save as..."), "", tr("C3D files (*.c3d)"));
    if (fileName.isEmpty())
        return;
    else {
        this->getModelFromFile(c3d::StdToPathstring(fileName.toStdString()));
        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"), file.errorString());
            return;
        }
    }
    setWindowTitle(fileName);
    if (m_ShipModel->sectionsCount() >= 1)
    {
        m_SplinesListWidget->setCurrentRow(2);
    }
}