// Precompiled Header
#include "stdafx.h"

// Local includes for custom functionality
#include "extrusionwizard.h" // Include for custom extrusion wizard functionality
#include "OcctQtViewer.h"    // Include for custom Open CASCADE Technology (OCCT) viewer class

#include <Standard_WarningsDisable.hxx> // Disable certain compiler warnings specific to OCCT

// Qt includes for GUI components and functionalities
#include <QtWidgets/QApplication> // Main application class for Qt applications
#include <QSurfaceFormat>         // Class to specify the format of the surface (e.g., OpenGL settings)
#include <QAction>                // Class for actions that can be added to menus and toolbars
#include <QLabel>                 // Class for displaying text or images
#include <QMainWindow>            // Base class for application's main window
#include <QMenuBar>               // Class to create menu bars
#include <QMessageBox>            // Class to display message boxes
#include <QVBoxLayout>            // Layout manager for arranging widgets vertically
#include <QPushButton>            // Class for creating push buttons
#include <QSlider>                // Class for creating slider controls

#include <Standard_WarningsRestore.hxx> // Restore the compiler warnings for OCCT

#include <Standard_Version.hxx> // Include to access OCCT version information


// Main function, entry point of the application
int main(int theNbArgs, char** theArgVec)
{
    QApplication aQApp(theNbArgs, theArgVec); // Initialize the Qt application with command-line arguments

    ExtrusionWizard mainWindow;
    //mainWindow.resize(mainWindow.sizeHint());
    mainWindow.show();
    
    return aQApp.exec(); // Enter the Qt event loop and start the application
}


// List of TODO items:
// *The link between the viewer object from the OCCT library and the OpenGL widget in the Qt application is not yet established in a reliable way. Everything is hardcoded
//  by pixels rather than have variable resizing and scaling. This is a major issue that needs to be addressed.