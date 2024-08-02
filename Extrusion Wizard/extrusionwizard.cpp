#include "stdafx.h"
#include "extrusionwizard.h"

ExtrusionWizard::ExtrusionWizard(QWidget* parent)
    : QMainWindow(parent),
    glWidget(new QOpenGLWidget(this)),
    occtViewer(new OcctQtViewer(glWidget)),
    layout(new QVBoxLayout(glWidget))
{
    // Set the layout for the OpenGL widget
    layout->addWidget(occtViewer);
    glWidget->setLayout(layout);

    // Set the central widget of the main window to be the OpenGL widget
    setCentralWidget(glWidget);
}

ExtrusionWizard::~ExtrusionWizard()
{
    delete layout;
    delete occtViewer;
    delete glWidget;
}
