#ifndef EXTRUSIONWIZARD_H
#define EXTRUSIONWIZARD_H

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QVBoxLayout>
#include "OcctQtViewer.h"
#include "ui_extrusionwizard.h"

class ExtrusionWizard : public QMainWindow
{
    Q_OBJECT

public:
    explicit ExtrusionWizard(QWidget* parent = nullptr);
    ~ExtrusionWizard();

private:
    Ui::ExtrusionWizardClass ui; // UI object
    OcctQtViewer* occtViewer = nullptr; // OCCT viewer
    void initializeOcctViewer(); // Function to initialize OCCT viewer
};

#endif // EXTRUSIONWIZARD_H
