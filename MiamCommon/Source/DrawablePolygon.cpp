//
//  DrawablePolygon.cpp
//  MiamEdit
//
//  Created by Gwendal Le Vaillant on 18/02/2016.
//
//

#include "DrawablePolygon.h"

#include "SceneCanvasComponent.h"

#include "XmlUtils.h"

using namespace Miam;

DrawablePolygon::DrawablePolygon(bptree::ptree & areaTree)
:
DrawableArea(areaTree)
{

	int W = areaTree.get<int>("<xmlattr>.width");
	int H = areaTree.get<int>("<xmlattr>.height");
	float oldCanvasRation = (float)W / (float)H;
	if (oldCanvasRation > 1.0F)
	{
		xScale = 1.0f / oldCanvasRation;
		yScale = 1.0f;
	}
	else
	{
		xScale = 1.0f;
		yScale = 1.0f / oldCanvasRation;
	}


    // Vérification préliminaire de présence des points du polygone
    size_t pointsCount = XmlUtils::CheckIndexes(areaTree, "geometry", "point");
    // Pré-chargement de chaque point de contour de <geometry>
    std::vector< bpt > polygonPoints;
    polygonPoints.resize(pointsCount);
    for (auto& point : areaTree.get_child("geometry"))
    {
        auto index = point.second.get<size_t>("<xmlattr>.index");
        try {
            polygonPoints[index].set<0>( point.second.get<double>("<xmlattr>.x") );
            polygonPoints[index].set<1>( point.second.get<double>("<xmlattr>.y") );
        }
        catch (bptree::ptree_error &e) {
            throw XmlReadException("Point " + boost::lexical_cast<std::string>(index) + ": ", e);
        }
    }
    // Ajout des points 1 par 1 dans le bon ordre
    for (auto& point : polygonPoints)
        contourPoints.outer().push_back(point);
    contourPoints.outer().push_back(polygonPoints[0]);// contour closing
    // Actualisation graphique
    createJucePolygon();
}

DrawablePolygon::DrawablePolygon(int64_t _Id) :
    DrawablePolygon(_Id, bpt(0.5f,0.5f), 3, 0.1f, Colours::darkgrey)
{
}


DrawablePolygon::DrawablePolygon(int64_t _Id, bpt _center, int pointsCount, float radius, Colour _fillColour, float _canvasRatio) :
    DrawableArea(_Id, _center, _fillColour)
{

    if (_canvasRatio > 1.0f) // ratio of an landscape-oriented window
    {
        xScale = 1.0f/_canvasRatio;
        yScale = 1.0f;
    }
    else // ratio of an portrait-oriented window
    {
        xScale = 1.0f;
        yScale = 1.0f*_canvasRatio;
    }
    
    // Computation of the points
    float currentAngle;
    for (int i=0; i<pointsCount ; i++)
    {
        currentAngle = 2.0f*float_Pi*(float)(i)/(float)(pointsCount);
		contourPoints.outer().push_back(bpt(center.get<0>() + radius*xScale*cosf(currentAngle),
			center.get<1>() + radius*yScale*sinf(currentAngle)));
    }
	 // to close the boost polygon
	contourPoints.outer().push_back(bpt(center.get<0>() + radius*xScale, center.get<1>()));

	verticesCount = (pointsCount + 1);
	//vertex_buffer = new float[verticesCount * 3];// = new float[verticesCount * 3]; // tout sommets + le centre * (x,y,z)
	outline_vertex_buffer.resize(pointsCount * 3);
	
	

	int indexCount = pointsCount * 3;
	index_buffer.resize(indexCount);
	for (int i = 0; i < pointsCount; ++i)
	{
		index_buffer[i * 3]		= i + 1;
		index_buffer[i * 3 + 1] = 0;
		index_buffer[i * 3 + 2] = i + 2 > pointsCount ? 1 : i + 2;
	}

	/*modelMatrix = Matrix3D<float>(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);*/
	modelParameters = Vector3D<float>(0.0f, 0.0f, 0.0f); // initialisé en 0,0 avec un angle de 0

    // Definition of the Juce polygon
    createJucePolygon();
}


DrawablePolygon::DrawablePolygon(int64_t _Id, bpt _center, bpolygon& _bcontourPoints, Colour _fillColour) :
	DrawableArea(_Id, _center, _fillColour)
{
	contourPoints = _bcontourPoints;

	createJucePolygon();
}

int DrawablePolygon::GetVerticesCount()
{
	return 3*verticesCount;
}

int DrawablePolygon::GetIndexCount()
{
	return (int)index_buffer.size();
}

bool DrawablePolygon::hasVerticesChanged()
{
	return verticesChanged;
}

bool DrawablePolygon::hasPositionChanged()
{
	return positionChanged;
}

float DrawablePolygon::GetVertices(int idx)
{
	if (verticesCount > 0)
		return vertex_buffer[idx];
	return 0.0f;
}

std::vector<float> DrawablePolygon::GetOutline()
{
	if (outline_vertex_buffer.size() > 0)
		return outline_vertex_buffer;
	return std::vector<float>();
}

std::vector<int> DrawablePolygon::GetIndex()
{
	if (index_buffer.size() > 0)
		return index_buffer;
	return std::vector<int>();
}

Vector3D<float> DrawablePolygon::GetModelParameters()
{
	return modelParameters;
}

// Construction helpers
void DrawablePolygon::createJucePolygon(int width, int height)
{
    // Création de nouveaux points en coordonnées normalisées
	if (keepRatio)
		rescaleContourPoints(width, height);
    
    // Création du contour Juce, sachant que les coordonnées normalisées ont été modifiées
    // si le ratio était consevé
    contour.clear();
	contour.startNewSubPath((float)contourPoints.outer().at(0).get<0>(), (float)contourPoints.outer().at(0).get<1>());
	for (size_t i = 1; i<contourPoints.outer().size(); i++)
		contour.lineTo((float)contourPoints.outer().at(i).get<0>(), (float)contourPoints.outer().at(i).get<1>());
	contour.closeSubPath();
    
    contour.applyTransform(AffineTransform::scale((float)width, (float)height));
    
    // Puis Création du contour en tant que Polygone BOOST (points (en pixels) séparés)
    contourPointsInPixels.clear();
    boost::geometry::strategy::transform::scale_transformer<double, 2, 2> scale(width, height);
    boost::geometry::transform(contourPoints, contourPointsInPixels, scale);
    
}



void DrawablePolygon::rescaleContourPoints(int width, int height)
{
	// first calculate the distances and angles so we could apply recreate the same polygon, but with the new xScale and yScale
	float newCanvasRatio = (float)width / (float)height;
	float newXScale;
	float newYScale;
	if (newCanvasRatio > 1.0f) // ratio of an landscape-oriented window
	{
		newXScale = 1.0f / newCanvasRatio;
		newYScale = 1.0f;
	}
	else // ratio of an portrait-oriented window
	{
		newXScale = 1.0f;
		newYScale = 1.0f*newCanvasRatio;
	}
	
	boost::geometry::strategy::transform::translate_transformer<double, 2, 2> invTr(-center.get<0>(), -center.get<1>());
	boost::geometry::strategy::transform::translate_transformer<double, 2, 2> Tr(center.get<0>(), center.get<1>());

	bpolygon newContourPoints;
	boost::geometry::transform(contourPoints, newContourPoints, invTr);

	for (int i = 0; i < (int)newContourPoints.outer().size(); ++i)
		boost::geometry::multiply_point(newContourPoints.outer().at(i), bpt(newXScale / xScale, newYScale / yScale));

	contourPoints.clear();
	boost::geometry::transform(newContourPoints, contourPoints, Tr);

	xScale = newXScale;
	yScale = newYScale;

}

// Default destructor
DrawablePolygon::~DrawablePolygon()
{

}



void DrawablePolygon::setIsFilled(bool shouldBeFilled)
{
	isFilled = shouldBeFilled;
}



// Called by the parent component (which is a canvas)
void DrawablePolygon::Paint(Graphics& g)
{
	if (isFilled)
	{
		g.setColour(fillColour);
		g.setOpacity(GetAlpha());
		g.fillPath(contour);
	}
    
    g.setColour(contourColour);
    g.setOpacity(GetAlpha());
    g.strokePath(contour, PathStrokeType(contourWidth));
    
    // Parent's drawings on top of these ones
    DrawableArea::Paint(g);
}


void DrawablePolygon::CanvasResized(SceneCanvasComponent* _parentCanvas)
{
    DrawableArea::CanvasResized(_parentCanvas);
    
    // JUCE contour points in pixels
    createJucePolygon(parentCanvas->getWidth(), parentCanvas->getHeight());
    
    // Internal BOOST contour points in pixels
    contourPointsInPixels.clear();
    boost::geometry::strategy::transform::scale_transformer<double, 2, 2> scale(parentCanvas->getWidth(), parentCanvas->getHeight());
    boost::geometry::transform(contourPoints, contourPointsInPixels, scale);

	vertex_buffer[0] = (float)centerInPixels.get<0>();
	vertex_buffer[1] = (float)centerInPixels.get<1>();
	vertex_buffer[2] = 0.0f;
	
	for (int i = 0; i<contourPointsInPixels.outer().size()-1; i++)
	{
		vertex_buffer[3 + i * 3] = (float)contourPointsInPixels.outer().at(i).get<0>();//radius*cosf(currentAngle);
		vertex_buffer[3 + i * 3 + 1] = (float)contourPointsInPixels.outer().at(i).get<1>();
		vertex_buffer[3 + i * 3 + 2] = 0.0f;
	}

	bpolygon outlinePolygon,tmpPolygon;
	float dist = (float)boost::geometry::distance(centerInPixels, contourPointsInPixels.outer().at(0));
	float newDist = dist + contourWidth;
	float resizeFactor = newDist / dist;
	boost::geometry::strategy::transform::translate_transformer<double, 2, 2> tr(-centerInPixels.get<0>(), -centerInPixels.get<1>());
	boost::geometry::strategy::transform::scale_transformer<double, 2, 2> rescaler(resizeFactor, resizeFactor);
	boost::geometry::strategy::transform::translate_transformer<double, 2, 2> invTr(centerInPixels.get<0>(), centerInPixels.get<1>());

	boost::geometry::transform(contourPointsInPixels, outlinePolygon, tr);
	boost::geometry::transform(outlinePolygon, tmpPolygon, rescaler);
	boost::geometry::transform(tmpPolygon, outlinePolygon, invTr);

	for (int i = 0; i < (int)outlinePolygon.outer().size()-1; ++i)
	{
		outline_vertex_buffer[i * 3] = (float)outlinePolygon.outer().at(i).get<0>();//radius*cosf(currentAngle);
		outline_vertex_buffer[i * 3 + 1] = (float)outlinePolygon.outer().at(i).get<1>();
		outline_vertex_buffer[i * 3 + 2] = 0.0f;
	}
}


// = = = = = = = = = = XML import/export = = = = = = = = = =
std::shared_ptr<bptree::ptree> DrawablePolygon::GetTree()
{
    auto inheritedTree = DrawableArea::GetTree();
    bptree::ptree geomeTree; // LOL.
    // Lecture du polygone boost
    std::vector<bpt> const& points = contourPoints.outer(); // ring
    for (size_t i=0 ; i<points.size()-1 ; i++) // so we don't read the last point
    {
        bptree::ptree pointTree;
        pointTree.put("<xmlattr>.index", i);
        pointTree.put("<xmlattr>.x", points[i].get<0>());
        pointTree.put("<xmlattr>.y", points[i].get<1>());
        geomeTree.add_child("point", pointTree);
    }
    inheritedTree->put_child("geometry", geomeTree);
    return inheritedTree;
}

    

