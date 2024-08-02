#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_extrusionwizard.h"

class ExtrusionWizard : public QMainWindow
{
    Q_OBJECT

public:
    ExtrusionWizard(QWidget *parent = nullptr);
    ~ExtrusionWizard();

private:
    Ui::ExtrusionWizardClass ui;
};
