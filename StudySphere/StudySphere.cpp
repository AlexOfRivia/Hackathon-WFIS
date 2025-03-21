#include "StudySphere.h"

StudySphere::StudySphere(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this); //setting up the UI
    //setting the calendar frame as hidden
	ui.calendarFrame->hide(); //hide the calendar frame




	//QObject::connect(
	//	ui.addNewSubjectButton, &QPushButton::clicked, this, &StudySphere::addNewSubject //connecting the add subject button to the addNewSubject function
	//);


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
