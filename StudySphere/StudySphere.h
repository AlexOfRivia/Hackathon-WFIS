#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_StudySphere.h"
#include <string>
#include <QString>
#include <vector>
#include <QCalendarWidget>
#include <QTableWidget>

class StudySphere : public QMainWindow
{
    Q_OBJECT
public:
    StudySphere(QWidget *parent = nullptr);
    ~StudySphere();
private:
    Ui::StudySphereClass ui;
    void on_addExamButton_clicked();
    void on_addSubjectButton_clicked();
    void on_backFromInfo_clicked();
    void on_calendarWidget_clicked(const QDate& date);
    void saveToJson(const std::string& date, const std::string& name, const std::string& startTime, const std::string& endTime, const std::string& note, const std::string& filename);
    void saveToJson(const std::string& date, const std::string& name, const std::string& startTime, const std::string& endTime, const std::string& room, const std::string& isRetake, const std::string& note, const std::string& filename);
    std::vector<QString> getJsonFiles(const std::string& date);
    void populateTableFromJson(QTableWidget* tableWidget, const std::vector<QString>& jsonFiles);
    void highlightDatesWithData();
};
