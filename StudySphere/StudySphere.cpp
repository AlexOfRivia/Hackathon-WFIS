#include "StudySphere.h"

StudySphere::StudySphere(QWidget *parent)
    : QMainWindow(parent)
{

	ui.setupUi(this); //setting up the UI
    //setting the calendar frame as hidden
	ui.calendarFrame->setParent(ui.mainScreenFrame); //setting the calendar frame as a child of the central widget 
	ui.flashCard->setParent(ui.flashCardFrame); //setting the flash card as a child of the flash card frame
	ui.didntGetIt->setParent(ui.flashCardFrame); //setting the didnt get it button as a child of the flash card frame
	ui.gotIt->setParent(ui.flashCardFrame); //setting the got it button as a child of the flash card frame
	ui.flashCard->hide(); //hide the flash card
	ui.didntGetIt->hide(); //hide the didnt get it button
	ui.gotIt->hide(); //hide the got it button
	ui.showAnswerButton->hide(); //hide the show answer button
	ui.showAnswerButton->setParent(ui.flashCardFrame); //setting the show answer button as a child of the flash card frame); //setting the show answer button as a child of the flash card frame
	//ui.calendarWidget->setParent(ui.calendarFrame); //setting the calendar widget as a child of the calendar frame
	ui.calendarFrame->hide(); //hide the calendar frame
	ui.flashCardFrame->hide(); //hide the flash card frame

	QObject::connect(
		ui.addNewSubjectButton, &QPushButton::clicked, this, &StudySphere::addNewSubject //connecting the add subject button to the addNewSubject function
	);

	QObject::connect(
		ui.addFlashCardButton, &QPushButton::clicked, this, &StudySphere::addFlashCard //connecting the add flas card button to the addFlashCard function
	);

	QObject::connect(
		ui.showAnswerButton, &QPushButton::clicked, this, &StudySphere::showAnswer //connecting the show answer button to the showAnswer function
	);

	QObject::connect(
		ui.studyButton, &QPushButton::clicked, this, &StudySphere::study //connecting the study button to the study function
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
		QDialog dialog(this); // Creating a dialog box
		dialog.setWindowTitle("Enter Flash Card info"); // Setting the title of the dialog box

		QVBoxLayout layout(&dialog);// Creating a layout for the dialog
		QComboBox comboBox(&dialog); // Creating a combo box widget
		QLineEdit questionLineEdit(&dialog); // Creating a line edit widget for the question
		QLineEdit answerLineEdit(&dialog); // Creating a line edit widget for the answer
		QPushButton* okButton = new QPushButton("OK", &dialog); // Creating an OK button

		for (int i = 0; i < subjectsVector.size(); i++) //Looping through the subjects vector
		{ 
			comboBox.addItem(subjectsVector[i]); //Adding the subjects to the combo box
		}

		layout.addWidget(&comboBox); //Adding the line edit to the layout
		layout.addWidget(new QLabel("Question: ", &dialog));
		layout.addWidget(&questionLineEdit); //Adding the line edit to the layout
		layout.addWidget(new QLabel("Answer: ", &dialog));
		layout.addWidget(&answerLineEdit); //Adding the line edit to the layout
		layout.addWidget(okButton); //Adding the OK button to the layout

		QObject::connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept); //Connecting the OK button to accept the dialog

		if (dialog.exec() == QDialog::Accepted) //Executing the dialog and checking if it was accepted
		{ 
			flashCard newFlashCard; //Creating a new flash card object
			newFlashCard.setSubject(comboBox.currentText().toStdString()); //Setting the subject of the flash card
			newFlashCard.setQuestion(questionLineEdit.text().toStdString()); //Setting the question of the flash card
			newFlashCard.setAnswer(answerLineEdit.text().toStdString()); //Setting the answer of the flash card
			flashCardsVector.push_back(newFlashCard); //Adding the flash card to the vector
		}
	}
}

//Function to study the flash cards
void StudySphere::study()
{
	if (flashCardsVector.empty())
	{
		QMessageBox::warning(this, "Error", "You have no flash cards added.");
		return;
	} else {
		ui.flashCard->show(); //show the flash card
		ui.didntGetIt->show(); //show the didnt get it button
		ui.gotIt->show(); //show the got it button
		ui.showAnswerButton->show(); //show the show answer button
		ui.qaLabel->setText(QString::fromStdString(flashCardsVector[0].getQuestion())); //Setting the question of the flash card to the label	
	}
}

//Function to show the answer of the flash card
void StudySphere::showAnswer(int index)
{
	ui.qaLabel->setText(QString::fromStdString(flashCardsVector[0].getAnswer()));  //Setting the answer of the flash card to the label
}

//Function for checking if got it wrong or right
void StudySphere::didGetIt(bool wasRight)
{

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
