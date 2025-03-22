#pragma once

#include <vector>
#include <QtWidgets/QMainWindow>
#include "ui_StudySphere.h"
#include "flashCard.h" //Include flashCard class
#include <QDialog>
#include <QLineEdit>
#include <QString>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QHBoxLayout>
#include <QDate>
#include <QCalendarWidget>
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
	void addNewSubject(); //Function to add a new subject to the flashcard
	void showCalendar(); //Function to show the calendar

	void showFlashCards(); //Function to show the flash cards
	void hideCalendar(); //Function to hide the calendar
	void hideFlashCards(); //Function to hide the flash cards
	void addFlashCard(); //Function to add a flash card
	void showAnswer(); //Function to show the answer of the flash card
	void study(); //Function to study the flash cards
	void saveToJSON(); //Function to save the flash cards to a JSON file
	void loadFromJSON(); //Function to load the flash cards from a JSON file
	void seeFlashCards(); //Function to see the flash cards
	void didGetIt(bool wasRight); //Function to show the answer of the flash card

private:
	Ui::StudySphereClass ui; //UI object
    std::vector<QString> subjectsVector;
	std::vector<flashCard> flashCardsVector;
	std::vector<flashCard> temporaryFlashCards; //For sorting by subject
	int index = 0;
	
};
