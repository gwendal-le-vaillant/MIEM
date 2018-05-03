/*
  ==============================================================================

    DrawableArea.h
    Created: 26 Mar 2016 12:04:39pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef DRAWABLEAREA_H_INCLUDED
#define DRAWABLEAREA_H_INCLUDED

#include "JuceHeader.h"

#include "IDrawableArea.h"


// Simple declaration for a pointer
class SceneCanvasComponent;


namespace Miam
{
    
    /// \brief Abstract class from which all Miam areas will be derived.
    ///
    /// Common graphics managing features for all possible areas.
    class DrawableArea : public virtual IDrawableArea
    {
        
        // =============== ATTRIBUTS ===============
        
        // General data
        protected :
        uint64 Id; ///< Unique ID
        SceneCanvasComponent* parentCanvas; ///< Parent that draws this(needed to get display properties)
        
        // Geometric data
        protected :
        bpt center; ///< Normalized center coordinates (x, y in [0.0,1.0])
        bpt centerInPixels; ///< Center coordinates in pixels (relative to a canvas)
        bool displayCenter;
        bpolygon contourPointsInPixels; ///< Caracteristic points (coordinates in pixels) along the contour of the area (not necessarily drawn)
        
        // Display data
        protected :
        
        double renderingScale;
        /// \brief Pour tout ce qui est en cache, on fait de base du rendu
        /// en taille x2 mais aussi en taille x1 maintenant.
        // Ensuite, si besoin, on fait un resize au moment du rendu final OpenGL
        /// (c'est Juce qui se débrouille avec...)
        // const double baseRenderingScale = 2.0; // plus utilisé...
        
        Colour fillColour; ///< Solid colour for filling the area (opacity should be 0xFF)
        float fillOpacity; ///< Opacity applied to the solid fill colour (in [0.0,1.0])
        OpacityMode opacityMode;
        
        /// \brief The lowest opacity of a displayed area
        static const uint8 lowFillOpacityUint8 = 40;
        virtual float getLowFillOpacity() const override { return (float)(lowFillOpacityUint8) / 255.0f; }
        
        Colour contourColour; ///< Solid color of of the external shape of the 2D area.
        float contourWidth; ///< Width (in pixels) of of the external shape of the 2D area.
        float centerContourWidth; ///< Width (in pixels) of the center circle drawing.
        
        int centerCircleRadius; ///< Radius (in pixels) of the center circle drawing.
        
        String name;
        bool isNameVisible;
        // Images bêtement comme ça pour l'instant...
        Image nameImage;
        Image nameImage2;
        static const int nameWidth = 120; // pixels
        static const int nameHeight = 15; // pixels
        
        bool keepRatio;
        
		bool verticesChanged = false;
		bool positionChanged = false;
		//float vertex_buffer[33*3]; // taille maximum : 32 points + centre

		static const int numPointsPolygon = 32;
		static const int numPointsRing = 32;
		static const int numPointCircle = 32;

		static const int numVerticesPolygon = numPointsPolygon + 1;
		static const int numVerticesRing = 2 * numPointsRing;
		static const int numVerticesCircle = numPointCircle + 1;

		
		int opaque_vertex_buffer_size = 3 * numVerticesRing;//3 * numPointsPolygon + 3 * numVerticesRing; // par défaut : contour + centre
		int opaque_index_buffer_size = 3 * numVerticesRing;//3 * 2 * numPointsPolygon + 3 * numVerticesRing;
		int opaque_color_buffer_size = 4 * numVerticesRing;

		GLfloat g_vertex_ring[3 * numVerticesRing];
		unsigned int ringIndices[3 * numVerticesRing];

		std::vector<float> opaque_vertex_buffer;
		std::vector<int> opaque_index_buffer;
		std::vector<float> opaque_color_buffer;


		Vector3D<float> modelParameters; // x, y, theta
        
        // =============== SETTERS & GETTERS ===============
        public :
        /// \returns Unique ID of the area
        virtual int64_t GetId() const override {return Id;}
        /// \param _Id Unique ID of the area
        virtual void SetId(int64_t _Id) override {Id = _Id;}
        /// \returns See DrawableArea::fillColour
        virtual Colour GetFillColour() const override {return fillColour;}
        /// \param _fillColour See DrawableArea::fillColour
        virtual void SetFillColour(Colour newColour) override;
        /// \param _fillOpacity See DrawableArea::fillOpacity
        virtual void SetAlpha(float newAlpha) override;
        virtual float GetAlpha() const override;
        
        
        virtual void SetOpacityMode(OpacityMode opacityMode_) override;
        virtual OpacityMode GetOpacityMode() const override {return opacityMode;}
        
        /// \brief Sets the name that could be displayed on screen next to the center
        virtual void SetName(String newName) override;
        
        void SetNameVisible(bool isVisible) {isNameVisible = isVisible;}
        
        void KeepRatio(bool _keepRatio);
        
        /// \brief Pour régler les problèmes d'écrans type "rétina"
        virtual void SetRenderingScale(double renderingScale_) override;
        
		bool hasVerticesChanged() override;
        
        // =============== MÉTHODES ===============

        
        public :
        
        /// \Brief Contruction from the unique ID, center, and fill colour that must
        /// be found in the given <area> sub-tree inner content.
        DrawableArea(bptree::ptree& areaTree);
        
        /// \brief Minimal constructor when building a new area
        ///
        /// \param _Id Unique ID of the area
        /// \param _center Normalized center coordinates (x, y in [0.0,1.0])
        /// \param _fillColour Solid colour for filling the area (opacity should be 0xFF)
		DrawableArea(int64_t _Id, bpt _center, Colour _fillColour);
        
        /// \brief Virtual destructor.
        virtual ~DrawableArea();
        
        // Clonage : rien de spé à faire ici...
        protected :
        virtual void onCloned() override {}
        
        private :
        void init();
		void ComputeRing(int numPoints);
        void resetImages();
        void renderCachedNameImages();
        public :
        
			int GetOpaqueVerticesCount() override { return numVerticesRing; }
			float GetOpaqueVertices(int idx) override { return opaque_vertex_buffer[idx]; }
			int GetIndexCount() override { return  3 * numVerticesRing; }
			int GetIndex(int idx) override { return opaque_index_buffer[idx]; }
			int GetOpaqueColourCount() { return 4 * numVerticesRing; }
			float GetOpaqueColour(int idx) { return opaque_color_buffer[idx]; }
        
        virtual void Paint(Graphics& g) override;
        virtual void CanvasResized(SceneCanvasComponent* _parentCanvas) override;
		virtual void fillOpenGLBuffers() override;

        // - - - - - XML import/export - - - - -
        virtual std::shared_ptr<bptree::ptree> GetTree() override;
        
        
    };
    
}



#endif  // DRAWABLEAREA_H_INCLUDED
