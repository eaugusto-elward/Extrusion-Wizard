#include "stdafx.h"
#include "extrusionwizard.h"

ExtrusionWizard::ExtrusionWizard(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    initializeOcctViewer(); // Initialize the OCCT viewer after UI setup
}

ExtrusionWizard::~ExtrusionWizard()
{
    delete occtViewer; // Safely delete occtViewer if it was created

}


void ExtrusionWizard::initializeOcctViewer()
{
    // Find the OpenGL widget by its object name
    QOpenGLWidget* glWidget = this->findChild<QOpenGLWidget*>("glWidget");

    if (glWidget)
    {
        // Initialize the OcctQtViewer with the OpenGL widget
        occtViewer = new OcctQtViewer(glWidget);

        // Set the geometry information for the viewer
        occtViewer->setGeometryInfo(glWidget->geometry());

    }
    else
    {
        qDebug() << "OpenGL widget with name 'glWidget' not found.";
    }
}