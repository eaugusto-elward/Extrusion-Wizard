// Precompiled Header
#include "stdafx.h"


// Local includes
#include "extrusionwizard.h"
#include "OcctQtViewer.h"

#include <Standard_WarningsDisable.hxx> // Standard disable warnings for OCCT
// Qt includes
#include <QtWidgets/QApplication>
#include <QSurfaceFormat>
#include <QAction>
#include <QLabel>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>

#include <Standard_WarningsRestore.hxx> // Standard restore warnings for OCCT

#include <Standard_Version.hxx> 
// Open CASCADE includes




//! Main application window.
class MyMainWindow : public QMainWindow
{
    OcctQtViewer* myViewer;
public:
    MyMainWindow() : myViewer(nullptr)
    {
        {
            // menu bar with Quit item
            QMenuBar* aMenuBar = new QMenuBar();
            QMenu* aMenuWindow = aMenuBar->addMenu("&File");
            {
                QAction* anActionSplit = new QAction(aMenuWindow);
                anActionSplit->setText("Split Views");
                aMenuWindow->addAction(anActionSplit);
                connect(anActionSplit, &QAction::triggered, [this]()
                    {
                        if (!myViewer->View()->Subviews().IsEmpty())
                        {
                            // remove subviews
                            myViewer->View()->View()->SetSubviewComposer(false);
                            NCollection_Sequence<Handle(V3d_View)> aSubviews = myViewer->View()->Subviews();
                            for (const Handle(V3d_View)& aSubviewIter : aSubviews)
                            {
                                aSubviewIter->Remove();
                            }
                            myViewer->OnSubviewChanged(myViewer->Context(), nullptr, myViewer->View());
                        }
                        else
                        {
                            // create two subviews splitting window horizontally
                            myViewer->View()->View()->SetSubviewComposer(true);

                            Handle(V3d_View) aSubView1 = new V3d_View(myViewer->Viewer());
                            aSubView1->SetImmediateUpdate(false);
                            aSubView1->SetWindow(myViewer->View(), Graphic3d_Vec2d(0.5, 1.0),
                                Aspect_TOTP_LEFT_UPPER, Graphic3d_Vec2d(0.0, 0.0));

                            Handle(V3d_View) aSubView2 = new V3d_View(myViewer->Viewer());
                            aSubView2->SetImmediateUpdate(false);
                            aSubView2->SetWindow(myViewer->View(), Graphic3d_Vec2d(0.5, 1.0),
                                Aspect_TOTP_LEFT_UPPER, Graphic3d_Vec2d(0.5, 0.0));

                            myViewer->OnSubviewChanged(myViewer->Context(), nullptr, aSubView1);
                        }
                        myViewer->View()->Invalidate();
                        myViewer->update();
                    });
            }
            {
                QAction* anActionQuit = new QAction(aMenuWindow);
                anActionQuit->setText("Quit");
                aMenuWindow->addAction(anActionQuit);
                connect(anActionQuit, &QAction::triggered, [this]()
                    {
                        close();
                    });
            }
            setMenuBar(aMenuBar);
        }
        {
            // 3D Viewer and some controls on top of it
            myViewer = new OcctQtViewer();
            QVBoxLayout* aLayout = new QVBoxLayout(myViewer);
            aLayout->setDirection(QBoxLayout::BottomToTop);
            aLayout->setAlignment(Qt::AlignBottom);
            {
                QPushButton* aQuitBtn = new QPushButton("About");
                aLayout->addWidget(aQuitBtn);
                connect(aQuitBtn, &QPushButton::clicked, [this]()
                    {
                        QMessageBox::information(0, "About Sample", QString()
                            + "OCCT 3D Viewer sample embedded into Qt Widgets.\n\n"
                            + "Open CASCADE Technology v." OCC_VERSION_STRING_EXT "\n"
                            + "Qt v." QT_VERSION_STR "\n\n"
                            + "OpenGL info:\n"
                            + myViewer->getGlInfo());
                    });
            }
            {
                QWidget* aSliderBox = new QWidget();
                QHBoxLayout* aSliderLayout = new QHBoxLayout(aSliderBox);
                {
                    QLabel* aSliderLabel = new QLabel("Background");
                    aSliderLabel->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: white; }");
                    aSliderLabel->setGeometry(50, 50, 50, 50);
                    aSliderLabel->adjustSize();
                    aSliderLayout->addWidget(aSliderLabel);
                }
                {
                    QSlider* aSlider = new QSlider(Qt::Horizontal);
                    aSlider->setRange(0, 255);
                    aSlider->setSingleStep(1);
                    aSlider->setPageStep(15);
                    aSlider->setTickInterval(15);
                    aSlider->setTickPosition(QSlider::TicksRight);
                    aSlider->setValue(0);
                    aSliderLayout->addWidget(aSlider);
                    connect(aSlider, &QSlider::valueChanged, [this](int theValue)
                        {
                            const float aVal = theValue / 255.0f;
                            const Quantity_Color aColor(aVal, aVal, aVal, Quantity_TOC_sRGB);

                            for (const Handle(V3d_View)& aSubviewIter : myViewer->View()->Subviews())
                            {
                                aSubviewIter->SetBgGradientColors(aColor, Quantity_NOC_BLACK, Aspect_GradientFillMethod_Elliptical);
                                aSubviewIter->Invalidate();
                            }
                            //myViewer->View()->SetBackgroundColor (aColor);
                            myViewer->View()->SetBgGradientColors(aColor, Quantity_NOC_BLACK, Aspect_GradientFillMethod_Elliptical);
                            myViewer->View()->Invalidate();
                            myViewer->update();
                        });
                }
                aLayout->addWidget(aSliderBox);
            }
            setCentralWidget(myViewer);
        }
    }
};

int main(int theNbArgs, char** theArgVec)
{
    QApplication aQApp(theNbArgs, theArgVec);

    QCoreApplication::setApplicationName("OCCT Qt Viewer sample");
    QCoreApplication::setOrganizationName("OpenCASCADE");
    QCoreApplication::setApplicationVersion(OCC_VERSION_STRING_EXT);

#ifdef __APPLE__
    // suppress Qt warning "QCocoaGLContext: Falling back to unshared context"
    bool isCoreProfile = true;
    QSurfaceFormat aGlFormat;
    aGlFormat.setDepthBufferSize(24);
    aGlFormat.setStencilBufferSize(8);
    if (isCoreProfile) { aGlFormat.setVersion(4, 5); }
    aGlFormat.setProfile(isCoreProfile ? QSurfaceFormat::CoreProfile : QSurfaceFormat::CompatibilityProfile);
    QSurfaceFormat::setDefaultFormat(aGlFormat);
#endif

    MyMainWindow aMainWindow;
    aMainWindow.resize(aMainWindow.sizeHint());
    aMainWindow.show();
    return aQApp.exec();
}
