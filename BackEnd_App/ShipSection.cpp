#include "ShipSection.h"

ShipSection::ShipSection(SArray<MbCartPoint>* points, MbePlaneType type) {

	MbCurve* linePointer{ nullptr };
	switch (type) {
		//Кубический сплайн
		case(MbePlaneType::pt_CubicSpline) :
			    SplineCurve(*points, false, MbePlaneType::pt_CubicSpline, linePointer);
				break;

		//Ломаная
		case(MbePlaneType::pt_Polyline) :
				SplineCurve(*points, false, MbePlaneType::pt_Polyline, linePointer);
				break;

		//Сплайн Эрмита
		case(MbePlaneType::pt_Hermit) :
			SplineCurve(*points, false, MbePlaneType::pt_Hermit, linePointer);
			break;

		//NURBS
		case(MbePlaneType::pt_Nurbs) : 
			SplineCurve(*points, false, MbePlaneType::pt_Nurbs, linePointer);
		
			break;
			
		case(pt_PolyCurve):
		
			auto error = SplineCurve(*points, false, pt_PolyCurve, linePointer);
			break;
	}
	static double z = 0.f;
	m_LocalPlacement = MbPlacement3D(MbCartPoint3D(0,0,z));
	z += 10.;
	m_line = linePointer;
}

void ShipSection::calculatePolygon(double sag, MbPolygon& poligon) {
	m_line->CalculatePolygon(sag, poligon);
}

void ShipSection::calculateNormals(std::vector<MbDirection>& dir, std::vector<MbCartPoint>& points) {
	MbCartPoint point;
	MbDirection elem;
	for (double a = m_line->GetTMin(); a < m_line->GetTMax(); a += m_line->Step(a, Math::visualSag)) {
		m_line->PointOn(a, point);
		points.push_back(point);
		m_line->Normal(a, elem);
		dir.push_back(elem);
	}
}

void ShipSection::addPoint(MbCartPoint &NewPoint)
{
	auto curve = m_line.static_cast_to<MbPolyCurve>().get();
	auto t = m_line.get()->PointProjection(NewPoint);
	curve->InsertPoint(t, NewPoint, 1.f, 1.f);
	m_line.assign(static_cast <MbCurve*>(curve));
	
}

void ShipSection::deletePoint(size_t index)
{
	if ( !m_frozenPoints.empty()  && *(m_frozenPoints.find(index)) == index)
	{
		m_frozenPoints.erase(m_frozenPoints.find(index));
	}
	MbPolyCurve* curve = static_cast <MbPolyCurve*>(m_line.get());
	curve->RemovePoint(index);
	m_line.assign(static_cast <MbCurve*>(curve));
	ChangeInterpolationType(this->GetInterpolationType());
}

SArray<MbCartPoint> ShipSection::getPoints(bool IsChangeInterpolation) const
{
	
	if(m_line.get()->IsA()== pt_Nurbs && IsChangeInterpolation)
	{
		auto curve = m_line.static_cast_to<MbPolyCurve>().get();
		SArray<MbCartPoint> SarOfPoints;
		for (size_t i = 0; i < curve->GetPointsCount(); i++)
		{
			MbCartPoint NurbsPoint;
			curve->GetPoint(i, NurbsPoint);
			auto t = m_line->PointProjection(NurbsPoint);
			m_line->PointProjectionNewton(NurbsPoint,1.f,1.f,c3d::NEWTON_COUNT,t,false);
			auto point =m_line.get()->PointOn(t);
			SarOfPoints.push_back(point);

		}
		return SarOfPoints;

	}
	else
	{
		auto curve = m_line.static_cast_to<MbPolyCurve>().get();
		SArray<MbCartPoint> SarOfPoints;
		curve->GetPoints(SarOfPoints);
		return SarOfPoints;
	}
}

std::set<size_t> ShipSection::getFrozenPoints()
{
	return m_frozenPoints;
}

void ShipSection::addFrozenPoint(size_t index)
{
	m_frozenPoints.insert(index);
}

void ShipSection::removeFrozenPoint(size_t index)
{
	m_frozenPoints.erase(index);
}

void ShipSection::movePoint(MbVector vector, size_t index)
{
	if (m_frozenPoints.count(index) == 0)
	{
		MbControlData Points;
		m_line->GetBasisPoints(Points);
		Points[index].Move(vector);
		m_line->SetBasisPoints(Points);
		m_line->Refresh();
		this->ChangeInterpolationType(this->GetInterpolationType());
	}
}
void ShipSection::movePoint(size_t index,double x,double y)
{
	if (m_frozenPoints.count(index) == 0)
	{
		MbControlData Points;
		m_line->GetBasisPoints(Points);
		Points[index].Move(x-Points[index].x  ,y-Points[index].y);
		m_line->SetBasisPoints(Points);
		m_line->Refresh();
		this->ChangeInterpolationType(this->GetInterpolationType());
	}
}

void ShipSection::movePointByX(size_t index, double x)
{
	if (m_frozenPoints.count(index) == 0)
	{
		MbControlData Points;
		m_line->GetBasisPoints(Points);
		Points[index].x=x;
		m_line->SetBasisPoints(Points);
		m_line->Refresh();
	}
}

void ShipSection::movePointByY(size_t index, double y)
{
	if (m_frozenPoints.count(index) == 0)
	{
		MbControlData Points;
		m_line->GetBasisPoints(Points);
		Points[index].y = y;
		m_line->SetBasisPoints(Points);
		m_line->Refresh();
	}
}

std::vector<MbCartPoint> ShipSection::IntermediantePoints()
{
	if (!m_iSNullLayer)
	{
		std::vector<MbCartPoint> IntermediantePoints;
		SArray<double> Vertical_T;
		SArray<double> Horizontal_T;
		auto curve = m_line.static_cast_to<MbPolyCurve>().get();
		for (size_t i = 0; i < curve->GetPointsCount() - 1; i++)
		{
			MbCartPoint point1;
			MbCartPoint point2;
			curve->GetPoint(i, point1);
			curve->GetPoint(i + 1, point2);

			auto T1 = m_line->PointProjection(point1);
			auto T2 = m_line->PointProjection(point2);
			auto t = (T2 + T1) / 2;
			IntermediantePoints.push_back(m_line.get()->PointOn(t));
		}
		return IntermediantePoints;
	}
	return std::vector<MbCartPoint>{};
}


bool ShipSection::rotatePoints(size_t index, double angle)
{	if (m_frozenPoints.empty()==true || (m_frozenPoints.size() == 1 && *(m_frozenPoints.begin())==index))
	{	
		MbControlData Points;
		m_line->GetBasisPoints(Points);
		Points.Rotate(Points[index], angle);
		m_line->SetBasisPoints(Points);
		m_line->Refresh();
	}
	return false;
}
bool ShipSection::multiplyPoints(double multiplayerX, double multiplayerY)
{
	if (m_frozenPoints.empty() == true)
	{
		MbControlData Points;
		m_line->GetBasisPoints(Points);

		for (size_t i = 0; i != Points.Count(); i++)
		{
			Points[i].Scale(multiplayerX, multiplayerY);
		}
		m_line->SetBasisPoints(Points);
		m_line->Refresh();
		return true;
	}
	return false;
}

size_t ShipSection::getNumberOfPoints()
{
	MbControlData Points;
	m_line->GetBasisPoints(Points);
	return Points.Count();
}

MbCartPoint3D ShipSection::getSplineCord()
{
	return this->m_LocalPlacement.GetOrigin();
}

void ShipSection::setSplineCord(double x, double y, double z)
{
	m_LocalPlacement.SetOrigin( MbCartPoint3D(x, y, z) );
}

void ShipSection::setSplineX(double x)
{
	this->m_LocalPlacement.SetOrigin( MbCartPoint3D(x,m_LocalPlacement.GetOrigin().origin.y, m_LocalPlacement.GetOrigin().origin.z));
}
void ShipSection::setSplineY(double y)
{
	this->m_LocalPlacement.SetOrigin(MbCartPoint3D(m_LocalPlacement.GetOrigin().origin.x, y, m_LocalPlacement.GetOrigin().origin.z));
}
void ShipSection::setSplineZ(double z)
{
	this->m_LocalPlacement.SetOrigin(MbCartPoint3D(m_LocalPlacement.GetOrigin().origin.x, m_LocalPlacement.GetOrigin().origin.y ,z));
}

SPtr<MbCurve> ShipSection::getCurve()
{
	return m_line;
}

void ShipSection::setNullLayer()
{
	this->m_iSNullLayer = true;
}

bool ShipSection::isNullLayer()
{
	return this->m_iSNullLayer;
}

MbePlaneType ShipSection::GetInterpolationType()
{
	return  m_line->IsA();
}

MbPlacement3D ShipSection::GetPlacement()
{
	return m_LocalPlacement;
}

void ShipSection::ChangeInterpolationType(MbePlaneType type)
{
	MbCurve* linePointer{ nullptr };
	switch (type) {
		//Кубический сплайн
	case(MbePlaneType::pt_CubicSpline):
		SplineCurve(this->getPoints(true), false, MbePlaneType::pt_CubicSpline, linePointer);
		break;

	case(MbePlaneType::pt_Polyline):
		SplineCurve(this->getPoints(true), false, MbePlaneType::pt_Polyline, linePointer);
		break;

		//Сплайн Эрмита
	case(MbePlaneType::pt_Hermit):
		SplineCurve(this->getPoints(true), false, MbePlaneType::pt_Hermit, linePointer);
		break;

		//
	case(MbePlaneType::pt_Nurbs):
		auto s =SplineCurve(this->getPoints(true), false, MbePlaneType::pt_Nurbs, linePointer);
		break;
	}
	m_line = linePointer;
}

std::shared_ptr<ShipSection> ShipSection::createPolyline(SArray<MbCartPoint>* points) {
	if (points != nullptr && points->size() >= 2)
	{
		ShipSection ship (points, MbePlaneType::pt_Polyline);
		return std::make_shared<ShipSection>(std::move(ship));
	}
	else
		return nullptr;
}

std::shared_ptr<ShipSection> ShipSection::createCubicSpline(SArray<MbCartPoint>* points) {
	if (points != nullptr && points->size() >= 2) 
	{
		ShipSection ship(points, MbePlaneType::pt_CubicSpline);
		return std::make_shared<ShipSection>(ship);
	}
	else
		return nullptr;
}

std::shared_ptr<ShipSection> ShipSection::createHermit(SArray<MbCartPoint>* points) {
	if (points != nullptr && points->size() >= 2) 
	{
		ShipSection ship(points, MbePlaneType::pt_Hermit);
		return std::make_shared<ShipSection>(ship);
	}
	else
		return nullptr;
}

std::shared_ptr<ShipSection> ShipSection::createNURBS(SArray<MbCartPoint>* points) {
	if (points != nullptr && points->size() >= 2) 
	{
		ShipSection ship(points, MbePlaneType::pt_Nurbs);
		return std::make_shared<ShipSection>(ship);
	}
	else
		return nullptr;
}

std::shared_ptr<ShipSection> ShipSection::createSectionByCurve(SPtr<MbCurve> curve)
{	

	if (curve != nullptr)
	{	
		auto polyCurve = curve.static_cast_to<MbPolyCurve>().get();;
		SArray<MbCartPoint> SarOfPoints;
		polyCurve->GetPoints(SarOfPoints);
		ShipSection ship(&SarOfPoints, curve->IsA());
		return std::make_shared<ShipSection>(ship);
	}
	else
		return nullptr;
}