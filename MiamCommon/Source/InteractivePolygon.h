/*
  ==============================================================================

    InteractivePolygon.h
    Created: 13 Apr 2016 3:56:35pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef INTERACTIVEPOLYGON_H_INCLUDED
#define INTERACTIVEPOLYGON_H_INCLUDED


#include "DrawablePolygon.h"
#include "InteractiveArea.h"

#include "SubTriangle.h"


namespace Miam
{
    
    /// \brief A polygonal area with interactive abilities.
    class InteractivePolygon : public DrawablePolygon, public InteractiveArea
    {
        
        
        // - - - - - Construction/Destruction + polymorphic cloning - - - - -
        public :
        
        InteractivePolygon(bptree::ptree & areaTree);
        
        /// \param _Id See DrawablePolygon::DrawablePolygon
        InteractivePolygon(int64_t _Id);
        /// \param _Id See DrawablePolygon::DrawablePolygon
        InteractivePolygon(int64_t _Id, bpt _center, int pointsCount, float radius, Colour _fillColour, float _canvasRatio);
        /// \param _Id See DrawablePolygon::DrawablePolygon
       	InteractivePolygon(int64_t _Id, bpt _center, bpolygon& _contourPoints, Colour _fillColour);
        
        virtual ~InteractivePolygon() {}
        
        virtual std::shared_ptr<IDrawableArea> Clone() override;
        protected :
        virtual void onCloned() override;
        public :
        
        // Contruction helpers
        private :
        void init();
 
        
        // Display functions
        public :
        // virtual void Paint(Graphics& g) override; // Nothing to paint over a normal area !
        virtual void CanvasResized(SceneCanvasComponent* _parentCanvas) override;
        
        
        
        
        // Updates
        protected :
        /// \brief Computation of the subdivisions of the polygon : it will be divided into
        /// triangles, each triangle being connected to the center.
        ///
        /// Computation of the angular values (positive, and from the center) taken by each
        /// triangle.
        void updateSubTriangles();
        
        
        // Interactions computing
        public :
        virtual bool HitTest(bpt T) const override;
        double ComputeInteractionWeight(bpt T) override;
        
        
        // ----- Private helpers (for interaction computing) -----
        protected :
        /// <summary>
        /// Finds the id of the interacted sub triangle.
        ///
        /// May be optimized, but we do not care... Complexity
        /// O(N) is okay, may be O(log(N)) but... N=1..10
        /// </summary>
        /// <param name="angle"></param>
        /// <returns></returns>
        SubTriangle& findSubTriangle(double angle);
        
        
        // Attributes
        protected :
        std::vector<SubTriangle> subTriangles;
        
		private :
			void computeSurface();
			//double area;
		public :
			double GetSurface() override;
        
    };
    
    
}






#endif  // INTERACTIVEPOLYGON_H_INCLUDED
