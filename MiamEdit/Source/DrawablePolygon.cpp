//
//  DrawablePolygon.cpp
//  MiamEdit
//
//  Created by Gwendal Le Vaillant on 18/02/2016.
//
//

#include "DrawablePolygon.h"

using namespace Miam;


DrawablePolygon::DrawablePolygon(int64_t _Id) :
    DrawablePolygon(_Id, Point<double>(0.5f,0.5f), 3, 0.1f, Colours::darkgrey)
{
}


DrawablePolygon::DrawablePolygon(int64_t _Id, Point<double> _center, int pointsCount, float radius, Colour _fillColour, float _canvasRatio) :
    DrawableArea(_Id, _center, _fillColour)
{
    // Ratio managing : if <1 (height > width), we just inverse it
    float canvasRatio = _canvasRatio;
    if (canvasRatio < 1)
        canvasRatio = 1/canvasRatio;
    
    
    // Computation of the points
    float currentAngle;
    for (int i=0; i<pointsCount ; i++)
    {
        currentAngle = 2.0f*float_Pi*(float)(i)/(float)(pointsCount);
        contourPoints.push_back(Point<double>(center.x + radius/canvasRatio*cosf(currentAngle),
                                             center.y + radius*sinf(currentAngle)));
    }
    
    // Definition of the Juce polygon
    createJucePolygon();
}

DrawablePolygon::DrawablePolygon(int64_t _Id, Point<double> _center, std::vector<Point<double>>& _contourPoints, Colour _fillColour) :
    DrawableArea(_Id, _center, _fillColour)
{
    contourPoints = _contourPoints; // reminder : makes a elmt-by-elmt copy
    
    createJucePolygon();
}


// Construction helpers
void DrawablePolygon::createJucePolygon(int width, int height)
{
    contour.clear();
    contour.startNewSubPath(contourPoints[0].toFloat());
    for (int i=1; i<contourPoints.size() ; i++)
        contour.lineTo(contourPoints[i].toFloat());
    contour.closeSubPath();
    
    contour.applyTransform(AffineTransform::scale((float)width, (float)height));
}

// Default destructor
DrawablePolygon::~DrawablePolygon()
{
}






// Called by the parent component (which is a canvas)
void DrawablePolygon::Paint(Graphics& g)
{
    g.setColour(fillColour);
    g.setOpacity(fillOpacity);
    g.fillPath(contour);
    
    g.setColour(contourColour);
    g.strokePath(contour, PathStrokeType(contourWidth));
    
    // Parent's drawings on top of these ones
    DrawableArea::Paint(g);
}


void DrawablePolygon::CanvasResized(int width, int height)
{
    DrawableArea::CanvasResized(width, height);
    
    createJucePolygon(width, height);
}

