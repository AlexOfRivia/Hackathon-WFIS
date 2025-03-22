#pragma once
#include <string>

class flashCard
{
public:
    flashCard(); //Constructor
    ~flashCard(); //Destructor
    std::string getSubject() const; //getting the flashcard subject
    std::string getQuestion() const; //getting the flashcard question
    std::string getAnswer() const; //getting the flashcard answer

    void setSubject(std::string sub); //setting the flashcard subject
    void setQuestion(std::string ques); //setting the flashcard question
    void setAnswer(std::string ans); //setting the flashcard answer

private:
    std::string subject;
    std::string question;
    std::string answer;
};
