#include "DrawingWdgt.h"
#include <mesh_polygon.h>

#define MinusPoint QPointF(-1.f, -1.f) 

const auto GRID_STEP = 20;
const QPen defaultPen(Qt::blue, 3);
const QPen highlightPen(Qt::red, 4);
const QPen lineToPen(Qt::black, 5);
const QPen ActiveLayerPen(Qt::red,7);
const QPen PointsPen(Qt::green, 10);
const QPen DelitingPen(Qt::magenta, 10);
const QSize m_defaultSize(850, 700);
bool m_isLocked = true;
bool m_isResetSpline = false;
bool m_availableToRead = false;
bool m_IsModelLoaded = false;
bool m_isDelitingMod = false;
bool m_gridOn=false;
bool m_notLoaded = true;
int m_NumberOfMoveblePoint=-1;
int m_NumberOfAddingPoints = -1;
QPointF mouseGridCord= MinusPoint;
bool m_isRelisedLeft;
size_t shipSectionActiveIndex=0;
std::vector <QVector<QPointF>> modelPoints=std::vector <QVector<QPointF>>(100);
std::vector<ShipSection> allSections;
std::vector<MbCartPoint> AddingPoints;

static QPoint QPointFromMbCartPoint(MbCartPoint point) { return QPoint(point.x, point.y); }

DrawingWdgt::DrawingWdgt(QWidget* parent, int new_drawX, int new_drawY) :
	QWidget(parent),
	m_drawX(new_drawX),
	m_drawY(new_drawY),
	m_mousePos(QPointF()){
	if (m_drawX == 250 && m_drawY == 250)
	this->setCursor(Qt::CrossCursor);
	setMouseTracking(true);
}

void DrawingWdgt::clearMutablePoints()
{
	m_hotpointsArray.clear();
	m_pointsArray.clear();
}

void DrawingWdgt::changeGridMod(bool mod)
{
	if (!mod)
		mouseGridCord = MinusPoint;
	m_gridOn = mod;

}

void DrawingWdgt::setActiveLayerIndex(size_t index)
{
	if(index >=1)
	{
		drawChosenLayer(index-1);
	}

}

QPointF DrawingWdgt::calculateCoordinates(QPointF mouse_cords) {
	return QPointF(m_drawX * (mouse_cords.x() / this->size().width()), m_drawY * (mouse_cords.y() / this->size().height()));
}

QPoint DrawingWdgt::calculatePixels(QPointF point, double ShiftX, double ShiftY){
	return QPoint((point.x() / m_drawX) * this->size().width() +ShiftX, ( - (point.y() / m_drawY) * this->size().height()) + ShiftY);
}

QVector<QPointF> DrawingWdgt::createQVector(SArray<MbCartPoint>* saray){
	QVector<QPointF> result;
	for (auto it = saray->begin(); it < saray->end(); it++)
		result.push_back(QPointF(it->x, it->y));
	return result;
}

void DrawingWdgt::whichPointMatchMouseCords(QMouseEvent* event)
{
	if (!modelPoints[shipSectionActiveIndex].empty())
	{
	m_mousePos =calculatePixels(QPoint(event->pos()));
	auto iter = std::find_if(modelPoints[shipSectionActiveIndex].begin(), modelPoints[shipSectionActiveIndex].end(), [event,this](QPointF point)
		{
			return std::abs(calculateCoordinates(event->pos()).x() - point.x()) < 6.f && std::abs(calculateCoordinates(event->pos()).y() - point.y()*-1.f) < 6.f;	
		});
	if (iter != modelPoints[shipSectionActiveIndex].end())
	{
		m_NumberOfMoveblePoint= std::distance(modelPoints[shipSectionActiveIndex].begin(), iter);
		return;
	}
	else
		m_NumberOfMoveblePoint = -1;
	}
	m_NumberOfMoveblePoint = -1;
	if (!AddingPoints.empty())
	{
		m_mousePos = calculatePixels(QPoint(event->pos()));
		auto iter = std::find_if(AddingPoints.begin(), AddingPoints.end(), [event, this](MbCartPoint point)
			{
				return std::abs(calculateCoordinates(event->pos()).x() - point.x) < 4.f && std::abs(calculateCoordinates(event->pos()).y() - point.y * -1.f) < 4.f;
			});
		if (iter != AddingPoints.end())
		{
			m_NumberOfAddingPoints = std::distance(AddingPoints.begin(), iter);
			return;
		}
	}
}

void DrawingWdgt::mouseMoveEvent(QMouseEvent* event)
{
	if(m_NumberOfMoveblePoint!=-1)
	{
		emit MoveModelPoint(calculateCoordinates(event->pos()).x(), calculateCoordinates(event->pos()).y()*-1 / m_scale, m_NumberOfMoveblePoint, shipSectionActiveIndex);
	}
	if (!m_gotEndPoint && m_gridOn && m_modelPointer!=nullptr && shipSectionActiveIndex+1>m_modelPointer->sectionsCount()-m_modelPointer->countOfNullSections())
	{
		if (event->pos().x() % GRID_STEP <= GRID_STEP-1 && event->pos().y() % GRID_STEP <= GRID_STEP-1)
		{
			mouseGridCord = QPointF(int(event->pos().x() / GRID_STEP) * GRID_STEP, int(event->pos().y() / GRID_STEP) * GRID_STEP);
		}
		else if (event->pos().x() % GRID_STEP >= GRID_STEP+1 && event->pos().y() % GRID_STEP >= GRID_STEP+1)
		{

			mouseGridCord = QPointF(int(event->pos().x() / GRID_STEP + 1) * GRID_STEP, int(event->pos().y() / GRID_STEP + 1) * GRID_STEP);
		}
		else if (event->pos().x() % GRID_STEP >= GRID_STEP+1 && event->pos().y() % GRID_STEP <= GRID_STEP-1)
		{

			mouseGridCord = QPointF(int(event->pos().x() / GRID_STEP +1) * GRID_STEP, int(event->pos().y() / GRID_STEP) * GRID_STEP);
		}
		else if (event->pos().x() % GRID_STEP <= GRID_STEP-1 && event->pos().y() % GRID_STEP >= GRID_STEP+1)
		{

			mouseGridCord = QPointF(int(event->pos().x() / GRID_STEP) * GRID_STEP, int(event->pos().y() / GRID_STEP + 1) * GRID_STEP);
		}
		else
		{
			mouseGridCord = MinusPoint;
		}

	}
}
void DrawingWdgt::isDelitingMod()
{
	m_isDelitingMod =!m_isDelitingMod;
}

void DrawingWdgt::drawChosenLayer(size_t x)
{
	shipSectionActiveIndex = x;
	m_isResetSpline = true;
	mouseGridCord = MinusPoint;
	if (m_modelPointer != nullptr && x<m_modelPointer->sectionsCount()){
	AddingPoints.clear();
	AddingPoints = m_modelPointer->getSection(shipSectionActiveIndex).get()->IntermediantePoints();

	}

}

void DrawingWdgt::changeActiveModel(ShipModel* Model)
{
	m_modelPointer = Model;
	std::vector<ShipSection> NewModel= Model->getAllSections();
	m_availableToRead = false;
	allSections.clear();
	for (size_t t=0; t < NewModel.size(); t++)
	{
		allSections.push_back(NewModel[t]);
	}

	if (modelPoints.size() < NewModel.size())
		modelPoints.resize(NewModel.size() + 1);
	for (size_t i = 0; i < NewModel.size(); i++)
	{
		SArray<MbCartPoint> modelsPoints = NewModel[i].getPoints();
		QVector<QPointF> splynesPoints = createQVector(&modelsPoints);
		
		modelPoints[i].clear();
		for (qsizetype z = 0; z<splynesPoints.size(); z++)
		{
			modelPoints[i].push_back(splynesPoints[z]);
		}
		
	}
	AddingPoints.clear();
	if (m_modelPointer != nullptr && shipSectionActiveIndex < m_modelPointer->sectionsCount() && m_modelPointer->getSection(shipSectionActiveIndex).get()->GetInterpolationType()!= pt_Nurbs && m_modelPointer->getSection(shipSectionActiveIndex).get()->GetInterpolationType() != pt_Polyline) {
		
		AddingPoints = m_modelPointer->getSection(shipSectionActiveIndex).get()->IntermediantePoints();
	}


	m_availableToRead = true;
}

void DrawingWdgt::loadedDraw()
{
	m_isLocked = false;
	m_availableToRead = true;
	m_coordinatesArray.clear();
	m_pointsArray.clear();
	m_hotpointsArray.clear();
}

void DrawingWdgt::lockDrawing()
{
	m_isLocked = true;
	m_availableToRead = false;
	m_coordinatesArray.clear();
	m_pointsArray.clear();
	m_hotpointsArray.clear();
}

void DrawingWdgt::LoadedModel()
{
	m_IsModelLoaded = true;
}

void DrawingWdgt::UnlockModelLoaded()
{
	m_IsModelLoaded = false;
}

void DrawingWdgt::unlockDrawing()
{
	m_isLocked = false;
}

void DrawingWdgt::drawDeletingAndAddingPoints(QPainter& painter)
{
	if (!m_isDelitingMod)
		painter.setPen(PointsPen);
	else
		painter.setPen(DelitingPen);
	for (size_t x = 0; x < size_t(modelPoints[shipSectionActiveIndex].size()); x++)
	{
		if (!m_modelPointer->getSection(shipSectionActiveIndex).get()->isNullLayer())
		{

			auto tmp = painter.brush();
			painter.setBrush(Qt::green);
			painter.setPen(PointsPen);
			painter.drawEllipse(calculatePixels(modelPoints[shipSectionActiveIndex][x]), 5, 5);
			painter.setBrush(tmp);
			if (m_isDelitingMod)
			{
				painter.setPen(QPen(Qt::black, 3));
			
				painter.drawLine(calculatePixels(modelPoints[shipSectionActiveIndex][x],-5.f,5.f), calculatePixels(modelPoints[shipSectionActiveIndex][x],5.f,-5.f));
				painter.drawLine(calculatePixels(modelPoints[shipSectionActiveIndex][x], -5.f, -5.f), calculatePixels(modelPoints[shipSectionActiveIndex][x], 5.f, 5.f));

			}if (!m_isDelitingMod && !AddingPoints.empty() && x < AddingPoints.size())
			{
				QColor color = Qt::green;
				color.setAlpha(180);
				painter.setBrush(color);
				painter.setPen(QPen(color, 3));
				painter.drawEllipse(calculatePixels(QPointFromMbCartPoint(AddingPoints[x])), 7, 7);

				painter.setPen(QPen(QColor(0, 0, 0, 180), 3));
				
				painter.drawLine(calculatePixels(QPointFromMbCartPoint(AddingPoints[x]), 0.f, 5.f), calculatePixels(QPointFromMbCartPoint(AddingPoints[x]), 0.f, -5.f));
				painter.drawLine(calculatePixels(QPointFromMbCartPoint(AddingPoints[x]), 5.f), calculatePixels(QPointFromMbCartPoint(AddingPoints[x]), -5.f));
				
				painter.setBrush(tmp);
			}
		}
	}
}

void DrawingWdgt::paintEvent(QPaintEvent*) {
	QPainter p(this);
	
	if (m_notLoaded)
	{
		for (int i = 0; i < int(this->width()/10)+1; i++)
		{
			p.drawLine(i * 10, 0, i * 10, this->height() + 10);
			p.drawLine(0, i * 10 ,this->width()+10, i * 10);
		}
	}
	if (mouseGridCord != MinusPoint)
	{ 
		p.setPen(QPen(Qt::blue, 5));
		p.drawPoint(mouseGridCord);
	}
	if (m_availableToRead)
	{
		p.setPen(defaultPen);
		DrawModelsCurves(p);
		if (!m_gotEndPoint && !m_pointsArray.empty()) {
			p.setPen(highlightPen);
			p.scale(m_scale, m_scale);
			for (qsizetype i = 1; i < m_hotpointsArray.size(); i++)
			{	
				p.setPen(lineToPen);
				p.drawLine(m_hotpointsArray[i - 1], m_hotpointsArray[i]);
				p.setPen(highlightPen);
				p.drawPoint(m_hotpointsArray[i - 1]);
			}
			p.drawPoint(m_mousePos);
		}
		if (!m_gotEndPoint && m_hotpointsArray.size() == 1)
			p.drawPoint(m_hotpointsArray[0]);
		if (m_isResetSpline)
		{
			m_gotEndPoint = false;
			m_coordinatesArray.clear();
			m_pointsArray.clear();
			m_hotpointsArray.clear();
			m_gotEndPoint = false;
			m_isResetSpline = false;		
		}
		drawDeletingAndAddingPoints(p);
	}
	update();
}

void DrawingWdgt::mouseReleaseEvent(QMouseEvent* event){
	if (m_NumberOfMoveblePoint != -1 ) {
		emit MoveModelPoint(calculateCoordinates(event->pos()).x(), calculateCoordinates(event->pos()).y() * -1 / m_scale, m_NumberOfMoveblePoint, shipSectionActiveIndex);
		m_NumberOfMoveblePoint = -1;
		update();
	}
}

void DrawingWdgt::SetLayer(){
	if (m_gotEndPoint == false && (!m_IsModelLoaded) && m_hotpointsArray.size() >= 3)
	{
		m_gotEndPoint = true;
		SArray<MbCartPoint> saray(m_coordinatesArray);
		for (size_t i = 0; i < saray.size(); i++)
		{
			saray[i].y *= -1;
		}
		std::shared_ptr<ShipSection> cubCurve = ShipSection::createCubicSpline(&saray);
		MbPolygon polygon{};
		cubCurve->calculatePolygon(Math::visualSag, polygon);
		polygon.GetPoints(saray);
		m_pointsArray = createQVector(&saray);
		emit PushLayer(cubCurve);
		emit EnterOZCord();
		mouseGridCord = MinusPoint;
	}
}

void DrawingWdgt::mousePressEvent(QMouseEvent* e) {
	if (!m_isLocked) {
		if (e->button() == Qt::LeftButton)
		{
			processLeftMouseButton(e);
		}
		else
			if (e->button() == Qt::RightButton) {
				processRightMouseButton();
				return;
			}
		if (e->buttons() & Qt::LeftButton)
		{
			processActionsLeftMouseButton(e);
		}
	}
}
void DrawingWdgt::processRightMouseButton()
{
	if (m_gotEndPoint == false && (!m_IsModelLoaded) && m_hotpointsArray.size() >= 3) {
		m_gotEndPoint = true;
		SArray<MbCartPoint> saray(m_coordinatesArray);
		for (size_t i = 0; i < saray.size(); i++)
		{
			saray[i].y *= -1;
		}
		std::shared_ptr<ShipSection> cubCurve = ShipSection::createCubicSpline(&saray);
		MbPolygon polygon{};
		cubCurve->calculatePolygon(Math::visualSag, polygon);
		polygon.GetPoints(saray);
		m_pointsArray = createQVector(&saray);
		emit PushLayer(cubCurve);
		emit EnterOZCord();
		mouseGridCord = MinusPoint;
	}
}
void DrawingWdgt::processLeftMouseButton(QMouseEvent* e)
{
	if ( m_NumberOfMoveblePoint != -1)
	{
		m_NumberOfMoveblePoint = -1;
		return;
	}
	if (!m_gotEndPoint && allSections[shipSectionActiveIndex].getNumberOfPoints() == 3) {
		if (m_scale != 0) {
			m_mousePos.setX(e->pos().x() / m_scale);
			m_mousePos.setY(e->pos().y() / m_scale);
		}
		else {
			m_mousePos.setX(e->pos().x());
			m_mousePos.setY(e->pos().y());
		}
		if (m_hotpointsArray.size() != 0 && (m_hotpointsArray[m_hotpointsArray.size() - 1] != m_mousePos && m_hotpointsArray[m_hotpointsArray.size() - 1] != mouseGridCord)) {

			if (mouseGridCord != MinusPoint)
			{
				AddPointToLocalModel(mouseGridCord);
			}
			else
			{
				AddPointToLocalModel(m_mousePos);
			}
		}
		else if (m_hotpointsArray.empty())
		{
			if (mouseGridCord != MinusPoint)
			{
				AddPointToLocalModel(mouseGridCord);
			}
			else
			{
				AddPointToLocalModel(m_mousePos);
			}
		}
		std::cout << m_mousePos.x() << " - x; " << m_mousePos.y() << " - y" << std::endl;
	}
}
void DrawingWdgt::processActionsLeftMouseButton(QMouseEvent* e)
{
	if ( m_NumberOfMoveblePoint == -1 && m_isDelitingMod)
	{
		whichPointMatchMouseCords(e);
		if (m_NumberOfMoveblePoint != -1 && m_modelPointer->getSection(shipSectionActiveIndex).get()->getNumberOfPoints() > 2)
		{
			int pointIndex = m_NumberOfMoveblePoint;
			m_NumberOfMoveblePoint = -1;
			emit deletePoint(pointIndex);
		}

		return;
	}
	else if (m_NumberOfMoveblePoint == -1 || m_NumberOfAddingPoints == -1)
	{
		whichPointMatchMouseCords(e);
		if (m_NumberOfAddingPoints != -1)
		{
			emit addPoint(AddingPoints[m_NumberOfAddingPoints]);
			m_NumberOfAddingPoints = -1;
		}

	}
}
void DrawingWdgt::AddPointToLocalModel(QPointF point)
{
	m_pointsArray.push_back(point);
	m_hotpointsArray.push_back(point);
	QPointF coord = calculateCoordinates(point);
	m_coordinatesArray.push_back(MbCartPoint(coord.x(), coord.y()));
}
void DrawingWdgt::DrawModelsCurves(QPainter& painter)
{
	for (size_t i = 0; i < m_modelPointer->sectionsCount(); i++)
	{
		if (!m_modelPointer->getSection(i).get()->isNullLayer() && shipSectionActiveIndex != i)
		{


			SArray<MbCartPoint> saray;
			MbPolygon polygon{};
			m_modelPointer->getSection(i).get()->calculatePolygon(Math::visualSag, polygon);
			polygon.GetPoints(saray);
			m_pointsArray = createQVector(&saray);
			for (qsizetype x = 1; x < m_pointsArray.size(); x++)
			{
				painter.drawLine(calculatePixels(m_pointsArray[x - 1]), calculatePixels(m_pointsArray[x]));
			}
		}
	}

	if (shipSectionActiveIndex < m_modelPointer->sectionsCount() && !m_modelPointer->getSection(shipSectionActiveIndex).get()->isNullLayer())
	{
		SArray<MbCartPoint> activeSaray;
		MbPolygon activePolygon{};
		m_modelPointer->getSection(shipSectionActiveIndex).get()->calculatePolygon(Math::visualSag, activePolygon);
		activePolygon.GetPoints(activeSaray);
		m_pointsArray = createQVector(&activeSaray);
		painter.setPen(ActiveLayerPen);
		for (qsizetype x = 1; x < m_pointsArray.size(); x++)
		{
			painter.drawLine(calculatePixels(m_pointsArray[x - 1]), calculatePixels(m_pointsArray[x]));
		}
		painter.setPen(defaultPen);
	}
}