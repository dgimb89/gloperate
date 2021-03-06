/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <glbinding/gl/gl.h>
#include <globjects/logging.h>
#include <gloperate-osg/OsgPainter.h>
#include <gloperate/capabilities/ViewportCapability.h>
#include <gloperate/capabilities/TargetFramebufferCapability.h>
#include <gloperate/capabilities/InputCapability.h>
#include <gloperate/capabilities/VirtualTimeCapability.h>


using namespace gl;
using namespace gloperate;
namespace gloperate_osg
{


/**
*  @brief
*    Constructor
*/
OsgPainter::OsgPainter(ResourceManager & resourceManager)
: Painter(resourceManager)
, m_viewer(nullptr)
, m_embedded(nullptr)
, m_scene(nullptr)
, m_viewportX(0)
, m_viewportY(0)
, m_viewportWidth(0)
, m_viewportHeight(0)
, m_viewportCapability(new gloperate::ViewportCapability)
, m_targetFramebufferCapability(new gloperate::TargetFramebufferCapability)
, m_inputCapability(new gloperate::InputCapability)
, m_virtualTimeCapability(new gloperate::VirtualTimeCapability)
{
    // Register capabilities
    addCapability(m_viewportCapability);
    addCapability(m_targetFramebufferCapability);
    addCapability(m_inputCapability);
    addCapability(m_virtualTimeCapability);
}

/**
*  @brief
*    Destructor
*/
OsgPainter::~OsgPainter()
{
    osg_cleanup();
}

/**
*  @brief
*    Get OSG viewer
*/
osgViewer::Viewer * OsgPainter::viewer() const
{
    return m_viewer;
}

/**
*  @brief
*    Get OSG scene
*/
osg::Node * OsgPainter::scene() const
{
    return m_scene;
}

/**
*  @brief
*    Set OSG scene
*/
void OsgPainter::setScene(osg::Node * scene)
{
    osg_setScene(scene);
}

void OsgPainter::onInitialize()
{
    osg_onInitialize();
}

void OsgPainter::onPaint()
{
    // Get framebuffer to render into
    globjects::Framebuffer * fbo = m_targetFramebufferCapability->framebuffer();
    if (!fbo) {
        fbo = globjects::Framebuffer::defaultFBO();
    }

    // Bind framebuffer
    fbo->bind(GL_FRAMEBUFFER);

    // Draw osg scene
    osg_onPaint();
}


} // namespace gloperate_osg
