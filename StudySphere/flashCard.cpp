#include "flashCard.h"


//Function to get the subject of the flashcard (for showing flash cards from certain subject)
std::string flashCard::getSubject()
{
	return this->subject;
}

//Function to set the subject of the flashcard
void flashCard::setSubject(std::string sub)
{
	this->subject = sub;
}

//Constructor
flashCard::flashCard()
{
}

//Destructor
flashCard::~flashCard()
{

}

