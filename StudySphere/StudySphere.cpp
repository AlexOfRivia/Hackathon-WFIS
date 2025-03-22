#include "StudySphere.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <fstream>
#include <iostream>
#include <QTableWidget>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>


StudySphere::StudySphere(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.calendarFrame->hide();
   
    
    std::vector<QString> jsonFiles = {"JsonStronghold/output1.json","JsonStronghold/output2.json","JsonStronghold/output3.json" ,"JsonStronghold/output4.json" };
    connect(ui.addExamButton, &QPushButton::clicked, this, &StudySphere::on_addExamButton_clicked);
    connect(ui.addSubjectButton, &QPushButton::clicked, this, &StudySphere::on_addSubjectButton_clicked);
    connect(ui.backFromInfo, &QPushButton::clicked, this, &StudySphere::on_backFromInfo_clicked);
    saveToJson("2025-12-01", "Biology", "17:00", "18:00","Focus on histopatology", "JsonStronghold/output1.json");
    saveToJson("2025-12-02", "Math", "13:00", "18:00","A220","No", "Focus on equations", "JsonStronghold/output2.json");
    saveToJson("2025-12-03", "Linear Algebry", "17:30", "18:00", "b193", "Yes", "Hard", "JsonStronghold/output3.json");
    saveToJson("2025-12-04", "C++ programing ", "19:00", "20:00", "Overloading", "JsonStronghold/output4.json");
    populateTableFromJson(ui.infoTable, jsonFiles);
    
}

StudySphere::~StudySphere()
{}

void StudySphere::on_addExamButton_clicked()
{
    bool ok;
    QString examName = QInputDialog::getText(this, "Add Exam", 
                                         "Exam name:", QLineEdit::Normal,
                                         "", &ok);
    if (ok && !examName.isEmpty()) {
        // Do something with the exam name
        QMessageBox::information(this, "Success", "Added exam: " + examName);
    }
    
}
void StudySphere::on_addSubjectButton_clicked()
{
    QMessageBox::information(this, "Add Subject", "Add Subject button clicked.");

}

void StudySphere::on_backFromInfo_clicked()
{
    ui.infoFrame->hide();
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
    
        
    }
}