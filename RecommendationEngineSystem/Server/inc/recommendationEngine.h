#pragma once

#include<iostream>
#include<vector>
#include<sstream>
#include <unordered_map>
#include <algorithm>
#include <fstream>

#include "menuItem.h"
#include "dbConnection.h"

class RecommendationEngine
{
private:
    DatabaseConnection* database;
public:
    RecommendationEngine (DatabaseConnection* database);
    std::vector<std::pair<MenuItem, double>> getRecommendedMenuItems(const int& noOfItems);
    double getSentimentScore(const int& itemId);
    double getRatingScore(const int& itemId);
    double getTotalScore(const int& itemId);
    std::vector<MenuItem> getDiscardedMenuItems();
};

class SentimentAnalyser
{
private:
    std::unordered_map<std::string, int> sentimentWords;
public:
    SentimentAnalyser(const std::string& fileName);
    void loadSentimentWords(const std::string& filename);
    double calculateSentimentScore(const std::string& reviewComment);
    bool isNegationWord(const std::string& word);
};