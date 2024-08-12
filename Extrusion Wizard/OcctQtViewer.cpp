// Precompiled header
#include "stdafx.h"

#ifdef _WIN32
#include <windows.h> // Include for Windows-specific functionality
#endif

// OCCT includes
#include <OpenGl_Context.hxx> // Include for OpenGL context

// Local includes
#include "OcctQtViewer.h" // Include the header for this implementation file
#include "OcctGlTools.h"  // Include the header for OpenGL tools

// Disable certain compiler warnings specific to OCCT
#include <Standard_WarningsDisable.hxx>

// Qt includes for GUI components and functionalities
#include <QApplication> // Main application class for Qt applications
#include <QMessageBox>  // Class to display message boxes
#include <QMouseEvent>  // Class for handling mouse events

// Restore the compiler warnings for OCCT
#include <Standard_WarningsRestore.hxx>

// OCCT includes for creating shapes and context management
#include <AIS_Shape.hxx>               // Include for AIS shape
#include <AIS_ViewCube.hxx>            // Include for AIS view cube
#include <Aspect_DisplayConnection.hxx> // Include for display connection
#include <Aspect_NeutralWindow.hxx>     // Include for neutral window
#include <BRepPrimAPI_MakeBox.hxx>      // Include for creating a box shape
#include <Message.hxx>                 // Include for message handling
#include <OpenGl_GraphicDriver.hxx>    // Include for graphic driver in OpenGL
#include <OpenGl_FrameBuffer.hxx>      // Include for OpenGL frame buffer
#include <Quantity_Color.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <AIS_Line.hxx>
#include <Geom_CartesianPoint.hxx>
#include <Geom_Line.hxx>



// Anonymous namespace for utility functions
namespace
{
    //! Map Qt buttons bitmask to virtual keys.
    Aspect_VKeyMouse qtMouseButtons2VKeys(Qt::MouseButtons theButtons)
    {
        Aspect_VKeyMouse aButtons = Aspect_VKeyMouse_NONE;
        if ((theButtons & Qt::LeftButton) != 0)
        {
            aButtons |= Aspect_VKeyMouse_LeftButton;
        }
        if ((theButtons & Qt::MiddleButton) != 0)
        {
            aButtons |= Aspect_VKeyMouse_MiddleButton;
        }
        if ((theButtons & Qt::RightButton) != 0)
        {
            aButtons |= Aspect_VKeyMouse_RightButton;
        }
        return aButtons;
    }

    //! Map Qt mouse modifiers bitmask to virtual keys.
    Aspect_VKeyFlags qtMouseModifiers2VKeys(Qt::KeyboardModifiers theModifiers)
    {
        Aspect_VKeyFlags aFlags = Aspect_VKeyFlags_NONE;
        if ((theModifiers & Qt::ShiftModifier) != 0)
        {
            aFlags |= Aspect_VKeyFlags_SHIFT;
        }
        if ((theModifiers & Qt::ControlModifier) != 0)
        {
            aFlags |= Aspect_VKeyFlags_CTRL;
        }
        if ((theModifiers & Qt::AltModifier) != 0)
        {
            aFlags |= Aspect_VKeyFlags_ALT;
        }
        return aFlags;
    }

    //! Map Qt key to virtual key.
    Aspect_VKey qtKey2VKey(int theKey)
    {
        switch (theKey)
        {
        case 1060: // Russian 'A'
        case Qt::Key_A: return Aspect_VKey_A;
        case 1048: // Russian 'B'
        case Qt::Key_B: return Aspect_VKey_B;
        case 1057: // Russian 'C'
        case Qt::Key_C: return Aspect_VKey_C;
        case 1042: // Russian 'D'
        case Qt::Key_D: return Aspect_VKey_D;
        case 1059: // Russian 'E'
        case Qt::Key_E: return Aspect_VKey_E;
        case 1040: // Russian 'F'
        case Qt::Key_F: return Aspect_VKey_F;
        case Qt::Key_G: return Aspect_VKey_G;
        case Qt::Key_H: return Aspect_VKey_H;
        case Qt::Key_I: return Aspect_VKey_I;
        case Qt::Key_J: return Aspect_VKey_J;
        case Qt::Key_K: return Aspect_VKey_K;
        case 1044: // Russian 'L'
        case Qt::Key_L: return Aspect_VKey_L;
        case Qt::Key_M: return Aspect_VKey_M;
        case Qt::Key_N: return Aspect_VKey_N;
        case Qt::Key_O: return Aspect_VKey_O;
        case Qt::Key_P: return Aspect_VKey_P;
        case 1049: // Russian 'Q'
        case Qt::Key_Q: return Aspect_VKey_Q;
        case 1050: // Russian 'R'
        case Qt::Key_R: return Aspect_VKey_R;
        case 1067: // Russian 'S'
        case Qt::Key_S: return Aspect_VKey_S;
        case 1045: // Russian 'T'
        case Qt::Key_T: return Aspect_VKey_T;
        case Qt::Key_U: return Aspect_VKey_U;
        case 1052: // Russian 'V'
        case Qt::Key_V: return Aspect_VKey_V;
        case 1062: // Russian 'W'
        case Qt::Key_W: return Aspect_VKey_W;
        case 1063: // Russian 'X'
        case Qt::Key_X: return Aspect_VKey_X;
        case Qt::Key_Y: return Aspect_VKey_Y;
        case 1071: // Russian 'Z'
        case Qt::Key_Z: return Aspect_VKey_Z;
            //
        case Qt::Key_0: return Aspect_VKey_0;
        case Qt::Key_1: return Aspect_VKey_1;
        case Qt::Key_2: return Aspect_VKey_2;
        case Qt::Key_3: return Aspect_VKey_3;
        case Qt::Key_4: return Aspect_VKey_4;
        case Qt::Key_5: return Aspect_VKey_5;
        case Qt::Key_6: return Aspect_VKey_6;
        case Qt::Key_7: return Aspect_VKey_7;
        case Qt::Key_8: return Aspect_VKey_8;
        case Qt::Key_9: return Aspect_VKey_9;
            //
        case Qt::Key_F1:        return Aspect_VKey_F1;
        case Qt::Key_F2:        return Aspect_VKey_F2;
        case Qt::Key_F3:        return Aspect_VKey_F3;
        case Qt::Key_F4:        return Aspect_VKey_F4;
        case Qt::Key_F5:        return Aspect_VKey_F5;
        case Qt::Key_F6:        return Aspect_VKey_F6;
        case Qt::Key_F7:        return Aspect_VKey_F7;
        case Qt::Key_F8:        return Aspect_VKey_F8;
        case Qt::Key_F9:        return Aspect_VKey_F9;
        case Qt::Key_F10:       return Aspect_VKey_F10;
        case Qt::Key_F11:       return Aspect_VKey_F11;
        case Qt::Key_F12:       return Aspect_VKey_F12;
            //
        case Qt::Key_Up:        return Aspect_VKey_Up;
        case Qt::Key_Left:      return Aspect_VKey_Left;
        case Qt::Key_Right:     return Aspect_VKey_Right;
        case Qt::Key_Down:      return Aspect_VKey_Down;
        case Qt::Key_Plus:      return Aspect_VKey_Plus;
        case Qt::Key_Minus:     return Aspect_VKey_Minus;
        case Qt::Key_Equal:     return Aspect_VKey_Equal;
        case Qt::Key_PageDown:  return Aspect_VKey_PageDown;
        case Qt::Key_PageUp:    return Aspect_VKey_PageUp;
        case Qt::Key_Home:      return Aspect_VKey_Home;
        case Qt::Key_End:       return Aspect_VKey_End;
        case Qt::Key_Escape:    return Aspect_VKey_Escape;
        case Qt::Key_Back:      return Aspect_VKey_Back;
        case Qt::Key_Enter:     return Aspect_VKey_Enter;
        case Qt::Key_Backspace: return Aspect_VKey_Backspace;
        case Qt::Key_Space:     return Aspect_VKey_Space;
        case Qt::Key_Delete:    return Aspect_VKey_Delete;
        case Qt::Key_Tab:       return Aspect_VKey_Tab;
        case 1025: // Russian '~'
        case Qt::Key_QuoteLeft: return Aspect_VKey_Tilde;
            //
        case Qt::Key_Shift:     return Aspect_VKey_Shift;
        case Qt::Key_Control:   return Aspect_VKey_Control;
        case Qt::Key_Alt:       return Aspect_VKey_Alt;
        case Qt::Key_Menu:      return Aspect_VKey_Menu;
        case Qt::Key_Meta:      return Aspect_VKey_Meta;
        default:                return Aspect_VKey_UNKNOWN;
        }
    }
}

//! OpenGL FBO subclass for wrapping FBO created by Qt using GL_RGBA8 texture format instead of GL_SRGB8_ALPHA8.
//! This FBO is set to OpenGl_Context::SetDefaultFrameBuffer() as a final target.
//! Subclass calls OpenGl_Context::SetFrameBufferSRGB() with sRGB=false flag,
//! which asks OCCT to disable GL_FRAMEBUFFER_SRGB and apply sRGB gamma correction manually.
class OcctQtFrameBuffer : public OpenGl_FrameBuffer
{
    DEFINE_STANDARD_RTTI_INLINE(OcctQtFrameBuffer, OpenGl_FrameBuffer)
public:
    //! Empty constructor.
    OcctQtFrameBuffer() {}

    //! Make this FBO active in context.
    virtual void BindBuffer(const Handle(OpenGl_Context)& theGlCtx) override
    {
        OpenGl_FrameBuffer::BindBuffer(theGlCtx);
        theGlCtx->SetFrameBufferSRGB(true, false);
    }

    //! Make this FBO as drawing target in context.
    virtual void BindDrawBuffer(const Handle(OpenGl_Context)& theGlCtx) override
    {
        OpenGl_FrameBuffer::BindDrawBuffer(theGlCtx);
        theGlCtx->SetFrameBufferSRGB(true, false);
    }

    //! Make this FBO as reading source in context.
    virtual void BindReadBuffer(const Handle(OpenGl_Context)& theGlCtx) override
    {
        OpenGl_FrameBuffer::BindReadBuffer(theGlCtx);
    }
};

// ================================================================
// Function : OcctQtViewer
// Purpose  : Constructor for the OCCT Qt Viewer class
// ================================================================
OcctQtViewer::OcctQtViewer(QWidget* theParent)
    : QOpenGLWidget(theParent), // Initialize the base class QOpenGLWidget with the parent widget
    myIsCoreProfile(true),     // Initialize the core profile flag to true
    initWindowWidth(720),      // Initialize the initial window width
    initWindowHeight(480)     // Initialize the initial window height
{
    // Create a display connection for OCCT
    Handle(Aspect_DisplayConnection) aDisp = new Aspect_DisplayConnection();

    // Create an OpenGL graphic driver
    Handle(OpenGl_GraphicDriver) aDriver = new OpenGl_GraphicDriver(aDisp, false);

    // Let QOpenGLWidget manage buffer swap
    aDriver->ChangeOptions().buffersNoSwap = true;

    // Do not write into the alpha channel
    aDriver->ChangeOptions().buffersOpaqueAlpha = true;

    // Always use offscreen frame buffers
    aDriver->ChangeOptions().useSystemBuffer = false;

    // Create the OCCT viewer
    myViewer = new V3d_Viewer(aDriver);
    myViewer->SetDefaultBackgroundColor(Quantity_NOC_BLACK); // Set the default background color to black
    myViewer->SetDefaultLights();                           // Set the default lights
    myViewer->SetLightOn();                                 // Turn the lights on
    myViewer->ActivateGrid(Aspect_GT_Rectangular, Aspect_GDM_Lines); // Activate a rectangular grid
    
    
    


    // Create the AIS (Application Interactive Services) context
    myContext = new AIS_InteractiveContext(myViewer);
    AddXAxisZeroLine(myContext);                             // Add a zero line for the X axis
    AddYAxisZeroLine(myContext);                             // Add a zero line for the X axis
    AddZAxisZeroLine(myContext);                             // Add a zero line for the X axis


    // Create a view cube for orientation and navigation
    myViewCube = new AIS_ViewCube();
    myViewCube->SetViewAnimation(myViewAnimation);         // Set view animation for the view cube
    myViewCube->SetFixedAnimationLoop(false);              // Disable fixed animation loop
    myViewCube->SetAutoStartAnimation(true);               // Enable auto-start animation
    myViewCube->TransformPersistence()->SetOffset2d(Graphic3d_Vec2i(75, 75)); // Set 2D offset for the view cube
    myViewCube->SetSize(50);                              // Set the size of the view cube

    // Note: The window will be created later within initializeGL() callback!
    myView = myViewer->CreateView();
    myView->SetImmediateUpdate(false); // Disable immediate update
#ifndef __APPLE__
    myView->ChangeRenderingParams().NbMsaaSamples = 4; // Set multi-sample anti-aliasing to 4 samples (affects performance)
#endif
    myView->ChangeRenderingParams().ToShowStats = true; // Enable displaying statistics
    myView->ChangeRenderingParams().CollectedStats = (Graphic3d_RenderingParams::PerfCounters)
        (Graphic3d_RenderingParams::PerfCounters_FrameRate
            | Graphic3d_RenderingParams::PerfCounters_Triangles); // Collect frame rate and triangle count statistics

    // Qt widget setup
    setMouseTracking(true);               // Enable mouse tracking
    setBackgroundRole(QPalette::NoRole);  // Set background role to NoRole
    setFocusPolicy(Qt::StrongFocus);      // Set focus policy to handle context menu events from the keyboard
    setUpdatesEnabled(true);              // Enable updates
    setUpdateBehavior(QOpenGLWidget::NoPartialUpdate); // Set update behavior to no partial update

    // OpenGL setup managed by Qt
    QSurfaceFormat aGlFormat;
    aGlFormat.setDepthBufferSize(24);      // Set depth buffer size to 24 bits
    aGlFormat.setStencilBufferSize(8);     // Set stencil buffer size to 8 bits
    aDriver->ChangeOptions().contextDebug = aGlFormat.testOption(QSurfaceFormat::DebugContext); // Set context debug option

    // Set initial viewport size
    // TODO: Look into this interaction with QT widgets. It doesn't seem like it is working correctly on my version
    QSize widgetSize = size();
    resizeGL(widgetSize.width(), widgetSize.height());

    if (myIsCoreProfile)
    {
        aGlFormat.setVersion(4, 5); // Set OpenGL version to 4.5 if core profile is used
    }
    aGlFormat.setProfile(myIsCoreProfile ? QSurfaceFormat::CoreProfile : QSurfaceFormat::CompatibilityProfile); // Set OpenGL profile

    setFormat(aGlFormat); // Apply the surface format

#if defined(_WIN32)
    // Never use ANGLE on Windows, since OCCT 3D Viewer does not expect this
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
#endif
}

// ================================================================
// Function : ~OcctQtViewer
// Purpose  : Destructor for the OCCT Qt Viewer class
// ================================================================
OcctQtViewer::~OcctQtViewer()
{
    // Hold on to the X11 display connection until making another connection active with glXMakeCurrent()
    // to work around sudden crash in QOpenGLWidget destructor
    Handle(Aspect_DisplayConnection) aDisp = myViewer->Driver()->GetDisplayConnection();

    // Release OCCT viewer resources
    myContext->RemoveAll(false);
    myContext.Nullify();
    myView->Remove();
    myView.Nullify();
    myViewer.Nullify();

    // Make active the OpenGL context created by Qt
    makeCurrent();
    aDisp.Nullify();
}

// ================================================================
// Function : dumpGlInfo
// Purpose  : Dump OpenGL information to a string for diagnostics
// ================================================================
void OcctQtViewer::dumpGlInfo(bool theIsBasic, bool theToPrint)
{
    TColStd_IndexedDataMapOfStringString aGlCapsDict;
    myView->DiagnosticInformation(aGlCapsDict, theIsBasic ? Graphic3d_DiagnosticInfo_Basic : Graphic3d_DiagnosticInfo_Complete);
    TCollection_AsciiString anInfo;
    for (TColStd_IndexedDataMapOfStringString::Iterator aValueIter(aGlCapsDict); aValueIter.More(); aValueIter.Next())
    {
        if (!aValueIter.Value().IsEmpty())
        {
            if (!anInfo.IsEmpty())
            {
                anInfo += "\n";
            }
            anInfo += aValueIter.Key() + ": " + aValueIter.Value();
        }
    }

    if (theToPrint)
    {
        Message::SendInfo(anInfo);
    }
    myGlInfo = QString::fromUtf8(anInfo.ToCString());
}

// ================================================================
// Function : initializeGL
// Purpose  : Initialize OpenGL settings and create the OCCT view
// ================================================================
void OcctQtViewer::initializeGL()
{
    const QRect aRect = rect(); // Get the widget's rectangle
    const Graphic3d_Vec2i aViewSize(aRect.right() - aRect.left(), aRect.bottom() - aRect.top()); // Calculate the view size

    // Initialize the OpenGL context
    Handle(OpenGl_Context) aGlCtx = new OpenGl_Context();
    if (!aGlCtx->Init(myIsCoreProfile))
    {
        Message::SendFail() << "Error: OpenGl_Context is unable to wrap OpenGL context";
        QMessageBox::critical(0, "Failure", "OpenGl_Context is unable to wrap OpenGL context");
        QApplication::exit(1);
        return;
    }

    // Create and set up the OCCT window
    Handle(Aspect_NeutralWindow) aWindow = Handle(Aspect_NeutralWindow)::DownCast(myView->Window());
    if (!aWindow.IsNull())
    {
        aWindow->SetSize(aViewSize.x(), aViewSize.y());
        myView->SetWindow(aWindow, aGlCtx->RenderingContext());
        dumpGlInfo(true, true);
    }
    else
    {
        aWindow = new Aspect_NeutralWindow();
        aWindow->SetVirtual(true);

        Aspect_Drawable aNativeWin = (Aspect_Drawable)winId();
#ifdef _WIN32
        // Set the native window handle for Windows
        HDC aWglDevCtx = wglGetCurrentDC();
        HWND aWglWin = WindowFromDC(aWglDevCtx);
        aNativeWin = (Aspect_Drawable)aWglWin;
#endif
        aWindow->SetNativeHandle(aNativeWin);
        aWindow->SetSize(aViewSize.x(), aViewSize.y());
        myView->SetWindow(aWindow, aGlCtx->RenderingContext());
        dumpGlInfo(true, true);

        // Display the view cube in the context
        myContext->Display(myViewCube, 0, 0, false);
    }

    //{
    //    // Create and display a dummy shape for testing
    //    TopoDS_Shape aBox = BRepPrimAPI_MakeBox(100.0, 50.0, 90.0).Shape();
    //    Handle(AIS_Shape) aShape = new AIS_Shape(aBox);
    //    myContext->Display(aShape, AIS_Shaded, 0, false);
    //}
}

// ================================================================
// Function : closeEvent
// Purpose  : Handle the window close event
// ================================================================
void OcctQtViewer::closeEvent(QCloseEvent* theEvent)
{
    theEvent->accept(); // Accept the close event
}

// ================================================================
// Function : keyPressEvent
// Purpose  : Handle the key press event
// ================================================================
void OcctQtViewer::keyPressEvent(QKeyEvent* theEvent)
{
    Aspect_VKey aKey = qtKey2VKey(theEvent->key()); // Map the Qt key to a virtual key
    switch (aKey)
    {
    case Aspect_VKey_Escape:
    {
        QApplication::exit(); // Exit the application if Escape is pressed
        return;
    }
    case Aspect_VKey_F:
    {
        myView->FitAll(0.01, false); // Fit all objects in the view if 'F' is pressed
        update(); // Update the view
        return;
    }
    }
    QOpenGLWidget::keyPressEvent(theEvent); // Call the base class implementation
}

// ================================================================
// Function : mousePressEvent
// Purpose  : Handle the mouse press event
// ================================================================
void OcctQtViewer::mousePressEvent(QMouseEvent* theEvent)
{
    QOpenGLWidget::mousePressEvent(theEvent); // Call the base class implementation
    const Graphic3d_Vec2i aPnt(theEvent->pos().x(), theEvent->pos().y()); // Get the mouse position
    const Aspect_VKeyFlags aFlags = qtMouseModifiers2VKeys(theEvent->modifiers()); // Map Qt modifiers to virtual key flags
    if (!myView.IsNull()
        && UpdateMouseButtons(aPnt,
            qtMouseButtons2VKeys(theEvent->buttons()),
            aFlags,
            false))
    {
        updateView(); // Update the view if mouse buttons are updated
    }
}

// ================================================================
// Function : mouseReleaseEvent
// Purpose  : Handle the mouse release event
// ================================================================
void OcctQtViewer::mouseReleaseEvent(QMouseEvent* theEvent)
{
    QOpenGLWidget::mouseReleaseEvent(theEvent); // Call the base class implementation
    const Graphic3d_Vec2i aPnt(theEvent->pos().x(), theEvent->pos().y()); // Get the mouse position
    const Aspect_VKeyFlags aFlags = qtMouseModifiers2VKeys(theEvent->modifiers()); // Map Qt modifiers to virtual key flags
    if (!myView.IsNull()
        && UpdateMouseButtons(aPnt,
            qtMouseButtons2VKeys(theEvent->buttons()),
            aFlags,
            false))
    {
        updateView(); // Update the view if mouse buttons are updated
    }
}

// ================================================================
// Function : mouseMoveEvent
// Purpose  : Handle the mouse move event
// ================================================================
void OcctQtViewer::mouseMoveEvent(QMouseEvent* theEvent)
{
    QOpenGLWidget::mouseMoveEvent(theEvent); // Call the base class implementation
    const Graphic3d_Vec2i aNewPos(theEvent->pos().x(), theEvent->pos().y()); // Get the new mouse position
    if (!myView.IsNull()
        && UpdateMousePosition(aNewPos,
            qtMouseButtons2VKeys(theEvent->buttons()),
            qtMouseModifiers2VKeys(theEvent->modifiers()),
            false))
    {
        updateView(); // Update the view if mouse position is updated
        // Convert screen position to grid coordinates
        myView->ConvertToGrid(aNewPos.x(), aNewPos.y(), gridX, gridY, gridZ);

        // Emit the grid coordinates instead of just the screen position
        emit mousePositionChanged(gridX, gridY, gridZ);
    }
}

// ==============================================================================
// Function : wheelEvent
// Purpose  : Handle the mouse wheel event
// ==============================================================================
void OcctQtViewer::wheelEvent(QWheelEvent* theEvent)
{
    QOpenGLWidget::wheelEvent(theEvent); // Call the base class implementation
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    const Graphic3d_Vec2i aPos(Graphic3d_Vec2d(theEvent->position().x(), theEvent->position().y())); // Get the wheel position (Qt 5.14+)
#else
    const Graphic3d_Vec2i aPos(theEvent->pos().x(), theEvent->pos().y()); // Get the wheel position (older Qt versions)
#endif
    if (myView.IsNull())
    {
        return;
    }

    if (!myView->Subviews().IsEmpty())
    {
        Handle(V3d_View) aPickedView = myView->PickSubview(aPos); // Pick the subview at the wheel position
        if (!aPickedView.IsNull()
            && aPickedView != myFocusView)
        {
            // Switch input focus to another subview
            OnSubviewChanged(myContext, myFocusView, aPickedView);
            updateView();
            return;
        }
    }

    if (UpdateZoom(Aspect_ScrollDelta(aPos, double(theEvent->angleDelta().y()) / 8.0))) // Update zoom based on wheel scroll delta
    {
        updateView();
    }
}

// =======================================================================
// Function : updateView
// Purpose  : Update the view
// =======================================================================
void OcctQtViewer::updateView()
{
    update(); // Request an update
}

// ================================================================
// Function : paintGL
// Purpose  : Render the OpenGL scene
// ================================================================
void OcctQtViewer::paintGL()
{
    if (myView->Window().IsNull())
    {
        return;
    }

    // Wrap the FBO created by QOpenGLWidget
    Handle(OpenGl_Context) aGlCtx = OcctGlTools::GetGlContext(myView);
    Handle(OpenGl_FrameBuffer) aDefaultFbo = aGlCtx->DefaultFrameBuffer();
    if (aDefaultFbo.IsNull())
    {
        aDefaultFbo = new OcctQtFrameBuffer();
        aGlCtx->SetDefaultFrameBuffer(aDefaultFbo);
    }
    if (!aDefaultFbo->InitWrapper(aGlCtx))
    {
        aDefaultFbo.Nullify();
        Message::DefaultMessenger()->Send("Default FBO wrapper creation failed", Message_Fail);
        QMessageBox::critical(0, "Failure", "Default FBO wrapper creation failed");
        QApplication::exit(1);
        return;
    }

    // Resize the view if needed
    Graphic3d_Vec2i aViewSizeOld;
    Graphic3d_Vec2i aViewSizeNew = aDefaultFbo->GetVPSize();
    Handle(Aspect_NeutralWindow) aWindow = Handle(Aspect_NeutralWindow)::DownCast(myView->Window());
    aWindow->Size(aViewSizeOld.x(), aViewSizeOld.y());
    if (aViewSizeNew != aViewSizeOld)
    {
        aWindow->SetSize(aViewSizeNew.x(), aViewSizeNew.y());
        myView->MustBeResized();
        myView->Invalidate();
        dumpGlInfo(true, false);

        for (const Handle(V3d_View)& aSubviewIter : myView->Subviews())
        {
            aSubviewIter->MustBeResized();
            aSubviewIter->Invalidate();
            aDefaultFbo->SetupViewport(aGlCtx);
        }
    }

    // Flush pending input events and redraw the viewer
    Handle(V3d_View) aView = !myFocusView.IsNull() ? myFocusView : myView;
    aView->InvalidateImmediate();
    FlushViewEvents(myContext, aView, true);
}

//void OcctQtViewer::resizeGL(int w, int h)
//{
//    // Handle OpenGL viewport resizing
//    if (!myView.IsNull() && !myView->Window().IsNull())
//    {
//        myView->Window()->DoResize();
//        myView->MustBeResized();
//        myView->Redraw();
//    }
//}

// ================================================================
// Function : handleViewRedraw
// Purpose  : Handle the view redraw event
// ================================================================
void OcctQtViewer::handleViewRedraw(const Handle(AIS_InteractiveContext)& theCtx,
    const Handle(V3d_View)& theView)
{
    AIS_ViewController::handleViewRedraw(theCtx, theView);
    if (myToAskNextFrame)
    {
        // Request more frames for animation
        updateView();
    }
}

// ================================================================
// Function : AddXAxisZeroLine
// Purpose  : Create a red X axis line at Y = 0, Z = 0
// ================================================================
void OcctQtViewer::AddXAxisZeroLine(const Handle(AIS_InteractiveContext)& theContext)
{
    if (theContext.IsNull())
    {
        std::cerr << "Interactive context is not initialized." << std::endl;
        return;
    }

    // Define a point on the line (origin)
    gp_Pnt aPnt(0.0, 0.0, 0.0); // Origin

    // Define the direction of the line
    gp_Dir aDir(1.0, 0.0, 0.0); // Direction along the X-axis

    // Create an infinite line using the point and direction
    Handle(Geom_Line) aGeomLine = new Geom_Line(aPnt, aDir);

    // Create an AIS_Line object for visualization
    Handle(AIS_Line) aisLine = new AIS_Line(aGeomLine);

    // Set the color of the line to red
    aisLine->SetColor(Quantity_NOC_RED);

    // Display the line in the viewer
    theContext->Display(aisLine, Standard_True);

    // Optionally, make the line unselectable (as discussed earlier)
    theContext->Deactivate(aisLine);
}

// ================================================================
// Function : AddYAxisZeroLine
// Purpose  : Create a green Y axis line at X = 0, Z = 0
// ================================================================
void OcctQtViewer::AddYAxisZeroLine(const Handle(AIS_InteractiveContext)& theContext)
{
    if (theContext.IsNull())
    {
        std::cerr << "Interactive context is not initialized." << std::endl;
        return;
    }

    // Define a point on the line (origin)
    gp_Pnt aPnt(0.0, 0.0, 0.0); // Origin

    // Define the direction of the line
    gp_Dir aDir(0.0, 1.0, 0.0); // Direction along the X-axis

    // Create an infinite line using the point and direction
    Handle(Geom_Line) aGeomLine = new Geom_Line(aPnt, aDir);

    // Create an AIS_Line object for visualization
    Handle(AIS_Line) aisLine = new AIS_Line(aGeomLine);

    // Set the color of the line to red
    aisLine->SetColor(Quantity_NOC_GREEN);

    // Display the line in the viewer
    theContext->Display(aisLine, Standard_True);

    // Optionally, make the line unselectable (as discussed earlier)
    theContext->Deactivate(aisLine);
}
// ================================================================
// Function : AddZAxisZeroLine
// Purpose  : Create a short Blue Z axis line at Y = 0, X = 0
// ================================================================
void OcctQtViewer::AddZAxisZeroLine(const Handle(AIS_InteractiveContext)& theContext)
{
    if (theContext.IsNull())
    {
        std::cerr << "Interactive context is not initialized." << std::endl;
        return;
    }

    // Define the start and end points of the line along the X-axis
    gp_Pnt aPnt1(0.0, 0.0, 0.0); // Start point 
    gp_Pnt aPnt2(0.0, 0.0, 25.0);  // End point 

    // Create a line between the two points
    Handle(Geom_CartesianPoint) aP1 = new Geom_CartesianPoint(aPnt1);
    Handle(Geom_CartesianPoint) aP2 = new Geom_CartesianPoint(aPnt2);
    Handle(AIS_Line) aisLine = new AIS_Line(aP1, aP2);

    // Set the color of the line to red
    aisLine->SetColor(Quantity_NOC_BLUE);

    // Display the line in the viewer
    theContext->Display(aisLine, Standard_True);

    // Optionally, make the line unselectable (as discussed earlier)
    theContext->Deactivate(aisLine);
}

// ================================================================
// Function : OnSubviewChanged
// Purpose  : Handle the subview change event
// ================================================================
void OcctQtViewer::OnSubviewChanged(const Handle(AIS_InteractiveContext)&,
    const Handle(V3d_View)&,
    const Handle(V3d_View)& theNewView)
{
    myFocusView = theNewView; // Update the focus view
}


void OcctQtViewer::setGeometryInfo(const QRect& rect)
{
    initWindowWidth = rect.width();
    initWindowHeight = rect.height();
}