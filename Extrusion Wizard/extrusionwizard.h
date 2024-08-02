#ifndef EXTRUSIONWIZARD_H
#define EXTRUSIONWIZARD_H

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QVBoxLayout>
#include "OcctQtViewer.h"

class ExtrusionWizard : public QMainWindow
{
    Q_OBJECT

public:
    explicit ExtrusionWizard(QWidget* parent = nullptr);
    ~ExtrusionWizard();

private:
    QOpenGLWidget* glWidget; // OpenGL widget
    OcctQtViewer* occtViewer; // OCCT viewer
    QVBoxLayout* layout; // Layout for the OpenGL widget
};

#endif // EXTRUSIONWIZARD_H
