#include "flashCard.h"

//Function to get the subject of the flashcard (for showing flash cards from certain subject)
std::string flashCard::getSubject() const
{
    return this->subject;
}

std::string flashCard::getQuestion() const
{
    return this->question;
}

std::string flashCard::getAnswer() const
{
    return this->answer;
}

//Function to set the subject of the flashcard
void flashCard::setSubject(std::string sub)
{
    this->subject = sub;
}

//Function to set the subject of the flashcard
void flashCard::setQuestion(std::string ques)
{
    this->question = ques;
}

//Function to set the subject of the flashcard
void flashCard::setAnswer(std::string ans)
{
    this->answer = ans;
}

//Constructor
flashCard::flashCard()
{
}

//Destructor
flashCard::~flashCard()
{
}
