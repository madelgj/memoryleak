#include "Application.hpp"
using namespace std;

Application::Application()
{
    _currentMember = -1;
    _id = 0;
}

Application::~Application()
{
    _currentMember = -1;
    _members.clear();
}

bool Application::createMember(string username, string bio, string email, string password)
{
    for(unsigned long a=0; a<_members.size(); a++){
        if(_members[a]->getEmail()==email){
            return false;
        }
    }
    for(unsigned long a=0; a<_members.size(); a++){
        if(_members[a]->getUsername()==username){
            return false;
        }
    }
    _members.push_back(new Member(username,bio,email,password));
    return true;
}

bool Application::login(string email, string password)
{
    for(int a=0; a<_members.size(); a++){
        if (_members[a]->getEmail()==email && _currentMember==-1){
            if(_members[a]->getPassword()==password){
                _currentMember=a;
                return true;
            }
        }
    }
    return false;
}

bool Application::isLogged()
{
    if(_currentMember==-1){
        return false;
    }
    return true;
}

bool Application::logout()
{
    if(_currentMember==-1){
        return false;
    }
    _currentMember=-1;
    return true;
}

Member* Application::getCurrentMember()
{
    if(_currentMember==-1){
        return NULL;
    }
    return _members[_currentMember];
}

bool Application::eraseCurrentMember()
{
    if(_currentMember==-1){
        return false;
    }
    _members.erase(_members.begin()+_currentMember);
    _currentMember=-1;
    return true;
}

vector <MemberProfileInfo*> Application::showMembers()
{
    vector<MemberProfileInfo*> users;    //vector que guarda los datos que queremos obtener

    for (unsigned long i = 0; i < _members.size(); i++) {
        users.push_back((MemberProfileInfo*) _members[i]);
    }
    return users;
}

MemberProfileInfo* Application::showMember(string user)
{
    MemberProfileInfo* profile;
    profile=NULL;
    for (unsigned long a = 0; a < _members.size(); a++) {
        if(_members[a]->getUsername()==user){
            profile=(MemberProfileInfo*)_members[a];
        }
    }
    return profile;
}

bool Application::editEmail(string const email)
{
    for(unsigned long a=0; a<_members.size(); a++){
        if(_members[a]->getEmail()==email){
            return false;
        }
    }
    if(_currentMember==-1){
        return false;
    }
    _members[_currentMember]->setEmail(email);
    return true;
}

bool Application::editPassword(string const password)
{
    if(_currentMember==-1){
        return false;
    }
    _members[_currentMember]->setPassword(password);
    return true;
}

bool Application::editUsername(string const username)
{
    for(unsigned long a=0; a<_members.size(); a++){
        if(_members[a]->getUsername()==username){
            return false;
        }
    }
    if(_currentMember==-1){
        return false;
    }
    _members[_currentMember]->setUsername(username);
    return true;
}

bool Application::editBio(string const bio)
{
    if(_currentMember==-1){
        return false;
    }
    _members[_currentMember]->setBio(bio);
    return true;
}

bool Application::createQuestion(const string &title, const string &description, vector<string> tags)
{
    if (!isLogged()){
        return false;
    }
    _id++;

    unsigned long time = 10; //how to set time? I think we can set a random value
    _questions.push_back(new Question(_id,time,(MemberProfileInfo*)getCurrentMember(),title,description,tags));
    return true;
}

bool Application::answerQuestion(const int &idQuestion, const string &answerText)
{
    if (!isLogged()){
        return false;
    }
    int questionIndex = questionExists(idQuestion);
    // we check if the id provided corresponds to any question
    if (questionIndex == -1){
        return false;
    } else {
        if(!_questions[questionIndex]->getClosed()){
            unsigned long time = 10;
            _id++;
            _questions[questionIndex]->addInteraction(new Answer(_id,time,(MemberProfileInfo*)getCurrentMember(),answerText));
            return true;
        }
    }
    return false;
}

bool Application::comment(const int &idQA, const string &commentText)
{
    if (!isLogged()){
        return false;
    }
    unsigned long time = 10;
    int questionIndex = questionExists(idQA);
    // the id corresponds to a question
    if (questionIndex != -1){
        _id++;
        _questions[questionIndex]->addInteraction(new Comment(_id,time,(MemberProfileInfo*)getCurrentMember(),commentText));
        return true;
    }
    // check if corresponds to an answer

    Interaction* answerToComment = interactionExists(idQA);
    //the id corresponds to an answer
    //if(answerToComment != nullptr && answerToComment->is()=="Answer"){
    if( dynamic_cast<Answer*>(interactionExists(idQA))!= nullptr){
       // Answer* targetAnswer = (Answer*)answerToComment;
        Answer* targetAnswer = dynamic_cast<Answer*>(answerToComment);
        _id++;
        targetAnswer->addComment(new Comment(_id,time,(MemberProfileInfo*)getCurrentMember(),commentText));
        return true;
    } else {
        return false;
    }

}

bool Application::closeQuestion(const int &idQuestion)
{
    if (!isLogged()){
        return false;
    }
    int questionIndex = questionExists(idQuestion);
    if (questionIndex == -1){
        return false;
    } else{
        // we check if it's the author
        if(_questions[questionIndex]->getAuthor()->getUsername()==_members[_currentMember]->getUsername()){
            _questions[questionIndex]->setClosed(true);
            return true;
        }
    }
    return false;
}

bool Application::acceptAnswer(const int &idAnswer)
{
    if (!isLogged()){
        return false;
    }
    // we check if the id corresponds to any interaction
    Interaction* answerToClose = interactionExists(idAnswer);
    if(answerToClose!= nullptr && answerToClose->is()=="Answer"){
        int index = interactionIndex(idAnswer);
       // check if is the question's author
        if (_questions[index]->getAuthor()->getUsername()==_members[_currentMember]->getUsername()){
            Answer* targetAnswer = (Answer*) answerToClose;
            answerToClose->getAuthor()->increaseReputation();
            targetAnswer->setRightAnswer(true);
            return true;
        }
    }
    return false;
}

vector<Question *> Application::getQuestions()
{
    return _questions;
}

vector<Question *> Application::getQuestionsByTag(const string &tag)
{
    vector<Question *> questionsWithTag;
    for(unsigned long i=0; i<_questions.size(); i++){
        if(_questions[i]->questionHasTag(tag)){
            questionsWithTag.push_back(_questions[i]);
        }
    }
    return questionsWithTag;
}

bool Application::upvoteAnswer(const int &idAnswer)
{
    if (!isLogged()){
        return false;
    }
    Answer* answerToVote = (Answer*)interactionExists(idAnswer);
    if(answerToVote != nullptr){
        answerToVote->incrementVotes();
        answerToVote->getAuthor()->increaseReputation();
        return true;
    } else{
        return false;
    }

}

bool Application::downvoteAnswer(const int &idAnswer)
{
    if (!isLogged()){
        return false;
    }
    Answer* answerToVote = (Answer*)interactionExists(idAnswer);
    if(answerToVote != nullptr){
        answerToVote->decrementVotes();
        answerToVote->getAuthor()->decreaseReputation();
        return true;
    } else{
        return false;
    }

}

bool Application::upvoteQuestion(const int &idQuestion)
{
    if (!isLogged()){
        return false;
    }
    int questionIndex = questionExists(idQuestion);
    if (questionIndex == -1){
        return false;
    }

    _questions[questionIndex]->incrementVotes();
    return true;
}

bool Application::downvoteQuestion(const int &idQuestion)
{
    if (!isLogged()){
        return false;
    }
    int questionIndex = questionExists(idQuestion);
    if (questionIndex == -1){
        return false;
    } else {
        _questions[questionIndex]->decrementVotes();
        return true;
    }

}

bool Application::deleteQuestion(const int &idQuestion)
{
    if (!isLogged()){
        return false;
    }
    int questionIndex = questionExists(idQuestion);
    if (questionIndex != -1){
        // we check if it's the question's author
        if(_questions[questionIndex]->getAuthor()->getUsername()==_members[_currentMember]->getUsername()){
            _questions.erase(_questions.begin() + questionIndex);
            return true;
        }
    }
    return false;
}

bool Application::deleteInteraction(const int &idInteraction)
{
    if (!isLogged()){
        return false;
    }
    int interactionToDelete = interactionIndex(idInteraction);
    if(interactionToDelete != -1){
        // we check if it's the question's author
        if(_questions[interactionToDelete]->getAuthor()->getUsername()==_members[_currentMember]->getUsername()){
            _questions[interactionToDelete]->removeInteraction(idInteraction);
            return true;
        }

    }
    return false;
}

bool Application::modifyQuestion(const int &idQuestion, const string &newDescription)
{ //only author can modify
    if (!isLogged()){
        return false;
    }

    int questionIndex = questionExists(idQuestion);
    if (questionIndex != -1){
        // we see if the person trying to modify it is the author
        if(_questions[questionIndex]->getAuthor()->getUsername() == _members[_currentMember]->getUsername() ){
            _questions[questionIndex]->setDescription(newDescription);
            return true;
        } else {
            return false;
        }
    } else{
        return false;
    }
}

bool Application::modifyInteraction(const int &idInteraction, const string &newText)
{
    if (!isLogged()){
        return false;
    }
    Interaction* interactionToChange = interactionExists(idInteraction);
    if(interactionToChange->getAuthor()->getUsername()==_members[_currentMember]->getUsername()){
        if(interactionToChange != nullptr){
            interactionToChange -> setText(newText);
            return true;
        }
    }
    return false;

}

int Application::questionExists(const int &idQuestion)
{
    for (unsigned long i=0;i<_questions.size();i++){
        if(idQuestion == _questions[i]->getId()){
            return i;
        }
    }

    return -1;
}

Interaction *Application::interactionExists(const int &idInteraction){
    Interaction* answerToClose = nullptr;
    for (unsigned long i=0;i<_questions.size();i++){     // the id corresponds to an interaction
        answerToClose = _questions[i]->exists(idInteraction);
    }
    return answerToClose;

}
// we could avoid this method by passing the index above
int Application::interactionIndex(const int &idInteraction){
    Interaction* answerToClose;
    for (unsigned long i=0;i<_questions.size();i++){     // the id corresponds to an interaction
        answerToClose = _questions[i]->exists(idInteraction);
        if (answerToClose != nullptr){
            return i;
        }

    }
    return -1;
}
