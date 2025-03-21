#include "StudySphere.h"

StudySphere::StudySphere(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this); //setting up the UI
    //setting the calendar frame as hidden
	ui.calendarFrame->setParent(ui.mainScreenFrame); //setting the calendar frame as a child of the central widget 
	ui.centralWidget->setParent(ui.calendarFrame); //setting the calendar widget as a child of the calendar frame
	ui.calendarFrame->hide(); //hide the calendar frame
	ui.flashCardFrame->hide(); //hide the flash card frame

	//QObject::connect(
	//	ui.addNewSubjectButton, &QPushButton::clicked, this, &StudySphere::addNewSubject //connecting the add subject button to the addNewSubject function
	//);

	QObject::connect(
		ui.calendarButton, &QPushButton::clicked, this, &StudySphere::showCalendar //connecting the calendar button to the showCalendar function
	);

	QObject::connect(
		ui.flashCardsButton, &QPushButton::clicked, this, &StudySphere::showFlashCards //connecting the flash card button to the showFlashCards function
	);

	QObject::connect(
		ui.backButton2, &QPushButton::clicked, this, &StudySphere::hideFlashCards //connecting the add subject button to the addNewSubject function
	);

	QObject::connect(
		ui.backButton, &QPushButton::clicked, this, &StudySphere::hideCalendar //connecting the back button to the hideCalendar function
	);


}

//Function to show the calendar
void StudySphere::showCalendar()
{
	/*this will be used after clicking the calendar button*/
	ui.calendarFrame->show(); //show the calendar frame
}

void StudySphere::showFlashCards()
{
	/*this will be used after clicking the flash card button*/
	ui.flashCardFrame->show(); //show the flash card frame
}

//Function to hide the calendar
void StudySphere::hideCalendar()
{
	ui.calendarFrame->hide(); //hide the calendar frame
}

//Function to hide flash cards
void StudySphere::hideFlashCards()
{
	ui.flashCardFrame->hide(); //hide the flash card frame 
}

//Function to add a new subject to the flashcard
void StudySphere::addNewSubject()
{
    /*this will be used after clicking the add new subject button*/
	QDialog dialog(this); //Creating a dialog box
	dialog.setWindowTitle("Add Subject Name"); //Setting the title of the dialog box

	QLineEdit lineEdit(&dialog); //Creating a line edit widget
	QPushButton* okButton = new QPushButton("OK"); //Creating an OK button

	this->subjectsVector.push_back(lineEdit.text().toStdString()); //Adding the subject to the subjects vector

}

StudySphere::~StudySphere()
{}
