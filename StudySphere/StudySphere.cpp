#include "StudySphere.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QVBoxLayout>

StudySphere::StudySphere(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.calendarFrame->hide();
   
    

    connect(ui.addExamButton, &QPushButton::clicked, this, &StudySphere::on_addExamButton_clicked);
    connect(ui.addSubjectButton, &QPushButton::clicked, this, &StudySphere::on_addSubjectButton_clicked);
    connect(ui.backFromInfo, &QPushButton::clicked, this, &StudySphere::on_backFromInfo_clicked);
  
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
