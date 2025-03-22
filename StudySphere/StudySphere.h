#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_StudySphere.h"
#include <string>
#include <QString>
#include <vector>
#include <QCalendarWidget>
#include <QTableWidget>
#include "flashCard.h" //Include flashCard class
#include <QDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QHBoxLayout>
#include <QDate>
#include <QTextStream>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>

class StudySphere : public QMainWindow
{
    Q_OBJECT
public:
    StudySphere(QWidget *parent = nullptr); //Constructor
    ~StudySphere(); //Destructor
    
    // Flashcard functionality
    void addNewSubject(); //Function to add a new subject to the flashcard
    void addFlashCard(); //Function to add a flash card
    void showAnswer(); //Function to show the answer of the flash card
    void study(); //Function to study the flash cards
    void saveToJSON(); //Function to save the flash cards to a JSON file
    void loadFromJSON(); //Function to load the flash cards from a JSON file
    void seeFlashCards(); //Function to see the flash cards
    void didGetIt(bool wasRight); //Function for checking if got it wrong or right
    void showFlashCards(); //Function to show the flash cards UI
    void hideFlashCards(); //Function to hide the flash cards UI
    
    // Calendar functionality
    void showCalendar(); //Function to show the calendar
    void hideCalendar(); //Function to hide the calendar
    
private:
    Ui::StudySphereClass ui; //UI object
    
    // Calendar functionality members
    void on_addExamButton_clicked();
    void on_addSubjectButton_clicked();
    void on_backFromInfo_clicked();
    void on_calendarWidget_clicked(const QDate& date);
    void on_tableCellChanged(int row, int column);
    void saveToJson(const std::string& date, const std::string& name, const std::string& startTime, const std::string& endTime, const std::string& note, const std::string& filename);
    void saveToJson(const std::string& date, const std::string& name, const std::string& startTime, const std::string& endTime, const std::string& room, const std::string& isRetake, const std::string& note, const std::string& filename);
    void updateJsonFromTable(int row);
    std::vector<QString> getJsonFiles(const std::string& date);
    void populateTableFromJson(QTableWidget* tableWidget, const std::vector<QString>& jsonFiles);
    void highlightDatesWithData();
    void on_deleButton(QPushButton* deleteButton);
    std::vector<QString> currentJsonFiles;
    
    // Flashcard functionality members
    std::vector<QString> subjectsVector;
    std::vector<flashCard> flashCardsVector;
    std::vector<flashCard> temporaryFlashCards; //For sorting by subject
    int index = 0;
};
