#include "StudySphere.h"

StudySphere::StudySphere(QWidget *parent)
    : QMainWindow(parent)
{

	ui.setupUi(this); //setting up the UI
    //setting the calendar frame as hidden
	ui.calendarFrame->setParent(ui.mainScreenFrame); //setting the calendar frame as a child of the central widget 
	//ui.calendarWidget->setParent(ui.calendarFrame); //setting the calendar widget as a child of the calendar frame
	ui.calendarFrame->hide(); //hide the calendar frame
	ui.flashCardFrame->hide(); //hide the flash card frame

	QObject::connect(
		ui.addNewSubjectButton, &QPushButton::clicked, this, &StudySphere::addNewSubject //connecting the add subject button to the addNewSubject function
	);

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

//Function to add a flash card
void StudySphere::addFlashCard()
{
	if (subjectsVector.empty())
	{
		QMessageBox::warning(this, "Error", "You have no subjects .");
		return;
	} else {
		
	}
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
	QDialog dialog(this); // Creating a dialog box
	dialog.setWindowTitle("Add Subject Name"); // Setting the title of the dialog box

	QVBoxLayout layout(&dialog); // Creating a layout for the dialog
	QLineEdit lineEdit(&dialog); // Creating a line edit widget
	QPushButton* okButton = new QPushButton("OK", &dialog); // Creating an OK button

	layout.addWidget(&lineEdit); // Adding the line edit to the layout
	layout.addWidget(okButton); // Adding the OK button to the layout

	QObject::connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept); // Connecting the OK button to accept the dialog

	if (dialog.exec() == QDialog::Accepted) { // Executing the dialog and checking if it was accepted
		this->subjectsVector.push_back(lineEdit.text()); // Adding the subject to the vector
		ui.subjectsComboBox->addItem(lineEdit.text()); // Adding the subject to the combo box
	}
}

StudySphere::~StudySphere()
{}
