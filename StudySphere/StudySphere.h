#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_StudySphere.h"

class StudySphere : public QMainWindow
{
    Q_OBJECT

public:
    StudySphere(QWidget *parent = nullptr);
    ~StudySphere();

private:
    Ui::StudySphereClass ui;
};
