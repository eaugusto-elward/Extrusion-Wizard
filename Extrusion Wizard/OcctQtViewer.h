// Copyright (c) 2021 OPEN CASCADE SAS
//
// This file is part of the examples of the Open CASCADE Technology software library.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE

#ifndef _OcctQtViewer_HeaderFile
#define _OcctQtViewer_HeaderFile

#include <Standard_WarningsDisable.hxx>
#include <QOpenGLWidget>
#include <Standard_WarningsRestore.hxx>

#include <AIS_InteractiveContext.hxx>
#include <AIS_ViewController.hxx>
#include <V3d_View.hxx>
#include <Aspect_RectangularGrid.hxx>
#include <Quantity_Color.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <AIS_Line.hxx>
#include <Geom_CartesianPoint.hxx>
#include <Geom_Line.hxx>

class AIS_ViewCube;

//! OCCT 3D View.
class OcctQtViewer : public QOpenGLWidget, public AIS_ViewController
{
	Q_OBJECT
public:

	//! Main constructor.
	OcctQtViewer(QWidget* theParent = nullptr);

	//! Destructor.
	virtual ~OcctQtViewer();

	//! Return Viewer.
	const Handle(V3d_Viewer)& Viewer() const { return myViewer; }

	//! Return View.
	const Handle(V3d_View)& View() const { return myView; }

	//! Return AIS context.
	const Handle(AIS_InteractiveContext)& Context() const { return myContext; }

	//! Return OpenGL info.
	const QString& getGlInfo() const { return myGlInfo; }

	//! Minial widget size.
	virtual QSize minimumSizeHint() const override { return QSize(400, 400); }

	//! Default widget size.
	virtual QSize sizeHint()        const override { return QSize(720, 480); }

	void setGeometryInfo(const QRect& rect);

signals:
	void mousePositionChanged(double x, double y, double z);

public:
	void changeToMM();   // Slot to change grid to millimeters
	void changeToInch(); // Slot to change grid to inches
	void changeToFeet(); // Slot to change grid to feet
	void toggleXLine();
	void toggleYLine();
	void toggleZLine();
	void toggleGrid();
	void toggleFPS();
	void createBasicGrid();
	//! Dump OpenGL info.
	void openGlInfoBox();

public:

	//! Handle subview focus change.
	virtual void OnSubviewChanged(const Handle(AIS_InteractiveContext)&,
		const Handle(V3d_View)&,
		const Handle(V3d_View)& theNewView) override;

protected: // OpenGL events

	virtual void initializeGL() override;
	virtual void paintGL() override;
	//virtual void resizeGL(int w, int h) override;

protected: // user input events

	virtual void closeEvent(QCloseEvent* theEvent) override;
	virtual void keyPressEvent(QKeyEvent* theEvent) override;
	virtual void mousePressEvent(QMouseEvent* theEvent) override;
	virtual void mouseReleaseEvent(QMouseEvent* theEvent) override;
	virtual void mouseMoveEvent(QMouseEvent* theEvent) override;
	virtual void wheelEvent(QWheelEvent* theEvent) override;

private:

	//! Dump OpenGL info.
	void dumpGlInfo(bool theIsBasic, bool theToPrint);

	//! Request widget paintGL() event.
	void updateView();

	//! Handle view redraw.
	virtual void handleViewRedraw(const Handle(AIS_InteractiveContext)& theCtx,
		const Handle(V3d_View)& theView) override;
	//! Add a red 0 line to X axis.
	void AddXAxisZeroLine(const Handle(AIS_InteractiveContext)& theContext);

	//! Add a green 0 line to Y axis.
	void AddYAxisZeroLine(const Handle(AIS_InteractiveContext)& theContext);

	//! Add a Blue 0 line to X axis.
	void AddZAxisZeroLine(const Handle(AIS_InteractiveContext)& theContext);





public:
	

	Handle(V3d_Viewer)             myViewer;
	Handle(V3d_View)               myView;
	Handle(AIS_InteractiveContext) myContext;
	Handle(AIS_ViewCube)           myViewCube;

	Handle(V3d_View)               myFocusView;

	Handle(Aspect_Grid) myGrid;

private:
	Handle(AIS_Line) myXAxisZeroLine;
	Handle(AIS_Line) myYAxisZeroLine;
	Handle(AIS_Line) myZAxisZeroLine;

	QString myGlInfo;
	bool myIsCoreProfile;

private:
	int initWindowWidth, initWindowHeight;    // Pulled from the QT geometry
	double gridX, gridY, gridZ;
	bool xLineison, yLineison, zLineison = true;
	bool gridIsOn = true;
	/*enum GridType { MM, INCH, FEET };
	GridType gridType = MM;*/
	

};

#endif // _OcctQtViewer_HeaderFile
