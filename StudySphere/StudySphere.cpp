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

StudySphere::StudySphere(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.calendarFrame->hide();
   
    

    connect(ui.backFromInfo, &QPushButton::clicked, this, &StudySphere::on_backFromInfo_clicked);
  
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

        if (!name.isEmpty()) {
            QTextCharFormat format;
            format.setBackground(Qt::cyan);
            format.setFontWeight(QFont::Bold);
            ui.calendarWidget->setDateTextFormat(selectedDate, format);
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

        if (!name.isEmpty()) {
            QTextCharFormat format;
            format.setBackground(Qt::cyan);
            format.setFontWeight(QFont::Bold);
            ui.calendarWidget->setDateTextFormat(selectedDate, format);

        }
    }
}

void StudySphere::on_backFromInfo_clicked()
{
    ui.infoFrame->hide();
}
