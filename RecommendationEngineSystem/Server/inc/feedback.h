#pragma once

#include <iostream>

class Feedback
{
private:
    int itemId_, rating_, feedbackId_;
    std::string userId_, comment_;
public:
    Feedback(const int &itemId, const std::string &userId, const int &rating, const std::string &comment);

    int getFeedbackId();
    int getItemId();
    std::string getUserId();
    int getRating();
    std::string getComment();

    void setFeedbackId(const int &feedbackId);
    void setItemId(const int &itemId);
    void setUserId(const std::string &userId);
    void setRating(const int &rating);
    void setComment(const std::string &comment);
};
