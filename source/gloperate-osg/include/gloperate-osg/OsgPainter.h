/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


// Do not include any osg headers OR gloperate headers that pull in globjects/glbinding,
// because that would break the separation between glbinding and gl.h.
// Use only pointers to OSG objects, call ref/unref to maintain memory management,
// and separate all calls to osg in osg_ functions implemented inside OsgPainter_osg.cpp.


#include <gloperate/Painter.h>
#include <gloperate-osg/gloperate-osg_api.h>


namespace osgViewer {
    class Viewer;
    class GraphicsWindowEmbedded;
}
namespace osg {
    class Node;
}
namespace gloperate {
    class ViewportCapability;
    class TargetFramebufferCapability;
    class InputCapability;
    class VirtualTimeCapability;
}


namespace gloperate_osg
{


/**
*  @brief
*    Painter that renders an OSG scene
*/
class GLOPERATE_OSG_API OsgPainter : public gloperate::Painter
{


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] resourceManager
    *    Resource manager, e.g., to load and save textures
    */
    OsgPainter(gloperate::ResourceManager & resourceManager);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~OsgPainter();

    /**
    *  @brief
    *    Get OSG viewer
    *
    *  @return
    *    OSG viewer
    */
    osgViewer::Viewer * viewer() const;

    /**
    *  @brief
    *    Get OSG scene
    *
    *  @return
    *    OSG scene
    */
    osg::Node * scene() const;

    /**
    *  @brief
    *    Set OSG scene
    *
    *  @param[in] scene
    *    OSG scene
    */
    void setScene(osg::Node * scene);


protected:
    virtual void onInitialize() override;
    virtual void onPaint() override;


protected:
    // The following functions are actually using OSG code, so they are not compatible with
    // globjects/glbinding include. Therefore, they are separately implemented in OsgPainter_osg.cpp
    void osg_setScene(osg::Node * scene);
    void osg_onInitialize();
    void osg_onPaint();
    void osg_cleanup();


protected:
    osgViewer::Viewer                 * m_viewer;          /**< OSG viewer */
    osgViewer::GraphicsWindowEmbedded * m_embedded;        /**< Interface that acts like a window to OSG */
    osg::Node                         * m_scene;           /**< The displayed scene */
    int                                 m_viewportX;       /**< Viewport position x */
    int                                 m_viewportY;       /**< Viewport position y */
    int                                 m_viewportWidth;   /**< Viewport width */
    int                                 m_viewportHeight;  /**< Viewport height */

    // Capabilities
    gloperate::ViewportCapability          * m_viewportCapability;
    gloperate::TargetFramebufferCapability * m_targetFramebufferCapability;
    gloperate::InputCapability             * m_inputCapability;
    gloperate::VirtualTimeCapability       * m_virtualTimeCapability;


};


} // namespace gloperate_osg
