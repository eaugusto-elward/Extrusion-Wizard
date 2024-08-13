#ifndef EXTRUSIONWIZARD_H
#define EXTRUSIONWIZARD_H

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "OcctQtViewer.h"
#include "ui_extrusionwizard.h"

class ExtrusionWizard : public QMainWindow
{
    Q_OBJECT

public:
    explicit ExtrusionWizard(QWidget* parent = nullptr);
    ~ExtrusionWizard();

protected:
    void showEvent(QShowEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void updatePositionLabels(double x, double y, double z);

private:
    Ui::ExtrusionWizardClass ui; // UI object
    OcctQtViewer* occtViewer = nullptr; // OCCT viewer
    void initializeOcctViewer(); // Function to initialize OCCT viewer
    void updateViewerGeometry(); // Function to update viewer geometry
    void printDebugInfo() const;
    

    void changeToMM();
    void changeToInch();
    void changeToFeet();

    void toggleXLine();
    void toggleYLine();
    void toggleZLine();
    void toggleGrid();
    void dumpGlInfo();
    void toggleFPS();
};

#endif // EXTRUSIONWIZARD_H
