#include "StudySphere.h"

StudySphere::StudySphere(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this); //setting up the UI
	this->loadFromJSON(); //loading the flash cards from a JSON file
    //setting the calendar frame as hidden
	ui.calendarFrame->setParent(ui.mainScreenFrame); //setting the calendar frame as a child of the central widget 
	ui.flashCard->setParent(ui.flashCardFrame); //setting the flash card as a child of the flash card frame
	ui.didntGetIt->setParent(ui.flashCardFrame); //setting the didnt get it button as a child of the flash card frame
	ui.gotIt->setParent(ui.flashCardFrame); //setting the got it button as a child of the flash card frame
	ui.seeFlashCardsButton->setParent(ui.flashCardFrame); //setting the see flash cards button as a child of the central widget
	ui.flashCard->hide(); //hide the flash card
	ui.didntGetIt->hide(); //hide the didnt get it button
	ui.gotIt->hide(); //hide the got it button
	ui.showAnswerButton->hide(); //hide the show answer button
	ui.showAnswerButton->setParent(ui.flashCardFrame); //setting the show answer button as a child of the flash card frame); //setting the show answer button as a child of the flash card frame
	ui.calendarWidget->setParent(ui.calendarFrame); //setting the calendar widget as a child of the calendar frame
	ui.calendarFrame->hide(); //hide the calendar frame
	ui.flashCardFrame->hide(); //hide the flash card frame

	QObject::connect(
		ui.addNewSubjectButton, &QPushButton::clicked, this, &StudySphere::addNewSubject //connecting the add subject button to the addNewSubject function
	);

	QObject::connect(
		ui.addFlashCardButton, &QPushButton::clicked, this, &StudySphere::addFlashCard //connecting the add flas card button to the addFlashCard function
	);

	QObject::connect(
		ui.gotIt, &QPushButton::clicked, this, [=]() { this->didGetIt(true); } //connecting the got it button to the didGetIt function
	);

	QObject::connect(
		ui.didntGetIt, &QPushButton::clicked, this, [=]() { this->didGetIt(false); } //connecting the got it button to the didGetIt function
	);

	QObject::connect(
		ui.showAnswerButton, &QPushButton::clicked, this, &StudySphere::showAnswer //connecting the show answer button to the showAnswer function
	);

	QObject::connect(
		ui.studyButton, &QPushButton::clicked, this, &StudySphere::study //connecting the study button to the study function
	);
	
	QObject::connect(
		ui.seeFlashCardsButton, &QPushButton::clicked, this, &StudySphere::seeFlashCards //connecting the see flash cards button to the seeFlashCards function
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

//Function to see the flash cards
void StudySphere::seeFlashCards()
{
	if (flashCardsVector.empty())
	{
		QMessageBox::warning(this, "Error", "You have no flash cards added.");
		return;
	}
	else {

		//checkbox, question, answer, subject - delete checked ones
		QDialog dialog(this); //Creating a dialog box
		dialog.setWindowTitle("Flash Cards"); //Setting the title of the dialog box

		QVBoxLayout layout(&dialog); //Creating a layout for the dialog
		QVBoxLayout flashCardLayout; //Creating a layout for the flash cards
		for (int i = 0; i < flashCardsVector.size(); i++) //Looping through the flash cards vector
		{
			QHBoxLayout* flashCardRow = new QHBoxLayout(); //Creating a row for the flash card
			QCheckBox* checkBox = new QCheckBox(&dialog); //Creating a check box
			QLabel* questionLabel = new QLabel(QString::fromStdString(flashCardsVector[i].getQuestion()), &dialog); //Creating a label for the question
			QLabel* answerLabel = new QLabel(QString::fromStdString(flashCardsVector[i].getAnswer()), &dialog); //Creating a label for the answer
			QLabel* subjectLabel = new QLabel(QString::fromStdString(flashCardsVector[i].getSubject()), &dialog); //Creating a label for the subject
			flashCardRow->addWidget(checkBox); //Adding the check box to the row
			flashCardRow->addWidget(questionLabel); //Adding the question label to the row
			flashCardRow->addWidget(answerLabel); //Adding the answer label to the row
			flashCardRow->addWidget(subjectLabel); //Adding the subject label to the row
			flashCardLayout.addLayout(flashCardRow); //Adding the row to the layout
		}
		layout.addLayout(&flashCardLayout); //Adding the flash card layout to the main layout
		layout.addWidget(new QPushButton("Delete Checked", &dialog)); //Adding a delete checked button to the layout
		dialog.exec(); //Executing the dialog
		

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

		//Clear the temporary flash cards vector
		temporaryFlashCards.clear();

		//Populate the temporary flash cards vector based on the selected subject
		for (int i = 0; i < flashCardsVector.size(); i++) //Looping through the flash cards vector
		{
			if (flashCardsVector[i].getSubject() == ui.subjectsComboBox->currentText().toStdString()) //Checking if the subject of the flash card is equal to the subject in the combo box
			{
				temporaryFlashCards.push_back(flashCardsVector[i]); //Adding the flash cards to the temporary flash cards vector
			}
		}

		//Check if there are any flash cards for the selected subject
		if (temporaryFlashCards.empty())
		{
			QMessageBox::warning(this, "Error", "No flash cards found for the selected subject.");
			return;
		}

		// Reset the index
		this->index = 0;

		//Set the alignment of the label to center
		ui.qaLabel->setAlignment(Qt::AlignCenter);
		// Set the question of the first flash card in the temporary vector to the label
		ui.qaLabel->setText(QString::fromStdString(temporaryFlashCards[index].getQuestion())); //Setting the question of the flash card to the label
	}
}


//Function to show the answer of the flash card
void StudySphere::showAnswer()
{
	//Set the alignment of the label to center
	ui.qaLabel->setAlignment(Qt::AlignCenter);
	ui.qaLabel->setText(QString::fromStdString(temporaryFlashCards[index].getAnswer()));  //Setting the answer of the flash card to the label
}

//Saving to JSON file
void StudySphere::saveToJSON()
{
	//Creating a JSON object
	QJsonObject jsonObject;
	//Creating a JSON array
	QJsonArray jsonArray;
	//Looping through the flash cards vector
	for (int i = 0; i < flashCardsVector.size(); i++)
	{
		//Creating a JSON object
		QJsonObject flashCardObject;
		//Adding the subject of the flash card to the JSON object
		flashCardObject["subject"] = QString::fromStdString(flashCardsVector[i].getSubject());
		//Adding the question of the flash card to the JSON object
		flashCardObject["question"] = QString::fromStdString(flashCardsVector[i].getQuestion());
		//Adding the answer of the flash card to the JSON object
		flashCardObject["answer"] = QString::fromStdString(flashCardsVector[i].getAnswer());
		//Adding the JSON object to the JSON array
		jsonArray.append(flashCardObject);
	}
	//Adding the JSON array to the JSON object
	jsonObject["flashCards"] = jsonArray;
	//Creating a JSON document
	QJsonDocument jsonDocument(jsonObject);
	//Creating a file object
	QFile file("flashCards.json");
	//Opening the file in write mode
	file.open(QIODevice::WriteOnly);
	//Writing the JSON document to the file
	file.write(jsonDocument.toJson());
	//Closing the file
	file.close();
}

//Loading from JSON file
void StudySphere::loadFromJSON()
{
	// Creating a file object
	QFile file("flashCards.json");
	// Opening the file in read mode
	if (!file.open(QIODevice::ReadOnly))
	{
		// Handle file open error
		QMessageBox::warning(this, "Error", "Unable to open file for reading.");
		return;
	}
	// Creating a JSON document
	QJsonDocument jsonDocument = QJsonDocument::fromJson(file.readAll());
	// Closing the file
	file.close();
	// Creating a JSON object
	QJsonObject jsonObject = jsonDocument.object();
	// Creating a JSON array
	QJsonArray jsonArray = jsonObject["flashCards"].toArray();
	// Clearing the flash cards vector
	flashCardsVector.clear();
	// Clearing the subjects vector to avoid duplicates
	subjectsVector.clear();
	// Looping through the JSON array
	for (const auto& flashCardValue : jsonArray)
	{
		QJsonObject flashCardObject = flashCardValue.toObject();
		flashCard newFlashCard;
		newFlashCard.setSubject(flashCardObject["subject"].toString().toStdString());
		newFlashCard.setQuestion(flashCardObject["question"].toString().toStdString());
		newFlashCard.setAnswer(flashCardObject["answer"].toString().toStdString());
		// Adding the flash card to the flash cards vector
		flashCardsVector.push_back(newFlashCard);
		// Adding the subject to the subjects vector if not already present
		QString subject = QString::fromStdString(newFlashCard.getSubject());
		if (std::find(subjectsVector.begin(), subjectsVector.end(), subject) == subjectsVector.end())
		{
			subjectsVector.push_back(subject);
		}
	}
	// Adding the subjects to the combo box
	for (int i = 0; i < subjectsVector.size(); i++)
	{
		ui.subjectsComboBox->addItem(subjectsVector[i]);
	}
}

//Function for checking if got it wrong or right
void StudySphere::didGetIt(bool wasRight)
{
		this->index++; // Incrementing the index
		if (index >= temporaryFlashCards.size())
		{
			this->index = 0;
		}

		//Set the alignment of the label to center
		ui.qaLabel->setAlignment(Qt::AlignCenter);
		ui.qaLabel->setText(QString::fromStdString(temporaryFlashCards[index].getQuestion())); // Setting the question of the flash card to the label
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

	

	if (dialog.exec() == QDialog::Accepted) { //Executing the dialog and checking if it was accepted
		QString newSubject = lineEdit.text();

		//Check if the subject already exists
		if (std::find(subjectsVector.begin(), subjectsVector.end(), newSubject) != subjectsVector.end()) {
			QMessageBox::warning(this, "Error", "Subject already exists.");
			return;
		}

		this->subjectsVector.push_back(newSubject); //Adding the subject to the vector
		ui.subjectsComboBox->addItem(newSubject); //Adding the subject to the combo box
	}
}

StudySphere::~StudySphere()
{
	this->saveToJSON(); //Saving the flash cards to a JSON file
}
