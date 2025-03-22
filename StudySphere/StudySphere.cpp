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

/* void StudySphere::on_calendarWidget_clicked(const QDate& date)
{
    ui.calendarFrame->hide();
    ui.infoFrame->show();

    std::string filePath = "JsonStronghold/" + date.toString("yyyy-MM-dd").toStdString();
    std::ifstream file(filePath);
    if (file.is_open()) {
        std::string line;
        std::string content;
        while (std::getline(file, line)) {
            content += line;
        }
        file.close();
        std::cout << "Strings read from " << filePath << std::endl;
        std::cout << content << std::endl;
    }
    else {
        std::cerr << "Failed to open file: " << filePath << std::endl;
    }
} */



std::vector<std::string> StudySphere::getJsonFiles(const std::string& date)
{
    std::vector<std::string> jsonFiles;
    std::regex dateRegex(date + ".*");
    std::string path = "JsonStronghold/";

    #ifdef _WIN32

    WIN32_FIND_DATA findData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    std::string searchPath = path + "*";

    hFind = FindFirstFile(searchPath.c_str(), &findData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                std::string filename(findData.cFileName);
                if (std::regex_match(filename, dateRegex)) {
                    jsonFiles.push_back(path + filename);
                }
            }
        } while (FindNextFile(hFind, &findData) != 0);
        FindClose(hFind);
    }
    #else

    DIR* dir = opendir(path.c_str());
    if (dir != nullptr) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            std::string filename(entry->d_name);

            if (filename != "." && filename != "..") {

                struct stat statbuf;
                std::string fullPath = path + filename;
                if (stat(fullPath.c_str(), &statbuf) == 0 && 
                    S_ISREG(statbuf.st_mode) && 
                    std::regex_match(filename, dateRegex)) {
                    jsonFiles.push_back(fullPath);
                }
            }
        }
        closedir(dir);
    }
    #endif

    return jsonFiles;
}

StudySphere::StudySphere(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.calendarFrame->hide();

    connect(ui.backFromInfo, &QPushButton::clicked, this, &StudySphere::on_backFromInfo_clicked);

    std::vector<std::string> jsonFiles = getJsonFiles("2025-03-21");

}

StudySphere::~StudySphere()
{}

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
        QString isRetake = isRetakeEdit->text();

        std::string filePath = "JsonStronghold/" + selectedDate.toString("yyyy-MM-dd").toStdString() +  name.toStdString();
        saveToJson(selectedDate.toString("yyyy-MM-dd").toStdString(), name.toStdString(), startTime.toString("HH:mm").toStdString(), endTime.toString("HH:mm").toStdString(), room.toStdString(), isRetake.toStdString(), notes.toStdString(), filePath);

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


        std::string filePath = "JsonStronghold/" + selectedDate.toString("yyyy-MM-dd").toStdString() + " " +  name.toStdString();
        saveToJson(selectedDate.toString("yyyy-MM-dd").toStdString(), name.toStdString(), startTime.toString("HH:mm").toStdString(), endTime.toString("HH:mm").toStdString(), notes.toStdString(), filePath);
    }
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


