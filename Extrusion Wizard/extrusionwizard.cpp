#include "stdafx.h"
#include "extrusionwizard.h"

ExtrusionWizard::ExtrusionWizard(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    initializeOcctViewer(); // Initialize the OCCT viewer after UI setup

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

void ExtrusionWizard::updatePositionLabels(double x, double y, double z)
{
    ui.xposlabel->setText(QString::number(x));
    ui.yposlabel->setText(QString::number(y));
    ui.zposlabel->setText(QString::number(z));
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

