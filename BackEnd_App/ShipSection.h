#ifndef __SHIP_SECTION_H
#define __SHIP_SECTION_H
#pragma once 
#include "templ_sptr.h"
#include "curve.h"
#include "action_curve.h"
#include "templ_s_array.h"
#include "mb_placement3d.h"
#include <cmath>
#include <set>
#include <string>
#include <cur_polycurve.h>
#include <model_item.h>
#include <plane_instance.h>
#include <cur_nurbs.h>
#include <mb_nurbs_function.h>
#include <mb_variables.h>


//класс представляющий собой секцию корабля
//создание объекта класса происходит через статические методы
class ShipSection {

private:

	SPtr<MbCurve> m_line{ nullptr };
	std::set <size_t> m_frozenPoints;
	bool m_iSNullLayer=false;
	MbPlacement3D m_LocalPlacement;
	ShipSection(SArray<MbCartPoint>* points, MbePlaneType type);

public:
	ShipSection() = delete;
	ShipSection(const ShipSection& other) = default;
	//выдать полигон для отрисовки
	void calculatePolygon(double sag, MbPolygon& poligon);

	void calculateNormals(std::vector < MbDirection>& dir, std::vector<MbCartPoint>& points);

	//создаёт объект класса с полилинией, возвращает умный указатель на него	
	static std::shared_ptr<ShipSection> createPolyline(SArray<MbCartPoint>* points);
	
	//создаёт объект класса с кубическим сплайном, возвращает умный указатель на него
	static std::shared_ptr<ShipSection> createCubicSpline(SArray<MbCartPoint>* points);
	
	//создаёт объект класса со сплайном эрмита, возвращает умный указатель на него
	static std::shared_ptr<ShipSection> createHermit(SArray<MbCartPoint>* points);
	
	//создаёт объект класса с NURBS кривую, возвращает умный указатель на него
	static std::shared_ptr<ShipSection> createNURBS(SArray<MbCartPoint>* points);
	
	//создаёт объект класса по уже существующей кривой, возвращает умный указатель на него
	static std::shared_ptr<ShipSection> createSectionByCurve(SPtr<MbCurve> curve);
	
	//получить замороженные точки
	std::set<size_t> getFrozenPoints();
	
	//добавить замороженную точку
	void addFrozenPoint(size_t index);
	
	// добавить точку в кривую
	void addPoint(MbCartPoint& NewPoint);
	
	//удалить точку из замороженных по индексу
	void removeFrozenPoint(size_t index);
	
	//удалить точку кривой по индексу 
	void deletePoint(size_t index);
	
	// получить массив точек кривой
	SArray<MbCartPoint> getPoints(bool IsChangeInterpolation= false) const;
	
	//сдвинуть точку по вектору
	void movePoint(MbVector vector, size_t index);
	
	//сдвинуть точку на координаты x,y
	void movePoint(size_t index, double x, double y);
	
	//сдвинуть точку на координату x
	void movePointByX(size_t index, double x);
	
	//сдвинуть точку на координату н
	void movePointByY(size_t index, double y);
	
	//высчитать промежуточные точки на кривой (между 2 точками )
	std::vector<MbCartPoint> IntermediantePoints();
	
	//поворот точки
	bool rotatePoints(size_t index, double angle);
	
	//умножение координат точки
	bool multiplyPoints(double multiplayerX, double multiplayerY);
	
	//получить количество точек
	size_t getNumberOfPoints();
	
	//получить локальную позицию сплайна в виде точки
	MbCartPoint3D getSplineCord();

	// Устанавливает местоположение локальной системы координат.
	void setSplineCord(double x, double y, double z);

	// Устанавливает значение координаты X точки в локальной системе координат.
	void setSplineX(double x);

	// Устанавливает значение координаты Y точки в локальной системе координат.
	void setSplineY(double y);

	// Устанавливает значение координаты Z точки в локальной системе координат.
	void setSplineZ(double z);

	// Возвращает умный указатель на кривую.
	SPtr<MbCurve> getCurve();

	// Устанавливает слой как нулевой (null).
	void setNullLayer();

	// Проверяет, является ли слой нулевым (null).
	bool isNullLayer();

	// Изменяет тип интерполяции на заданный тип.
	void ChangeInterpolationType(MbePlaneType type);

	// Возвращает текущий тип интерполяции.
	MbePlaneType GetInterpolationType();

	// Возвращает локальную систему координат объекта.
	MbPlacement3D GetPlacement();
	
};
#endif // __SHIP_SECTION_H
