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
        
        bool keepRatio;
        
        
        // - - - - - - TEST OPENGL MULTITHREADÉ - - - - - -
        // - - - - - - TEST OPENGL MULTITHREADÉ - - - - - -
        Image nameImage;
        // - - - - - - TEST OPENGL MULTITHREADÉ - - - - - -
        // - - - - - - TEST OPENGL MULTITHREADÉ - - - - - -
        
        
        
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
        
        /// \brief Sets the name that could be displayed on screen next to the center
        virtual void SetName(String newName) override;
        
        void SetNameVisible(bool isVisible) {isNameVisible = isVisible;}
        
        void KeepRatio(bool _keepRatio);
        
        
        
        
        
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
        
        private :
        void init();
        public :
        
        
        virtual void Paint(Graphics& g) override;
        virtual void CanvasResized(SceneCanvasComponent* _parentCanvas) override;

        // - - - - - XML import/export - - - - -
        virtual std::shared_ptr<bptree::ptree> GetTree() override;
        
        
    };
    
}



#endif  // DRAWABLEAREA_H_INCLUDED
