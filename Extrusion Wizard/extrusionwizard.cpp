#include "stdafx.h"
#include "extrusionwizard.h"

ExtrusionWizard::ExtrusionWizard(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	initializeOcctViewer(); // Initialize the OCCT viewer after UI setup


	// Connect the grid buttons to the corresponding signals
	connect(ui.buttonToMM, &QPushButton::clicked, this, &ExtrusionWizard::changeToMM);
	connect(ui.buttonToInch, &QPushButton::clicked, this, &ExtrusionWizard::changeToInch);
	connect(ui.buttonToFeet, &QPushButton::clicked, this, &ExtrusionWizard::changeToFeet);
	connect(ui.buttonToggleX, &QPushButton::clicked, this, &ExtrusionWizard::toggleXLine);
	connect(ui.buttonToggleY, &QPushButton::clicked, this, &ExtrusionWizard::toggleYLine);
	connect(ui.buttonToggleZ, &QPushButton::clicked, this, &ExtrusionWizard::toggleZLine);
	connect(ui.buttonToggleGrid, &QPushButton::clicked, this, &ExtrusionWizard::toggleGrid);
	connect(ui.buttonDumpGL, &QPushButton::clicked, this, &ExtrusionWizard::dumpGlInfo);
	connect(ui.toggleFPS, &QPushButton::clicked, this, &ExtrusionWizard::toggleFPS);

	connect(ui.sizeSlider, &QSlider::valueChanged, this, &ExtrusionWizard::onSizeSliderValueChanged);


	// Connect the mouse position changed signal to the updatePositionLabels slot
	connect(occtViewer, &OcctQtViewer::mousePositionChanged,
		this, &ExtrusionWizard::updatePositionLabels);
}

ExtrusionWizard::~ExtrusionWizard()
{
	delete occtViewer; // Safely delete occtViewer if it was created

}


void ExtrusionWizard::initializeOcctViewer()
{
	// Find the OpenGL widget by its object name
	QOpenGLWidget* glWidget = this->findChild<QOpenGLWidget*>("mainGL");

	if (glWidget)
	{

		// Initialize the OcctQtViewer with the OpenGL widget
		occtViewer = new OcctQtViewer(glWidget);

	}
	else
	{
		qDebug() << "OpenGL widget with name 'glWidget' not found.";
	}
}

void ExtrusionWizard::updateViewerGeometry()
{
	if (occtViewer)
	{
		// Set the geometry information for the viewer
		QOpenGLWidget* glWidget = this->findChild<QOpenGLWidget*>("mainGL");
		if (glWidget)
		{
			occtViewer->setGeometryInfo(glWidget->geometry());

			// Optionally, print widget information for debugging
			QRect geometry = glWidget->geometry();
			qDebug() << "mainGL Widget Geometry:";
			qDebug() << "X:" << geometry.x();
			qDebug() << "Y:" << geometry.y();
			qDebug() << "Width:" << geometry.width();
			qDebug() << "Height:" << geometry.height();
		}

		// Print debug information
		printDebugInfo();
	}
}

void ExtrusionWizard::printDebugInfo() const
{
	// Get the geometry information for the main window
	QRect geometry = this->geometry();

	// Get the geometry information for the OCCT viewer
	QRect occtGeometry = occtViewer->geometry();

	//Print all with qdebug
	qDebug() << "Main Window Geometry:";
	qDebug() << "X:" << geometry.x();
	qDebug() << "Y:" << geometry.y();
	qDebug() << "Width:" << geometry.width();
	qDebug() << "Height:" << geometry.height();
	qDebug() << "OCCT Viewer Geometry:";
	qDebug() << "X:" << occtGeometry.x();
	qDebug() << "Y:" << occtGeometry.y();
	qDebug() << "Width:" << occtGeometry.width();
	qDebug() << "Height:" << occtGeometry.height();


}

void ExtrusionWizard::changeToMM()
{
	gridType = MM;
}

void ExtrusionWizard::changeToInch()
{
	gridType = INCH;
}


void ExtrusionWizard::changeToFeet()
{
	gridType = FEET;
}

void ExtrusionWizard::toggleXLine()
{
	if (occtViewer)
	{
		occtViewer->toggleXLine();  // Directly call the slot in OcctQtViewer
	}
}

void ExtrusionWizard::toggleYLine()
{
	if (occtViewer)
	{
		occtViewer->toggleYLine();  // Directly call the slot in OcctQtViewer
	}
}

void ExtrusionWizard::toggleZLine()
{
	if (occtViewer)
	{
		occtViewer->toggleZLine();  // Directly call the slot in OcctQtViewer
	}
}

void ExtrusionWizard::toggleGrid()
{
	if (occtViewer)
	{
		occtViewer->toggleGrid();  // Directly call the slot in OcctQtViewer
	}
}

void ExtrusionWizard::dumpGlInfo()
{
	if (occtViewer)
	{
		occtViewer->openGlInfoBox();  // Directly call the slot in OcctQtViewer
	}
}

void ExtrusionWizard::toggleFPS()
{
	if (occtViewer)
	{
		occtViewer->toggleFPS();  // Directly call the slot in OcctQtViewer
	}
}


void ExtrusionWizard::updatePositionLabels(double x, double y, double z)
{
	// Switch case to change the displayed values based on the grid type
	switch (gridType)
	{
	case MM:
		ui.xposlabel->setText(QString("X: ") + QString::number(x) + " (mm)");
		ui.yposlabel->setText(QString("Y: ") + QString::number(y) + " (mm)");
		ui.zposlabel->setText(QString("Z: ") + QString::number(z) + " (mm)");
		break;
	case INCH:
		ui.xposlabel->setText(QString("X: ") + QString::number(x / 25.4, 'f', 3) + " (in)");
		ui.yposlabel->setText(QString("Y: ") + QString::number(y / 25.4, 'f', 3) + " (in)");
		ui.zposlabel->setText(QString("Z: ") + QString::number(z / 25.4, 'f', 3) + " (in)");
		break;
	case FEET:
		ui.xposlabel->setText(QString("X: ") + QString::number(x / 304.8, 'f', 3) + " (ft)");
		ui.yposlabel->setText(QString("Y: ") + QString::number(y / 304.8, 'f', 3) + " (ft)");
		ui.zposlabel->setText(QString("Z: ") + QString::number(z / 304.8, 'f', 3) + " (ft)");
		break;
	default:
		// Handle unexpected grid type
		ui.xposlabel->setText("X: N/A");
		ui.yposlabel->setText("Y: N/A");
		ui.zposlabel->setText("Z: N/A");
		break;
	}
}




void ExtrusionWizard::showEvent(QShowEvent* event)
{
	QMainWindow::showEvent(event);
	updateViewerGeometry(); // Update geometry when the window is first shown
}

void ExtrusionWizard::resizeEvent(QResizeEvent* event)
{
	QMainWindow::resizeEvent(event);
	updateViewerGeometry(); // Update geometry on window resize
}


void ExtrusionWizard::onSizeSliderValueChanged(int value)
{
	// Convert the slider value to a suitable grid size
	double gridSize = static_cast<double>(value); // or apply any scaling factor you need

	
}
