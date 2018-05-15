/*
  ==============================================================================

    EditableEllipse.h
    Created: 2 Feb 2017 2:17:32pm
    Author:  ayup1

  ==============================================================================
*/

#ifndef EDITABLEELLIPSE_H_INCLUDED
#define EDITABLEELLIPSE_H_INCLUDED

#include <vector>

#include "InteractiveEllipse.h"
#include "EditableArea.h"

namespace Miam
{
	class EditableEllipse : public InteractiveEllipse, public Miam::EditableArea
	{
		public :
        
        EditableEllipse(bptree::ptree & areaTree);
        
			EditableEllipse(int64_t _Id);
			EditableEllipse(int64_t _Id, bpt _center, double _r, Colour _fillColour, float _canvasRatio);
			EditableEllipse(int64_t _Id, bpt _center, double _a, double _b, Colour _fillColour, float _canvasRatio);

			virtual ~EditableEllipse() {/* DBG("ellipse deleted"); */}

        virtual std::shared_ptr<IDrawableArea> Clone() override
        {
            auto clone = std::make_shared<EditableEllipse>(*this);
            clone->onCloned();
            return clone;
        }

		private :
			void init();
			void graphicalInit();
			void behaviorInit();

		public :
			static const int dottedLineNparts = 20;
			static const int dottedLineVertexes = 4 * dottedLineNparts;
			static const int dottedLineIndices = 6 * dottedLineNparts;

			GLfloat g_vertex_dotted_line[3 * dottedLineVertexes];
			GLuint g_indices_dotted_line[dottedLineIndices];

			GLfloat g_vertex_circle[3 * numVerticesCircle];
			unsigned int circleIndices[3 * numPointCircle];
			int GetOpaqueVerticesCount() override {					// points du contour					manipulationLine	manipulationPoint
				return DrawableEllipse::GetOpaqueVerticesCount() + (numPointsPolygon * numVerticesCircle) + dottedLineVertexes + numVerticesRing;
			}
			int GetOpaqueColourCount() override { return DrawableEllipse::GetOpaqueColourCount() + 4 * ((numPointsPolygon * numVerticesCircle) + dottedLineVertexes + numVerticesRing); }
			int GetIndexCount() override { return DrawableEllipse::GetIndexCount() + numPointsPolygon * (3 * numPointCircle) + dottedLineIndices + (3 * numVerticesRing); }
			virtual void Paint(Graphics& g) override;
			virtual void CanvasResized(SceneCanvasComponent* _parentCanvas) override;
			virtual void fillOpenGLBuffers() override;
		private:
			void computeManipulationPoint();

			// ----- Setters and Getters -----
		public:
			void SetActive(bool activate) override;
			double getRadius();
			void computeManipulationLine(float Ox, float Oy, float Mx, float My, float width, float height);
		public :
			
			Miam::AreaEventType TryBeginPointMove(const Point<double>& hitPoint) override;
			Miam::AreaEventType TryMovePoint(const Point<double>& newLocation) override;
			Miam::AreaEventType EndPointMove() override;
			void Translate(const Point<double>& translation) override;
			void setCenterPosition(bpt newCenter);
			
			bool SizeChanged(double sizeFactor, bool minSize);
			void Rotate(double Radian);
			void updateContourPoints();

		protected:
			void recreateNormalizedPoints() override;

		private:
			bool isNewContourPointValid(const Point<double>& newLocation);
			bool isNewCenterValid(const Point<double>& newLocation);
        
        
        // - - - - - XML import/export - - - - -
        /// \returns "Ellipse" even if it is actually a circle.
        virtual std::string GetTypeAsString() const override {return "EditableEllipse";};
        // Pas d'override du GetTree : aucun paramètre spécifique pour cette classe
        //virtual std::shared_ptr<bptree::ptree> GetTree() override;
	};
}



#endif  // EDITABLEELLIPSE_H_INCLUDED
