#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#include "StudySphere.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QToolTip>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QTextEdit>
#include <QDialogButtonBox>
#include <QTimeEdit>
#include <QCheckBox>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>
#include <QTableWidget>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QString>
#include <QDir>
#include <QRegularExpression>
#include <QTextCharFormat>
#include <QPushButton>
#include <QColorDialog>

StudySphere::StudySphere(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    
    // Initialize calendar functionality
    ui.infoFrame->hide();
    ui.calendarFrame->hide();
    
    // Make the table editable
    ui.infoTable->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    
    // Connect signals and slots for calendar functionality
    connect(ui.backFromInfo, &QPushButton::clicked, this, &StudySphere::on_backFromInfo_clicked);
    connect(ui.addSubjectButton, &QPushButton::clicked, this, &StudySphere::on_addSubjectButton_clicked);
    connect(ui.addExamButton, &QPushButton::clicked, this, &StudySphere::on_addExamButton_clicked);
    connect(ui.calendarWidget, &QCalendarWidget::clicked, this, &StudySphere::on_calendarWidget_clicked);
    connect(ui.infoTable, &QTableWidget::cellChanged, this, &StudySphere::on_tableCellChanged);

    // Initialize flashcard functionality
    this->loadFromJSON(); //loading the flash cards from a JSON file
    ui.calendarFrame->setParent(ui.mainScreenFrame); //setting the calendar frame as a child of the central widget 
    ui.flashCard->setParent(ui.flashCardFrame); //setting the flash card as a child of the flash card frame
    ui.didntGetIt->setParent(ui.flashCardFrame); //setting the didnt get it button as a child of the flash card frame
    ui.gotIt->setParent(ui.flashCardFrame); //setting the got it button as a child of the flash card frame
    ui.seeFlashCardsButton->setParent(ui.flashCardFrame); //setting the see flash cards button as a child of the central widget
    ui.flashCard->hide(); //hide the flash card
    ui.didntGetIt->hide(); //hide the didnt get it button
    ui.gotIt->hide(); //hide the got it button
    ui.showAnswerButton->hide(); //hide the show answer button
    ui.showAnswerButton->setParent(ui.flashCardFrame); //setting the show answer button as a child of the flash card frame
    ui.calendarWidget->setParent(ui.calendarFrame); //setting the calendar widget as a child of the calendar frame
    ui.flashCardFrame->hide(); //hide the flash card frame

    ui.calendarWidget->setStyleSheet(calendarStyleSheet); //setting the style sheet of the calendar widget
    
    ui.wrongLabel->setParent(ui.flashCardFrame); //setting the wrong label as a child of the flash card frame)
    ui.rightLabel->setParent(ui.flashCardFrame); //setting the right label as a child of the flash card frame
    ui.wrongLabel->hide(); //hide the wrong label
    ui.rightLabel->hide(); //hide the right label

    ui.wrongLabel->setAlignment(Qt::AlignCenter); //set the alignment of the wrong label to center
    ui.rightLabel->setAlignment(Qt::AlignCenter); //set the alignment of the right label to center

    ui.wrongLabel->setStyleSheet("QLabel { color : #cbc5c5; }"); 
    ui.rightLabel->setStyleSheet("QLabel { color : #cbc5c5; }");

    // Enable word wrapping for qaLabel
    ui.qaLabel->setWordWrap(true);
    ui.qaLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);


    // Connect signals and slots for flashcard functionality
    QObject::connect(
        ui.addNewSubjectButton, &QPushButton::clicked, this, &StudySphere::addNewSubject //connecting the add subject button to the addNewSubject function
    );

    QObject::connect(
        ui.addFlashCardButton, &QPushButton::clicked, this, &StudySphere::addFlashCard //connecting the add flash card button to the addFlashCard function
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
        ui.backButton2, &QPushButton::clicked, this, &StudySphere::hideFlashCards //connecting the back button to the hideFlashCards function
    );

    QObject::connect(
        ui.backButton, &QPushButton::clicked, this, &StudySphere::hideCalendar //connecting the back button to the hideCalendar function
    );

    // Sample data for testing calendar functionality
    std::vector<QString> jsonFiles = getJsonFiles("2025-03-21");
    
        // Populate table with initial data
    populateTableFromJson(ui.infoTable, jsonFiles);
    
    // Highlight dates with data
    highlightDatesWithData();
}

StudySphere::~StudySphere()
{
    this->saveToJSON(); //Saving the flash cards to a JSON file
}

//Showing the color picker
void StudySphere::showColorPicker()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Select Color");
    if (color.isValid()) {
        // Do something with the selected color
        qDebug() << "Selected color:" << color.name();
    }
}

//Function to add a flash card
void StudySphere::addFlashCard()
{
    if (subjectsVector.empty())
    {
        QMessageBox::warning(this, "Error", "You have no subjects.");
        return;
    }
    else {
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

        comboBox.setCurrentIndex(ui.subjectsComboBox->currentIndex()); //Setting the current index of the combo box to the subjectComboBox
        layout.addWidget(&comboBox); //Adding the line edit to the layout
        layout.addWidget(new QLabel("Question: ", &dialog));
        layout.addWidget(&questionLineEdit); //Adding the line edit to the layout
        layout.addWidget(new QLabel("Answer: ", &dialog));
        layout.addWidget(&answerLineEdit); //Adding the line edit to the layout
        layout.addWidget(okButton); //Adding the OK button to the layout

        QObject::connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept); //Connecting the OK button to accept the dialog

        if (dialog.exec() == QDialog::Accepted) //Executing the dialog and checking if it was accepted
        {
            if (questionLineEdit.text().isEmpty() || answerLineEdit.text().isEmpty()) {
                QMessageBox::warning(this, "Error", "Question and Answer fields cannot be empty.");
                return;
            }

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
        // checkbox, question, answer, subject
        // Clicking the delete button will delete those flash cards where the checkbox is signed
        QDialog dialog(this); // Creating a dialog box
        dialog.setWindowTitle("Flash Cards"); // Setting the title of the dialog box
        QVBoxLayout layout(&dialog); // Creating a layout for the dialog
        QTableWidget tableWidget(&dialog); // Creating a table widget
        tableWidget.setColumnCount(4); // Setting the column count of the table
        tableWidget.setHorizontalHeaderLabels({ "Select", "Question", "Answer", "Subject" }); // Setting the horizontal header labels of the table
        tableWidget.setEditTriggers(QAbstractItemView::NoEditTriggers); // Setting the edit triggers of the table
        tableWidget.setSelectionBehavior(QAbstractItemView::SelectRows); // Setting the selection behavior of the table
        tableWidget.setSelectionMode(QAbstractItemView::SingleSelection); // Setting the selection mode of the table
        for (int i = 0; i < flashCardsVector.size(); i++) // Looping through the flash cards vector
        {
            tableWidget.insertRow(tableWidget.rowCount()); // Inserting a row in the table
            QTableWidgetItem* checkBoxItem = new QTableWidgetItem(); // Creating a new table widget item for the checkbox
            checkBoxItem->setCheckState(Qt::Unchecked); // Setting the checkbox state to unchecked
            tableWidget.setItem(tableWidget.rowCount() - 1, 0, checkBoxItem); // Adding the checkbox to the table
            tableWidget.setItem(tableWidget.rowCount() - 1, 1, new QTableWidgetItem(QString::fromStdString(flashCardsVector[i].getQuestion()))); // Adding the question of the flash card to the table
            tableWidget.setItem(tableWidget.rowCount() - 1, 2, new QTableWidgetItem(QString::fromStdString(flashCardsVector[i].getAnswer()))); // Adding the answer of the flash card to the table
            tableWidget.setItem(tableWidget.rowCount() - 1, 3, new QTableWidgetItem(QString::fromStdString(flashCardsVector[i].getSubject()))); // Adding the subject of the flash card to the table
        }
        layout.addWidget(&tableWidget); // Adding the table widget to the layout
        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog); // Creating a button box
        layout.addWidget(&buttonBox); // Adding the button box to the layout
        QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept); // Connecting the button box to accept the dialog
        QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject); // Connecting the button box to reject the dialog
        if (dialog.exec() == QDialog::Accepted) // Executing the dialog and checking if it was accepted
        {
            std::vector<int> indicesToDelete;
            for (int i = 0; i < tableWidget.rowCount(); i++)
            {
                if (tableWidget.item(i, 0)->checkState() == Qt::Checked)
                {
                    indicesToDelete.push_back(i);
                }
            }
            // Erase the flash cards from the vector in reverse order to avoid invalidating indices
            for (int i = indicesToDelete.size() - 1; i >= 0; i--)
            {
                flashCardsVector.erase(flashCardsVector.begin() + indicesToDelete[i]);
                //checking if the deleted flash card matches the current subject to remove it from the temporary flash cards vector
                if (temporaryFlashCards.size() > 0 && temporaryFlashCards[index].getQuestion() == tableWidget.item(indicesToDelete[i], 1)->text().toStdString())
                {
                    temporaryFlashCards.erase(temporaryFlashCards.begin() + index);
                }
            }
            // hiding the flash card
            ui.flashCard->hide();
            ui.didntGetIt->hide();
            ui.gotIt->hide();
            ui.showAnswerButton->hide();
            ui.wrongLabel->hide(); //hide the wrong label
            ui.rightLabel->hide(); //hide the right label
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
    }

    // Reset the counters
    this->wrongQuestions = 0;
    this->rightQuestions = 0;
    ui.wrongLabel->setText(QString::number(wrongQuestions)); 
    ui.rightLabel->setText(QString::number(rightQuestions)); 

    // Show the flash card UI elements
    ui.flashCard->show();
    ui.didntGetIt->show();
    ui.gotIt->show();
    ui.showAnswerButton->show();
    ui.wrongLabel->show(); 
    ui.rightLabel->show(); 

    // Clear the temporary flash cards vector
    temporaryFlashCards.clear();

    // Populate the temporary flash cards vector based on the selected subject
    std::string selectedSubject = ui.subjectsComboBox->currentText().toStdString();
    for (const auto& flashCardObj : flashCardsVector)
    {
        if (flashCardObj.getSubject() == selectedSubject)
        {
            temporaryFlashCards.push_back(flashCardObj);
        }
    }

    // Check if there are any flash cards for the selected subject
    if (temporaryFlashCards.empty())
    {
        QMessageBox::warning(this, "Error", "No flash cards found for the selected subject.");
        ui.flashCard->hide();
        ui.didntGetIt->hide();
        ui.gotIt->hide();
        ui.showAnswerButton->hide();
        ui.wrongLabel->hide();
        ui.rightLabel->hide();
        return;
    }

    // Reset the index
    this->index = 0;

    // Set the alignment of the label to center
    ui.qaLabel->setAlignment(Qt::AlignCenter);

    // Set the question of the first flash card in the temporary vector to the label
    ui.qaLabel->setText(QString::fromStdString(temporaryFlashCards[index].getQuestion()));
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
    if (temporaryFlashCards.empty()) {
        QMessageBox::warning(this, "Error", "No flash cards available.");
        return;
    }

    this->index++; // Incrementing the index
    if (index >= temporaryFlashCards.size())
    {
        this->index = 0;
    }

    //Set the alignment of the label to center
    ui.qaLabel->setAlignment(Qt::AlignCenter);
    ui.qaLabel->setText(QString::fromStdString(temporaryFlashCards[index].getQuestion())); // Setting the question of the flash card to the label

    switch (wasRight)
    {
    case true:
        this->rightQuestions += 1; //increment the right questions
        ui.rightLabel->setText(QString::number(rightQuestions)); //update the right label
        break;
    case false:
        this->wrongQuestions += 1; //increment the wrong questions
        ui.wrongLabel->setText(QString::number(wrongQuestions)); //update the wrong label
        break;
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

        if (newSubject.isEmpty()) {
            QMessageBox::warning(this, "Error", "Subject name cannot be empty.");
            return;
        }

        //Check if the subject already exists
        if (std::find(subjectsVector.begin(), subjectsVector.end(), newSubject) != subjectsVector.end()) {
            QMessageBox::warning(this, "Error", "Subject already exists.");
            return;
        }

        this->subjectsVector.push_back(newSubject); //Adding the subject to the vector
        ui.subjectsComboBox->addItem(newSubject); //Adding the subject to the combo box
    }
}

std::vector<QString> StudySphere::getJsonFiles(const std::string& date)
{
    std::vector<QString> jsonFiles;
    std::regex dateRegex(date + ".*");
    QString path = "JsonStronghold/";

    #ifdef _WIN32
    // Use QDir for Windows too - this avoids character conversion issues
    QDir dir(path);
    QStringList entries = dir.entryList(QDir::Files);
    
    for (const QString& filename : entries) {
        std::string filenameStr = filename.toStdString();
        if (std::regex_match(filenameStr, dateRegex)) {
            jsonFiles.push_back(path + filename);
        }
    }
    #else
    DIR* dir = opendir(path.toStdString().c_str());
    if (dir != nullptr) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            std::string filename(entry->d_name);
            if (filename != "." && filename != "..") {
                struct stat statbuf;
                std::string fullPath = path.toStdString() + filename;
                if (stat(fullPath.c_str(), &statbuf) == 0 && 
                    S_ISREG(statbuf.st_mode) && 
                    std::regex_match(filename, dateRegex)) {
                    jsonFiles.push_back(path + QString::fromStdString(filename));
                }
            }
        }
        closedir(dir);
    }
    #endif

    return jsonFiles;
}

void StudySphere::highlightDatesWithData()
{
    QTextCharFormat format;
	//making highlight visible despite the stylesheet
    format.setForeground(QColor("#A1D6B2")); //Blue Text
	format.setFontWeight(QFont::Bold);
    //format.setBackground(QColor(173, 216, 230)); // Light blue background
	format.setFontPointSize(15);

    // Directory path
    QString path = "JsonStronghold/";
    QDir dir(path);
    QStringList filters;
    filters << "*"; // All files
    dir.setNameFilters(filters);
    dir.setFilter(QDir::Files);
    
    // First, clear all highlights by setting all dates to default format
    QDate startDate = QDate(QDate::currentDate().year(), 1, 1);
    QDate endDate = QDate(QDate::currentDate().year(), 12, 31);
    for (QDate date = startDate; date <= endDate; date = date.addDays(1)) {
        ui.calendarWidget->setDateTextFormat(date, QTextCharFormat());
    }
    
    // Then add highlights for dates with files
    foreach(QString file, dir.entryList()) {
        // Extract date from filename (assuming format starts with YYYY-MM-DD)
        QRegularExpression regex("(\\d{4}-\\d{2}-\\d{2})");
        QRegularExpressionMatch match = regex.match(file);
        
        if (match.hasMatch()) {
            QString dateStr = match.captured(1);
            QDate date = QDate::fromString(dateStr, "yyyy-MM-dd");
            if (date.isValid() && hasEntriesForDate(dateStr)) {
                ui.calendarWidget->setDateTextFormat(date, format);
            }
        }
    }
}


void StudySphere::on_calendarWidget_clicked(const QDate& date)
{
    // Check if there are any JSON files for this date
    std::string dateStr = date.toString("yyyy-MM-dd").toStdString();
    std::vector<QString> jsonFiles = getJsonFiles(dateStr);
    
    // Store the current JSON files for editing
    currentJsonFiles = jsonFiles;
    
    // Only show the info frame if there are files
    if (!jsonFiles.empty()) {
        // Temporarily block signals to prevent triggering cellChanged during population
        ui.infoTable->blockSignals(true);
        
        // Populate the table with the JSON data
        populateTableFromJson(ui.infoTable, jsonFiles);
        
        // Re-enable signals
        ui.infoTable->blockSignals(false);
        
        // Show the info frame
        ui.infoFrame->show();
    }
}

void StudySphere::on_tableCellChanged(int row, int column)
{
    // Skip update if the row is the Delete or Flash Cards button columns
    if (column < 2)
        return;
    
    // Skip update if row is invalid
    if (row < 0 || row >= ui.infoTable->rowCount() || row >= currentJsonFiles.size())
        return;
    
    // Update the JSON file
    updateJsonFromTableRow(row);
}
void StudySphere::updateJsonFromTable(int row)
{
    // Get the JSON file path for this row
    if (row >= currentJsonFiles.size()) {
        qWarning() << "Row index out of bounds for JSON files";
        return;
    }
    
    QString filePath = currentJsonFiles[row];
    
    // Read the current JSON
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open file for update:" << filePath;
        return;
    }
    
    QByteArray jsonData = file.readAll();
    file.close();
    
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (jsonDoc.isNull()) {
        qWarning() << "Failed to parse JSON in file:" << filePath;
        return;
    }
    
    QJsonObject jsonObj = jsonDoc.object();
    
    // Update the JSON object with values from the table
    if (ui.infoTable->item(row, 0)) // Date
        jsonObj["date"] = ui.infoTable->item(row, 0)->text();
    
    if (ui.infoTable->item(row, 1)) // Name
        jsonObj["name"] = ui.infoTable->item(row, 1)->text();
    
    if (ui.infoTable->item(row, 2)) // Start Time
        jsonObj["startTime"] = ui.infoTable->item(row, 2)->text();
    
    if (ui.infoTable->item(row, 3)) // End Time
        jsonObj["endTime"] = ui.infoTable->item(row, 3)->text();
    
    if (ui.infoTable->item(row, 4)) // Room (optional)
        jsonObj["room"] = ui.infoTable->item(row, 4)->text();
    
    if (ui.infoTable->item(row, 5)) // Is Retake (optional)
        jsonObj["isRetake"] = ui.infoTable->item(row, 5)->text();
    
    if (ui.infoTable->item(row, 6)) // Note (optional)
        jsonObj["note"] = ui.infoTable->item(row, 6)->text();
    
    // Write the updated JSON back to the file
    jsonDoc.setObject(jsonObj);
    
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Could not open file for writing:" << filePath;
        return;
    }
    
    file.write(jsonDoc.toJson(QJsonDocument::Indented));
    file.close();
    
    qDebug() << "Updated JSON file:" << filePath;
}

void StudySphere::on_addExamButton_clicked()
{
    QDate selectedDate = ui.calendarWidget->selectedDate();

    QDialog dialog(this);
    dialog.setWindowTitle("Add Exam for " + selectedDate.toString("yyyy-MM-dd"));

    QFormLayout formLayout(&dialog);
	QComboBox *subjectCB = new QComboBox(&dialog);
    QLineEdit *roomEdit = new QLineEdit(&dialog);
    QCheckBox *isRetakeEdit = new QCheckBox(&dialog);
    QTimeEdit *startTimeEdit = new QTimeEdit(&dialog);
    QTimeEdit *endTimeEdit = new QTimeEdit(&dialog);

    startTimeEdit->setDisplayFormat("HH:mm");
    endTimeEdit->setDisplayFormat("HH:mm");

    startTimeEdit->setTime(QTime(8, 0));
    endTimeEdit->setTime(QTime(9, 0));

    QTextEdit *notesEdit = new QTextEdit(&dialog);
    notesEdit->setMaximumHeight(100);

	// Add subjects to the combo box
	for (int i = 0; i < subjectsVector.size(); i++)
	{
		subjectCB->addItem(subjectsVector[i]);
	}

    formLayout.addRow("Subject Name:", subjectCB);
    formLayout.addRow("Room:", roomEdit);
    formLayout.addRow("Is it a retake:", isRetakeEdit);
    formLayout.addRow("Start Time:", startTimeEdit);
    formLayout.addRow("End Time:", endTimeEdit);
    formLayout.addRow("Notes:", notesEdit);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                              Qt::Horizontal, &dialog);
    formLayout.addRow(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString name = subjectCB->currentText();
        QString notes = notesEdit->toPlainText();
        QTime startTime = startTimeEdit->time();
        QTime endTime = endTimeEdit->time();
        QString room = roomEdit->text();
        QString isRetake = isRetakeEdit->isChecked() ? "Yes" : "No";

        std::string filePath = "JsonStronghold/" + selectedDate.toString("yyyy-MM-dd").toStdString() +  " " + name.toStdString() + ".json";
        saveToJson(selectedDate.toString("yyyy-MM-dd").toStdString(), name.toStdString(), startTime.toString("HH:mm").toStdString(), endTime.toString("HH:mm").toStdString(), room.toStdString(), isRetake.toStdString(), notes.toStdString(), filePath);
        
        // Refresh the highlighted dates
        highlightDatesWithData();
        
        // If current date is displayed in the table, refresh it
        QDate currentDisplayedDate;
        if (ui.infoTable->rowCount() > 0 && ui.infoTable->item(0, 0)) {
            currentDisplayedDate = QDate::fromString(ui.infoTable->item(0, 0)->text(), "yyyy-MM-dd");
        }
        
        if (currentDisplayedDate.isValid() && currentDisplayedDate == selectedDate) {
            std::vector<QString> jsonFiles = getJsonFiles(selectedDate.toString("yyyy-MM-dd").toStdString());
            currentJsonFiles = jsonFiles;
            
            // Block signals during update
            ui.infoTable->blockSignals(true);
            populateTableFromJson(ui.infoTable, jsonFiles);
            ui.infoTable->blockSignals(false);
        }
    }
}

void StudySphere::on_addSubjectButton_clicked()
{
    QDate selectedDate = ui.calendarWidget->selectedDate();

    QDialog dialog(this);
    dialog.setWindowTitle("Add Subject for " + selectedDate.toString("yyyy-MM-dd"));

    QFormLayout formLayout(&dialog);

    // Use a combo box instead of a line edit
    QComboBox* subjectCB = new QComboBox(&dialog);
    QTimeEdit *startTimeEdit = new QTimeEdit(&dialog);
    QTimeEdit *endTimeEdit = new QTimeEdit(&dialog);

    // Add subjects to the combo box
    for (int i = 0; i < subjectsVector.size(); i++)
    {
        subjectCB->addItem(subjectsVector[i]);
    }

    startTimeEdit->setDisplayFormat("HH:mm");
    endTimeEdit->setDisplayFormat("HH:mm");

    startTimeEdit->setTime(QTime(8, 0));
    endTimeEdit->setTime(QTime(9, 0));

    QTextEdit *notesEdit = new QTextEdit(&dialog);
    notesEdit->setMaximumHeight(100);

    formLayout.addRow("Subject:", subjectCB);
    formLayout.addRow("Start Time:", startTimeEdit);
    formLayout.addRow("End Time:", endTimeEdit);
    formLayout.addRow("Notes:", notesEdit);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                              Qt::Horizontal, &dialog);
    formLayout.addRow(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString name = subjectCB->currentText();
        QString notes = notesEdit->toPlainText();
        QTime startTime = startTimeEdit->time();
        QTime endTime = endTimeEdit->time();

        std::string filePath = "JsonStronghold/" + selectedDate.toString("yyyy-MM-dd").toStdString() + " " +  name.toStdString() + ".json";
        saveToJson(selectedDate.toString("yyyy-MM-dd").toStdString(), name.toStdString(), startTime.toString("HH:mm").toStdString(), endTime.toString("HH:mm").toStdString(), notes.toStdString(), filePath);
        
        // Refresh the highlighted dates
        highlightDatesWithData();
        
        // If current date is displayed in the table, refresh it
        QDate currentDisplayedDate;
        if (ui.infoTable->rowCount() > 0 && ui.infoTable->item(0, 0)) {
            currentDisplayedDate = QDate::fromString(ui.infoTable->item(0, 0)->text(), "yyyy-MM-dd");
        }
        
        if (currentDisplayedDate.isValid() && currentDisplayedDate == selectedDate) {
            std::vector<QString> jsonFiles = getJsonFiles(selectedDate.toString("yyyy-MM-dd").toStdString());
            currentJsonFiles = jsonFiles;
            
            // Block signals during update
            ui.infoTable->blockSignals(true);
            populateTableFromJson(ui.infoTable, jsonFiles);
            ui.infoTable->blockSignals(false);
        }
    }
}


void StudySphere::on_backFromInfo_clicked()
{
    ui.infoFrame->hide();
    ui.calendarFrame->show();
}

void StudySphere::saveToJson(const std::string& date, const std::string& name, const std::string& startTime, const std::string& endTime, const std::string& note, const std::string& filename)
{
    std::string jsonContent = "{\n";
    jsonContent += "    \"date\": \"" + date + "\",\n";
    jsonContent += "    \"name\": \"" + name + "\",\n";
    jsonContent += "    \"startTime\": \"" + startTime + "\",\n";
    jsonContent += "    \"endTime\": \"" + endTime + "\",\n";
    jsonContent += "    \"note\": \"" + note + "\"\n";
    jsonContent += "}";

    std::ofstream file(filename);
    if (file.is_open()) {
        file << jsonContent;
        file.close();
        std::cout << "Strings saved to " << filename << std::endl;
    }
    else {
        std::cerr << "Failed to open file: " << filename << std::endl;
    }
}

void StudySphere::saveToJson(const std::string& date, const std::string& name, const std::string& startTime, const std::string& endTime, const std::string& room, const std::string& isRetake, const std::string& note, const std::string& filename)
{
    std::string jsonContent = "{\n";
    jsonContent += "    \"date\": \"" + date + "\",\n";
    jsonContent += "    \"name\": \"" + name + "\",\n";
    jsonContent += "    \"startTime\": \"" + startTime + "\",\n";
    jsonContent += "    \"endTime\": \"" + endTime + "\",\n";
    jsonContent += "    \"room\": \"" + room + "\",\n";
    jsonContent += "    \"isRetake\": \"" + isRetake + "\",\n";
    jsonContent += "    \"note\": \"" + note + "\"\n";
    jsonContent += "}";

    std::ofstream file(filename);
    if (file.is_open()) {
        file << jsonContent;
        file.close();
        std::cout << "Strings saved to " << filename << std::endl;
    }
    else {
        std::cerr << "Failed to open file: " << filename << std::endl;
    }
}

void StudySphere::populateTableFromJson(QTableWidget* tableWidget, const std::vector<QString>& jsonFiles) {
    // Clear the table
    tableWidget->clear();
    tableWidget->setRowCount(0);
    tableWidget->setColumnCount(0);

    // Determine the columns based on the JSON structure
    QStringList headers = { "Delete","Flash Cards","Date", "Name", "Start Time", "End Time", "Room", "Is Retake", "Note"};
    tableWidget->setColumnCount(headers.size());
    tableWidget->setHorizontalHeaderLabels(headers);

    // Iterate over each JSON file
    for (const QString& filePath : jsonFiles) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Could not open file:" << filePath;
            continue;
        }

        QByteArray jsonData = file.readAll();
        file.close();

        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
        if (jsonDoc.isNull()) {
            qWarning() << "Failed to parse JSON in file:" << filePath;
            continue;
        }

        QJsonObject jsonObj = jsonDoc.object();

        // Create a new row in the table
        int row = tableWidget->rowCount();
        tableWidget->insertRow(row);

        QPushButton* deleteButton = new QPushButton("Del");
        QPushButton* goToFlashCards = new QPushButton("Study");

        // Connect the delete button using a lambda function - no confirmation
        connect(deleteButton, &QPushButton::clicked, [this, row]() {
            deleteTableRow(row);
        });
        
        // Connect the study button to move to flashcard screen with subject selected
        connect(goToFlashCards, &QPushButton::clicked, [this, row, tableWidget]() {
            QString subjectName = tableWidget->item(row, 3)->text();
            // Find matching subject in combo box
            int index = ui.subjectsComboBox->findText(subjectName);
            if (index != -1) {
                // Set the selected subject in the combo box
                ui.subjectsComboBox->setCurrentIndex(index);
                
                // Hide the calendar and info frames
                ui.infoFrame->hide();
                ui.calendarFrame->hide();
                
                // Show the flashcard frame
                ui.flashCardFrame->show();
                
                // Start study mode
                this->study();
            } else {
                QMessageBox::warning(this, "Error", "No flashcards found for this subject. Please add some first.");
            }
        });

        // Populate the row with data from the JSON object
        tableWidget->setCellWidget(row, 0, deleteButton);
        tableWidget->setCellWidget(row, 1, goToFlashCards);
        tableWidget->setItem(row, 2, new QTableWidgetItem(jsonObj["date"].toString()));
        tableWidget->setItem(row, 3, new QTableWidgetItem(jsonObj["name"].toString()));
        tableWidget->setItem(row, 4, new QTableWidgetItem(jsonObj["startTime"].toString()));
        tableWidget->setItem(row, 5, new QTableWidgetItem(jsonObj["endTime"].toString()));
        
        int SubjectCaunt = 0;

        // Handle optional fields
        if (jsonObj.contains("room")) {
            tableWidget->setItem(row, 6, new QTableWidgetItem(jsonObj["room"].toString()));
            SubjectCaunt--;
        }
        
        if (jsonObj.contains("isRetake")) {
            tableWidget->setItem(row, 7, new QTableWidgetItem(jsonObj["isRetake"].toString()));
            SubjectCaunt--;
        }
        
        if (jsonObj.contains("note")) {
            tableWidget->setItem(row, 8, new QTableWidgetItem(jsonObj["note"].toString()));
        }
        if (SubjectCaunt==-2) {
            QColor backgroundColor("#22092C"); 
            QColor textColor("#FFFFFF");      
            for (int col = 2; col < tableWidget->columnCount(); ++col) {
                QTableWidgetItem* item = tableWidget->item(row, col);
                if (item) {
                    item->setBackground(backgroundColor);
                    item->setForeground(textColor);
                }
            }
        }
        else {
            QColor backgroundColor("#872341");
            QColor textColor("#FFFFFF");
            for (int col = 2; col < tableWidget->columnCount(); ++col) {
                QTableWidgetItem* item = tableWidget->item(row, col);
                if (item) {
                    item->setBackground(backgroundColor);
                    item->setForeground(textColor);
                }
            }
        }
        tableWidget->horizontalHeader()->setStretchLastSection(true);
    }
}


void StudySphere::deleteTableRow(int row)
{
    if (row < 0 || row >= currentJsonFiles.size())
        return;
    
    // Get the file path
    QString filePath = currentJsonFiles[row];
    
    // Get the date from the table
    QString dateStr = ui.infoTable->item(row, 2)->text();
    
    // Delete the file directly without confirmation
    QFile file(filePath);
    if (file.remove()) {
        // Remove entry from vector
        currentJsonFiles.erase(currentJsonFiles.begin() + row);
        
        // Remove row from table
        ui.infoTable->removeRow(row);
        
        // Check if this was the last entry for this date and update calendar
        if (!hasEntriesForDate(dateStr)) {
            // If no more entries for this date, remove the highlighting
            QDate date = QDate::fromString(dateStr, "yyyy-MM-dd");
            if (date.isValid()) {
                // Reset the date format to default (no highlighting)
                ui.calendarWidget->setDateTextFormat(date, QTextCharFormat());
            }
        }
    } else {
        QMessageBox::warning(this, "Error", "Failed to delete file: " + filePath);
    }
}

bool StudySphere::hasEntriesForDate(const QString& dateStr)
{
    // Convert QString to std::string for getJsonFiles
    std::string dateStrStd = dateStr.toStdString();
    
    // Directory path
    QString path = "JsonStronghold/";
    
    // Create regex pattern for files matching this date
    std::regex dateRegex(dateStrStd + ".*\\.json$");
    
    // Use QDir to search for files
    QDir dir(path);
    QStringList entries = dir.entryList(QDir::Files);
    
    // Check if any files match the pattern
    for (const QString& filename : entries) {
        std::string filenameStr = filename.toStdString();
        if (std::regex_match(filenameStr, dateRegex)) {
            return true; // Found at least one file
        }
    }
    
    return false; // No files found
}


void StudySphere::updateJsonFromTableRow(int row)
{
    // Only process if the row is valid and in range
    if (row < 0 || row >= currentJsonFiles.size()) {
        return;
    }
    
    // Get the file path for this row
    QString filePath = currentJsonFiles[row];
    
    // Read the current JSON
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open file for update:" << filePath;
        return;
    }
    
    QByteArray jsonData = file.readAll();
    file.close();
    
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (jsonDoc.isNull()) {
        qWarning() << "Failed to parse JSON in file:" << filePath;
        return;
    }
    
    QJsonObject jsonObj = jsonDoc.object();
    
    // Update the JSON object with values from the table
    if (ui.infoTable->item(row, 2)) // Date
        jsonObj["date"] = ui.infoTable->item(row, 2)->text();
    
    if (ui.infoTable->item(row, 3)) // Name
        jsonObj["name"] = ui.infoTable->item(row, 3)->text();
    
    if (ui.infoTable->item(row, 4)) // Start Time
        jsonObj["startTime"] = ui.infoTable->item(row, 4)->text();
    
    if (ui.infoTable->item(row, 5)) // End Time
        jsonObj["endTime"] = ui.infoTable->item(row, 5)->text();
    
    // Handle optional fields
    if (ui.infoTable->item(row, 6)) // Room
        jsonObj["room"] = ui.infoTable->item(row, 6)->text();
    else
        jsonObj.remove("room");
    
    if (ui.infoTable->item(row, 7)) // Is Retake
        jsonObj["isRetake"] = ui.infoTable->item(row, 7)->text();
    else
        jsonObj.remove("isRetake");
    
    if (ui.infoTable->item(row, 8)) // Note
        jsonObj["note"] = ui.infoTable->item(row, 8)->text();
    else
        jsonObj.remove("note");
    
    // Write the updated JSON back to the file
    jsonDoc.setObject(jsonObj);
    
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Could not open file for writing:" << filePath;
        return;
    }
    
    file.write(jsonDoc.toJson(QJsonDocument::Indented));
    file.close();
    
    qDebug() << "Updated JSON file:" << filePath;
}
