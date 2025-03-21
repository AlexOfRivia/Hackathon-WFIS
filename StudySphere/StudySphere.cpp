#include "StudySphere.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <fstream>
#include <iostream>


StudySphere::StudySphere(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.calendarFrame->hide();
   
    

    connect(ui.addExamButton, &QPushButton::clicked, this, &StudySphere::on_addExamButton_clicked);
    connect(ui.addSubjectButton, &QPushButton::clicked, this, &StudySphere::on_addSubjectButton_clicked);
    connect(ui.backFromInfo, &QPushButton::clicked, this, &StudySphere::on_backFromInfo_clicked);
    saveToJson("2025-12-01", "Biology", "17:00", "18:00","Focus on histopatology", "JsonStronghold/output1.json");
    saveToJson("2025-12-01", "Biology", "17:00", "18:00","A220","No", "Focus on histopatology", "JsonStronghold/output2.json");
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
    jsonContent += "    \"romm\": \"" + room + "\",\n";
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


