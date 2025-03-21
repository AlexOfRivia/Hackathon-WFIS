#include "StudySphere.h"

StudySphere::StudySphere(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //setting the calendar frame as hidden
	ui.calendarFrame->hide();



}

StudySphere::~StudySphere()
{}
