#include "stdafx.h" // Include the precompiled header

#include "OcctGlTools.h" // Include the header for this implementation file

// OCCT (Open CASCADE Technology) includes for working with OpenGL
#include <OpenGl_GraphicDriver.hxx> // Include for graphic driver in OpenGL
#include <OpenGl_FrameBuffer.hxx>   // Include for OpenGL frame buffer
#include <OpenGl_View.hxx>          // Include for OpenGL view
#include <OpenGl_Window.hxx>        // Include for OpenGL window

// ================================================================
// Function : GetGlContext
// Purpose  : Retrieve the OpenGL context associated with a given view.
// ================================================================
Handle(OpenGl_Context) OcctGlTools::GetGlContext(const Handle(V3d_View)& theView)
{
    // Downcast the generic V3d_View to an OpenGl_View
    Handle(OpenGl_View) aGlView = Handle(OpenGl_View)::DownCast(theView->View());

    // Retrieve and return the OpenGL context from the OpenGl_Window associated with the view
    return aGlView->GlWindow()->GetGlContext();
}
