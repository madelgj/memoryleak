#ifndef APPLICATION_HPP
#define APPLICATION_HPP
#include <iostream>
#include <vector>
#include "Member.hpp"
#include "Question.hpp"
#include "Comment.hpp"
#include "Answer.hpp"

using namespace std;

class Application
{
public:
    Application();
    ~Application();
    bool createMember(string username, string bio, string email, string password);
    bool login(string email, string password);
    bool isLogged();
    Member* getCurrentMember();
    bool eraseCurrentMember();
    bool logout();
    vector <MemberProfileInfo*> showMembers();
    MemberProfileInfo* showMember(string user);
    bool editEmail(string const email);
    bool editPassword(string const password);
    bool editUsername(string const username);
    bool editBio(string const bio);

    bool createQuestion(const string &title, const string &description, vector<string> tags);
    bool answerQuestion(const int &idQuestion, const string &answerText);
    bool comment(const int &idQA, const string &CommentText);

    bool closeQuestion(const int &idQuestion);
    bool acceptAnswer(const int &idAnswer);

    vector<Question *> getQuestions(); //not needed to be logged
    vector<Question *> getQuestionsByTag(const string &tag);

    bool upvoteAnswer(const int &idQA);
    bool downvoteAnswer(const int &idQA);
    bool upvoteQuestion(const int &idQA);
    bool downvoteQuestion(const int &idQA);

    void deleteQuestion(const int &idQuestion);
    void deleteInteraction(const int &idInteraction);

    void modifyQuestion(const int &idQuestion,const string &newDescription);
    void modifyInteraction(const int &idInteraction, const string &newText);

    int questionExists(const int &idQuestion);

private:
    vector<Member*> _members;
    int _id;
    int _currentMember;    //default no member is logged
    vector<Question*> _questions;
};

#endif // APPLICATION_HPP
