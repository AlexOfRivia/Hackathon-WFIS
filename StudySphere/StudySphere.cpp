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

StudySphere::StudySphere(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.infoFrame->hide();
    ui.calendarFrame->hide();
    
    // Make the table editable
    ui.infoTable->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    
    // Connect signals and slots
    connect(ui.backFromInfo, &QPushButton::clicked, this, &StudySphere::on_backFromInfo_clicked);
    connect(ui.addSubjectButton, &QPushButton::clicked, this, &StudySphere::on_addSubjectButton_clicked);
    connect(ui.addExamButton, &QPushButton::clicked, this, &StudySphere::on_addExamButton_clicked);
    connect(ui.calendarWidget, &QCalendarWidget::clicked, this, &StudySphere::on_calendarWidget_clicked);
    connect(ui.infoTable, &QTableWidget::cellChanged, this, &StudySphere::on_tableCellChanged);

    // Sample data for testing
    std::vector<QString> jsonFiles = getJsonFiles("2025-03-21");
    saveToJson("2025-12-01", "Biology", "17:00", "18:00","Focus on histopatology", "JsonStronghold/2025-12-01 Biology.json");
    saveToJson("2025-12-02", "Math", "13:00", "18:00","A220","No", "Focus on equations", "JsonStronghold/2025-12-02 Math.json");
    saveToJson("2025-12-03", "Linear Algebry", "17:30", "18:00", "b193", "Yes", "Hard", "JsonStronghold/2025-12-03 Linear Algebry.json");
    saveToJson("2025-12-04", "C++ programing ", "19:00", "20:00", "Overloading", "JsonStronghold/2025-12-04 C++ programing.json");
    
    // Populate table with initial data
    populateTableFromJson(ui.infoTable, jsonFiles);
    
    // Highlight dates with data
    highlightDatesWithData();
}

StudySphere::~StudySphere()
{}


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
    format.setBackground(QColor(173, 216, 230)); // Light blue background
    
    // Directory path
    QString path = "JsonStronghold/";
    QDir dir(path);
    QStringList filters;
    filters << "*"; // All files
    dir.setNameFilters(filters);
    dir.setFilter(QDir::Files);
    
    foreach(QString file, dir.entryList()) {
        // Extract date from filename (assuming format starts with YYYY-MM-DD)
        QRegularExpression regex("(\\d{4}-\\d{2}-\\d{2})");
        QRegularExpressionMatch match = regex.match(file);
        
        if (match.hasMatch()) {
            QString dateStr = match.captured(1);
            QDate date = QDate::fromString(dateStr, "yyyy-MM-dd");
            if (date.isValid()) {
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
    } else {
        // Show a message that there's no data for this date
        QMessageBox::information(this, "No Records", 
            "No records found for " + date.toString("yyyy-MM-dd"));
    }
}

void StudySphere::on_tableCellChanged(int row, int column)
{
    // Only process if there are current JSON files and row is valid
    if (row >= 0 && row < ui.infoTable->rowCount() && !currentJsonFiles.empty()) {
        updateJsonFromTable(row);
    }
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

    QLineEdit *nameEdit = new QLineEdit(&dialog);
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

    formLayout.addRow("Subject Name:", nameEdit);
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
        QString name = nameEdit->text();
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

    QLineEdit *nameEdit = new QLineEdit(&dialog);
    QTimeEdit *startTimeEdit = new QTimeEdit(&dialog);
    QTimeEdit *endTimeEdit = new QTimeEdit(&dialog);

    startTimeEdit->setDisplayFormat("HH:mm");
    endTimeEdit->setDisplayFormat("HH:mm");

    startTimeEdit->setTime(QTime(8, 0));
    endTimeEdit->setTime(QTime(9, 0));

    QTextEdit *notesEdit = new QTextEdit(&dialog);
    notesEdit->setMaximumHeight(100);

    formLayout.addRow("Subject Name:", nameEdit);
    formLayout.addRow("Start Time:", startTimeEdit);
    formLayout.addRow("End Time:", endTimeEdit);
    formLayout.addRow("Notes:", notesEdit);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                              Qt::Horizontal, &dialog);
    formLayout.addRow(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString name = nameEdit->text();
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
    QStringList headers = { "Date", "Name", "Start Time", "End Time", "Room", "Is Retake", "Note" };
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

        // Populate the row with data from the JSON object
        tableWidget->setItem(row, 0, new QTableWidgetItem(jsonObj["date"].toString()));
        tableWidget->setItem(row, 1, new QTableWidgetItem(jsonObj["name"].toString()));
        tableWidget->setItem(row, 2, new QTableWidgetItem(jsonObj["startTime"].toString()));
        tableWidget->setItem(row, 3, new QTableWidgetItem(jsonObj["endTime"].toString()));
        
        int SubjectCaunt = 0;

        // Handle optional fields
        if (jsonObj.contains("room")) {
            tableWidget->setItem(row, 4, new QTableWidgetItem(jsonObj["room"].toString()));
            SubjectCaunt--;
        }
        
        if (jsonObj.contains("isRetake")) {
            tableWidget->setItem(row, 5, new QTableWidgetItem(jsonObj["isRetake"].toString()));
            SubjectCaunt--;
        }
        
        if (jsonObj.contains("note")) {
            tableWidget->setItem(row, 6, new QTableWidgetItem(jsonObj["note"].toString()));
        }
        if (SubjectCaunt==-2) {
            QColor backgroundColor("#22092C"); 
            QColor textColor("#FFFFFF");      
            for (int col = 0; col < tableWidget->columnCount(); ++col) {
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
            for (int col = 0; col < tableWidget->columnCount(); ++col) {
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
