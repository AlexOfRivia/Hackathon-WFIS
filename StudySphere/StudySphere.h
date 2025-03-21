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
	void didGetIt(); //Function to show the answer of the flash card

private:
	Ui::StudySphereClass ui; //UI object
    std::vector<QString> subjectsVector;
	std::vector<flashCard> flashCardsVector;
	std::vector<flashCard> temporaryFlashCards; //For sorting by subject
};
