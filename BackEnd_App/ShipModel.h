#ifndef __SHIP_MODEL_H
#define __SHIP_MODEL_H
#ifdef WIN32
#define DllExport   __declspec( dllexport )
#else
#define DllExport 
#endif
#include "ShipSection.h"
#include <vector>

// класс модели корабля, хранит и оперирует массивом ShipSection
class ShipModel
{
private:

	std::vector  <std::shared_ptr<ShipSection>> Layers;
	
public:
	ShipModel();
	//выдаёт количество нулевых ShipSection в объекте
	size_t countOfNullSections();
	//добавление секции
	DllExport void pushSection(std::shared_ptr<ShipSection> section);
	//Вырезать секцию
	DllExport void eraseSection(size_t x);
	//выдать секцию по индексу	
	DllExport std::shared_ptr<ShipSection> getSection(size_t x);
	//выдать количество секций
	DllExport size_t sectionsCount() const;
	//вставить секцию по индексу
	DllExport void insertSection(size_t x, std::shared_ptr<ShipSection> section);
	//выдать все секции
	DllExport std::vector<ShipSection> getAllSections();
	//очистить массив секций
	DllExport void clear();
	//удалить нулевую секцию по индексу
	DllExport void deleteNull(int layerNumber);
	//получить все локальные системы координат секций
	SArray<MbPlacement3D> getAllPlasements();

	bool empty();
};

#endif 
