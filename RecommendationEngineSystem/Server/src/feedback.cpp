#include "feedback.h"

Feedback::Feedback(const int &itemId, const std::string &userId, const int &rating, const std::string &comment)
    : itemId_(itemId), userId_(userId), rating_(rating), comment_(comment) {}

int Feedback::getFeedbackId()
{
    return feedbackId_;
}

int Feedback::getItemId()
{
    return itemId_;
}

std::string Feedback::getUserId()
{
    return userId_;
}

int Feedback::getRating()
{
    return rating_;
}

std::string Feedback::getComment()
{
    return comment_;
}

void Feedback::setFeedbackId(const int &feedbackId)
{
    feedbackId_ = feedbackId;
}

void Feedback::setItemId(const int &itemId)
{
    itemId_ = itemId;
}

void Feedback::setUserId(const std::string &userId)
{
    userId_ = userId;
}

void Feedback::setRating(const int &rating)
{
    rating_ = rating;
}

void Feedback::setComment(const std::string &comment)
{
    comment_ = comment;
}