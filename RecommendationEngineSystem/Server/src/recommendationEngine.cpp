#include "recommendationEngine.h"

RecommendationEngine::RecommendationEngine(DatabaseConnection* database) : database(database){} 

std::vector<std::pair<MenuItem, double>> RecommendationEngine::getRecommendedMenuItems(const int& noOfItems)
{
    std::vector<std::pair<MenuItem, double>> recommendationItems;
    std::vector<MenuItem> recommendedItems;
    try 
    {
        std::unique_ptr<sql::Statement> statement(database->getConnection()->createStatement());
        std::unique_ptr<sql::ResultSet> result(statement->executeQuery(
            "SELECT mi.item_id, mi.item_name, mi.price, mi.availability_status, "
            "IFNULL(AVG(f.rating), 0) AS average_rating "
            "FROM MenuItems mi "
            "LEFT JOIN Feedback f ON mi.item_id = f.item_id "
            "GROUP BY mi.item_id, mi.item_name, mi.price, mi.availability_status "
        ));

        while (result->next()) {
            MenuItem menuItem;
            menuItem.setItemId(result->getInt("item_id"));
            menuItem.setItemName(result->getString("item_name"));
            menuItem.setItemPrice(result->getDouble("price"));
            menuItem.setRating(result->getDouble("average_rating"));
            menuItem.setAvailabilityStatus(result->getBoolean("availability_status"));

            recommendedItems.push_back(menuItem);
        }

        for(auto& item : recommendedItems)
        {
            auto totalScore = getTotalScore(item.getItemId());
            recommendationItems.push_back(std::make_pair(item,totalScore));
        }

        std::sort(recommendationItems.begin(), recommendationItems.end(),
                [](const std::pair<MenuItem, double>& a, const std::pair<MenuItem, double>& b) {
                    return a.second > b.second;
                });
    } 
    catch (sql::SQLException &e) 
    {
        std::cerr << "SQL error: " << e.what() << std::endl;
    }

        return recommendationItems;
}

SentimentAnalyser::SentimentAnalyser(const std::string& filename)
{
    loadSentimentWords(filename);
}

void SentimentAnalyser::loadSentimentWords(const std::string& filename)
{
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string word;
        int score;
        if (ss >> word >> score) {
            sentimentWords[word] = score;
        }
    }
}

double RecommendationEngine::getTotalScore(const int& itemId)
{
    double averageRatingScore = getRatingScore(itemId) * 0.2;
    double averageSentimentScore = getSentimentScore(itemId) * 0.5;

    return averageRatingScore + averageSentimentScore;
}

double RecommendationEngine::getRatingScore(const int& itemId)
{
    double averageRatingScore = 0;
    try
    {
        std::unique_ptr<sql::PreparedStatement> pStatement(database->connection->prepareStatement("SELECT AVG(rating) AS average_rating FROM Feedback WHERE item_id = ?"));
        pStatement->setInt(1, itemId);

        std::unique_ptr<sql::ResultSet> result(pStatement->executeQuery());

        if (result->next()) {
            averageRatingScore = result->getDouble("average_rating");
        }
    }
    catch (sql::SQLException &exception)
    {
        std::cerr << "MySQL error: " << exception.what() << std::endl;
    }
    return averageRatingScore;
}

double RecommendationEngine::getSentimentScore(const int& itemId)
{
    int noOfComments = 0;
    double averageSentimentScore = 0;
    double totalSentimentScore = 0;
    try 
    {
        SentimentAnalyser sentimentAnalyser("./sentimentWords.txt");
        std::unique_ptr<sql::PreparedStatement> pStatement(database->connection->prepareStatement("SELECT comment FROM Feedback WHERE item_id = ?"));
        pStatement->setInt(1, itemId);
        std::unique_ptr<sql::ResultSet> result(pStatement->executeQuery());
        while(result->next())
        {
            auto comment = result->getString("comment");
            totalSentimentScore += sentimentAnalyser.calculateSentimentScore(comment);
            noOfComments++;
        }
    }
    catch (sql::SQLException &exception)
    {
        std::cerr << "MySQL error: " << exception.what() << std::endl;
    }
    if(noOfComments > 0)
    {
        averageSentimentScore = totalSentimentScore/noOfComments;
    }
    return averageSentimentScore;
}

double SentimentAnalyser::calculateSentimentScore(const std::string& comment)
{
    std::stringstream ss(comment);
    std::string word;
    int score = 0;
    bool negation = false;
    int negationEffectCounter = 0;

    while (ss >> word) {
        std::cout<<word<<std::endl;
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end());
        if (isNegationWord(word)) {
            negation = true;
            negationEffectCounter = 5;
        }

        if (negationEffectCounter > 0) 
        {
            negationEffectCounter--;
            negation = negationEffectCounter > 0;
        } 
        else 
        {
            negation = false;
        }
        auto it = sentimentWords.find(word);
        if (it != sentimentWords.end()) {
            score += negation ? -it->second : it->second;
            if (negationEffectCounter == 0) {
                negation = false;
            }
        }
    }
    return score;
}

bool SentimentAnalyser::isNegationWord(const std::string& word) {
    auto it = sentimentWords.find(word);
    if(it!=sentimentWords.end())
    {
        return(it != sentimentWords.end() && it->second == -10);
    }
    else
    {
        return false;
    }
}

std::vector<MenuItem> RecommendationEngine::getDiscardedMenuItems()
{
    int totalItems;
    std::vector<std::pair<MenuItem, double>> recommendedItems;
    std::vector<MenuItem> discardedMenuItems;
    try 
    {
        std::unique_ptr<sql::Statement> statement(database->getConnection()->createStatement());
        std::unique_ptr<sql::ResultSet> result(statement->executeQuery("SELECT COUNT(*) AS total_items FROM MenuItems"));

        if (result->next()) 
        {
            totalItems = result->getInt("total_items");
            std::cout << "Total number of items in the MenuItems table: " << totalItems << std::endl;
        }
        recommendedItems = getRecommendedMenuItems(totalItems);
    } 
    catch (sql::SQLException &exception) 
    {
        std::cerr << "MySQL error: " << exception.what() << std::endl;
    }

    for (auto &item:recommendedItems)
    {
        if(item.first.getRating() < 2 || (item.second < 0))
        {
            discardedMenuItems.push_back(item.first);
            std::unique_ptr<sql::PreparedStatement> pStatement(database->getConnection()->prepareStatement("INSERT INTO DiscardedMenuItems (item_id) VALUES (?)"));
            pStatement->setInt(1, item.first.getItemId());
            pStatement->execute();
        }
    }
    return discardedMenuItems;
}
