#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_StudySphere.h"

class StudySphere : public QMainWindow
{
    Q_OBJECT

public:
    StudySphere(QWidget *parent = nullptr);
    ~StudySphere();

private slots:
    // Add these slot methods
    void on_addExamButton_clicked();
    void on_addSubjectButton_clicked();

private:
    Ui::StudySphereClass ui;
};
