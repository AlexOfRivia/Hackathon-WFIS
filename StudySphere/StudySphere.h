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

const QString calendarStyleSheet = R"(
QCalendarWidget {
    background-color: #2E2E2E;
    color: #FFFFFF;
    border: 1px solid #444444;
    border-radius: 5px;
}

QCalendarWidget QToolButton {
    color: #FFFFFF;
    background-color: #444444;
    border: none;
    width: 70px;
    height: 25px;
    border-radius: 5px;
}

QCalendarWidget QToolButton:hover {
    background-color: #555555;
}


QCalendarWidget QToolButton::menu-indicator {
    subcontrol-origin: padding;
    subcontrol-position: bottom right;
    right: 5px;
}

QCalendarWidget QSpinBox {
    color: #FFFFFF;
    background: transparent;
    border: 1px solid #444444;
    width: 50px;
    height: 25px;
    border-radius: 5px;
}

QCalendarWidget QSpinBox::up-button {
    subcontrol-origin: border;
    subcontrol-position: top right;
    width: 16px;
    border-left: 1px solid #444444;
}

QCalendarWidget QSpinBox::down-button {
    subcontrol-origin: border;
    subcontrol-position: bottom right;
    width: 16px;
    border-left: 1px solid #444444;
}

QCalendarWidget QSpinBox::up-arrow, QCalendarWidget QSpinBox::down-arrow {
    width: 10px;
    height: 10px;
}

QCalendarWidget QAbstractItemView:enabled {
    color: #FFFFFF;
    background-color: #2E2E2E;
    selection-background-color: #555555;
    selection-color: #FFFFFF;
    border: 1px solid #444444;
}

QCalendarWidget QAbstractItemView:disabled {
    color: #777777;
}

QCalendarWidget QWidget#qt_calendar_navigationbar {
    background-color: #444444;
    border-radius: 5px;
}

QCalendarWidget QTableView {
    border-width: 0px;
}

QCalendarWidget QTableView QTableCornerButton::section {
    background-color: #444444;
    border: none;
}

QCalendarWidget QHeaderView::section {
    background-color: #444444;
    color: #FFFFFF;
    border: none;
    height: 30px;
    border-radius: 5px;
}

QCalendarWidget QTableView::item {
    border: none;
    padding: 5px;
}

QCalendarWidget QTableView::item:selected {
    background-color: #555555;
    color: #FFFFFF;
    border-radius: 5px;
}
)";


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
    void deleteTableRow(int row);
    bool hasEntriesForDate(const QString& dateStr);
    void updateJsonFromTableRow(int row);
    std::vector<QString> currentJsonFiles;
    
    // Flashcard functionality members
    std::vector<QString> subjectsVector;
    std::vector<flashCard> flashCardsVector;
    std::vector<flashCard> temporaryFlashCards; //For sorting by subject
    int index = 0;
    int rightQuestions = 0;
    int wrongQuestions = 0;
};
