#include "ShipModel.h"



void ShipModel::pushSection(std::shared_ptr<ShipSection> section)
{
	this->Layers.push_back(section);
}

void ShipModel::eraseSection(size_t x)
{
	if (this->sectionsCount() >= x)
		this->Layers.erase(this->Layers.begin() + x);
}

std::shared_ptr<ShipSection> ShipModel::getSection(size_t x)
{
	return this->Layers[x];
}

size_t ShipModel::sectionsCount() const
{
	return this->Layers.size();
}

void ShipModel::insertSection(size_t x, std::shared_ptr<ShipSection> section)
{
	this->Layers.insert(this->Layers.begin() + x, section);
}

ShipModel::ShipModel()
{
}

size_t ShipModel::countOfNullSections()
{
	size_t counter=0;
	for (size_t i = 0; i < Layers.size(); i++)
	{
		if (this->Layers[i].get()->isNullLayer())
			counter++;
	}
	return counter;
}

DllExport std::vector<ShipSection> ShipModel::getAllSections()
{
	if (!this->Layers.empty())
	{
		std::vector<ShipSection> allsplines;
		for (size_t i = 0; i < this->sectionsCount(); i++)
		{
			if (Layers.size() < i)
				allsplines.push_back(*(this->getSection(i).get()));
			else
				allsplines.insert(allsplines.begin() + i, *(this->getSection(i).get()));
		}
		return allsplines;
	}
}
void ShipModel::clear()
{
	Layers.clear();
}

void ShipModel::deleteNull(int layerNumber)
{
	if (this->Layers[layerNumber].get()->isNullLayer() == true)
	{
		this->Layers.erase(this->Layers.begin()+ layerNumber);
		std::cout<< "Erasing Layer number " << layerNumber;
	
	}
}

SArray<MbPlacement3D> ShipModel::getAllPlasements()
{
	SArray<MbPlacement3D> placements;
	for (const auto l : Layers)
		placements.push_back(l->GetPlacement());

	return placements;
}

bool ShipModel::empty()
{
	return Layers.empty();
}
