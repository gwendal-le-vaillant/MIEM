/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2019 Gwendal Le Vaillant.
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

#ifndef DrawablePolygon_hpp
#define DrawablePolygon_hpp


#include "JuceHeader.h"
#include "DrawableArea.h"


namespace Miam {
    
    
    
    /// \brief A polygon that can be drawn on a SceneCanvasComponent.
    ///
    /// \remarks The VBOs will contain DrawableArea VBOs contents,
    /// then the surface of the shape
    /// then the contour of the shape
    class DrawablePolygon : public DrawableArea
    {
        
        
        // ========== ATTRIBUTES ==========
        protected :
        // Geometric data
		bpolygon contourPoints; ///< Normalized contour points coordinates (x,y in [0.0;1.0])
        Path contour; ///< Closed contour path (coordinates in pixels, relative to its canvas)
        
        
        
        // =============== SETTERS & GETTERS ===============
        public :
        
        // ----- VBO sizes -----
        // on ajoute la surface de la forme + les lignes extérieures.
        // Format de ce VBO (un chiffre représente l'indice d'élément)
        // +0 : centre
        // +1 à +numVerticesPolygon : polygone lui-même
        // +numPointsPolygon+2 à +numPointsPolygon+2 + numPointsPolygonContour
        virtual int GetVerticesBufferElementsCount() const override
        { return DrawableArea::GetVerticesBufferElementsCount()
            + numVerticesPolygon // les vertices qui vont définir la surface (32 + 1)
            + numPointsPolygonContour; } // 1 ring complet
        
        virtual int GetIndicesBufferElementsCount() const override
        { return DrawableArea::GetIndicesBufferElementsCount()
            + 3 * numPointsPolygon // Surface of the polygon
            + 3 * numPointsPolygonContour; } // remplissage du ring (1 triangle par point)
        
        
        
        // ========== METHODS ==========
        
        // - - - - - Construction/Destruction + polymorphic cloning - - - - -
        public :
        
        DrawablePolygon(bptree::ptree & areaTree);
        
        /// \brief Default constructor that builds a centered dark grey triangle
        ///
        /// \param See DrawableArea::id
        DrawablePolygon(int64_t _Id);
        /// \brief Construction of a regular polygon defined by the given parameters.
        ///
        /// \param _Id See DrawableArea::id
        /// \param _center See DrawableArea::center
        /// \param pointsCount Number of point of the polygon.
        /// \param radius Distance (normalized) between the center and each contour point.
        /// \param _fillColour See DrawableArea::fillColour
        /// \param _canvasRatio Current canvas ratio, optionnal but needed to draw a
        /// regular-looking polygon (16/9 by default)
        DrawablePolygon(int64_t _Id, bpt _center, int pointsCount, float radius, Colour _fillColour, float _canvasRatio = 1.77777777f);
        /// \brief Construction of a polygon defined
        /// Construction of any polygon defined by its center and contour points
        ///
        /// \param _Id See DrawableArea::id
        /// \param _center See DrawableArea::center
        /// \param _contourPoints See DrawablePolygon::contourPoints
        /// \param _fillColour See DrawableArea::fillColour
		DrawablePolygon(int64_t _Id, bpt _center, bpolygon& _bcontourPoints, Colour _fillColour);

        virtual std::shared_ptr<IDrawableArea> Clone() override
        {
            auto clone = std::make_shared<DrawablePolygon>(*this);
            clone->onCloned();
            return clone;
        }
        
        // (re)Construction / destruction helpers
        private :
        void createJucePolygon(int width = 160, int height = 90);
        /// \brief Resizes all buffers and inits the indices buffer only
        void initBuffers(); // should be called when the number of contour points changed
        
        /// \brief Ré-crée des contourPoints (en coordonnées normalisées) pour la nouvelle résolution
        /// de canevas indiquée (et donc pour un nouveau ratio)
		void rescaleContourPoints(int width, int height);
        
        void refreshExternalContourVerticesSubBuffer(int externalContourVertexElmtOffset, GLfloat posZ);

		protected :
		float xScale, yScale;
        
        public :
        /// \brief Destructor.
        virtual ~DrawablePolygon();
        
        
        //  - - - - - Display functions - - - - -
        public :
        virtual void Paint(Graphics& g) override;
        virtual void CanvasResized(SceneCanvasComponent* _parentCanvas) override;

        /// \brief Refreshes the vertices' coordinates and colours (not the indices that are
        /// supposed to remain constant)
		virtual void RefreshOpenGLBuffers() override;

        
        
        // - - - - - XML import/export - - - - -
        virtual std::string GetTypeAsString() const override {return "DrawablePolygon";}
        virtual std::shared_ptr<bptree::ptree> GetTree() override;

    };
}


#endif /* DrawablePolygon_hpp */
