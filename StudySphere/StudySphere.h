#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_StudySphere.h"
#include <string>
#include <vector>

class StudySphere : public QMainWindow
{
    Q_OBJECT

public:
    StudySphere(QWidget *parent = nullptr);
    ~StudySphere();

private slots:
    void on_addExamButton_clicked();
    void on_addSubjectButton_clicked();
    void on_backFromInfo_clicked();
    void saveToJson(const std::string& date, const std::string& name, const std::string& startTime, const std::string& endTime, const std::string& note, const std::string& filename);
    void saveToJson(const std::string& date, const std::string& name, const std::string& startTime, const std::string& endTime, const std::string& room, const std::string& isRetake, const std::string& note, const std::string& filename);
//    void on_calendarWidget_clicked(const QDate&);


private:
    Ui::StudySphereClass ui;
    std::vector<std::string> getJsonFiles(const std::string& date);
};
