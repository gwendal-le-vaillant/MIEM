/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2017 Guillaume Villée.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <vector>
#include "IntersectionPolygon.h"
using namespace Amusing;

IntersectionPolygon::IntersectionPolygon(int64_t _Id, std::shared_ptr<CompletePolygon> m_parent1, std::shared_ptr<CompletePolygon> m_parent2, Colour _fillColour) : 
	Miam::InteractivePolygon(_Id)
{
	fillColour = _fillColour;
	isFilled = true;
	displayCenter = false;
	isNameVisible = false;
	SetOpacityMode(OpacityMode::DependingOnExcitement);
	SetAlpha(1.0f);
	
	threshold = 0.0;
	parent1 = m_parent1;
	parent2 = m_parent2;

	for (int i = 0; i < couloursBufferSize; ++i)
		coulours_buffer[i] = 0.0f;
}

IntersectionPolygon::~IntersectionPolygon()
{
}

void IntersectionPolygon::Paint(Graphics & g)
{
	if (inter.size() > 0 && parent1->isVisible() && parent2->isVisible())
		DrawablePolygon::Paint(g);
}

void IntersectionPolygon::CanvasResized(SceneCanvasComponent * _parentCanvas)
{
	parentCanvas = _parentCanvas;

	/// On part des polygones parents pour cr�er le polygone
	if (parent1->isVisible() && parent2->isVisible())
	{
		// compute the intersection
		bpolygon poly1, poly2;
		poly1 = parent1->getPolygon();
		poly2 = parent2->getPolygon();
		boost::geometry::correct(poly1);
		boost::geometry::correct(poly2);

		inter.clear();
		boost::geometry::intersection(poly1, poly2, inter);

		// threshold
		std::vector<int> idToDelete;
		for (int i = 0; i < (int)inter.size(); ++i)
			if (boost::geometry::area(inter[i]) < threshold)
				idToDelete.push_back(i);

		for (int i = (int)idToDelete.size(); i != 0; --i)
			inter.erase(inter.begin() + idToDelete[i]);

		//rescale
		bpolygon tmp;
		boost::geometry::strategy::transform::scale_transformer<double, 2, 2> scaler(_parentCanvas->getWidth(), _parentCanvas->getHeight());
		for (int i = 0; i < inter.size(); ++i)
		{
			boost::geometry::transform(inter[i], tmp, scaler);
			inter[i].clear();
			inter[i] = tmp;
			tmp.clear();
		}

	/// appartenance des diff�rents points aux diff�rents polygons
	//clear memory
	parent1ApexesAngle.clear();
	parent2ApexesAngle.clear();
	//verification des sommets de parent1
	for (int i = 0; i < poly1.outer().size(); ++i)
	{
		if(parent2->contains(poly1.outer().at(i))) // si est contenu dans parent 2 -> garder en memoire
			parent1ApexesAngle.push_back(parent1->getAngularPercentage(poly1.outer().at(i)));
	}

		for (int i = 0; i < poly2.outer().size(); ++i)
		{
			if (parent1->contains(poly2.outer().at(i))) // si est contenu dans parent 1 -> garder en memoire
				parent2ApexesAngle.push_back(parent2->getAngularPercentage(poly2.outer().at(i)));
		}

		/// creation of JUCE polygon
		contour.clear();
		for (int j = 0; j < inter.size(); ++j)
		{
			contour.startNewSubPath((float)inter.at(j).outer().at(0).get<0>(), (float)inter.at(j).outer().at(0).get<1>());
			for (size_t i = 1; i < inter.at(j).outer().size(); i++)
				contour.lineTo((float)inter.at(j).outer().at(i).get<0>(), (float)inter.at(j).outer().at(i).get<1>());
			contour.closeSubPath();
		}
	}
}

void IntersectionPolygon::setSurfaceThreshold(double m_threshold)
{
	threshold = m_threshold;
}

bool IntersectionPolygon::isChild(std::shared_ptr<CompletePolygon> m_parent)
{
	if (m_parent == parent1 || m_parent == parent2)
		return true;
	return false;
}

bool IntersectionPolygon::isEmpty()
{
	if (inter.size() > 0)
		return false;
	else
		return false;
}

std::shared_ptr<CompletePolygon> Amusing::IntersectionPolygon::getNearestParent(bpt location)
{
	double d1 = boost::geometry::distance(location, parent1->getCenter());
	double d2 = boost::geometry::distance(location, parent2->getCenter());
	if (d1 < d2)
		return parent1;
	else
		return parent2;
}

std::shared_ptr<CompletePolygon> Amusing::IntersectionPolygon::getOtherParent(std::shared_ptr<CompletePolygon> parentA)
{
	if (parentA == parent1)
		return parent2;
	else if (parentA == parent2)
		return parent1;
	else
		return nullptr;
}

int IntersectionPolygon::getApexesCount(std::shared_ptr<CompletePolygon> parent)
{
	if (parent1->isVisible() && parent2->isVisible())
	{
		if (parent == parent1)
			return (int)parent1ApexesAngle.size();
		else if (parent == parent2)
			return (int)parent2ApexesAngle.size();
		else
			return 0;
	}
	else
		return 0;
}

double IntersectionPolygon::getApexesAngle(std::shared_ptr<CompletePolygon> parent, int index)
{
	if (parent == parent1)
	{
		if (index < parent1ApexesAngle.size())
			return parent1ApexesAngle[index];
		else
			return 0.0;
	}
	else if (parent == parent2)
	{
		if (index < parent2ApexesAngle.size())
			return parent2ApexesAngle[index];
		else
			return 0.0;
	}
	else
		return 0.0;
}

void IntersectionPolygon::RefreshOpenGLBuffers()
{
	/*for (int i = 0; i < couloursBufferSize; ++i)
		coulours_buffer[i] = 0.0f;*/
}
