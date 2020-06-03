#pragma once

class GameStatistics
{
private:
	std::vector<enemyType> enemyTypes;
	std::map<std::string, std::string> filepaths;

	//Current
	std::string name;
	int currentPoints;
	std::map<enemyType, int> currentEnemyKills;

	//Total
	std::map<enemyType, int> totalEnemyKills;
	std::vector<std::pair<std::string,int>> scoreBoard;
	int totalPoints;

	//initlaizer Funtions
	void initVariables();
public:
	GameStatistics(std::map<std::string, std::string> filepaths);
	~GameStatistics();

	//Accesors
	const std::vector<std::pair<std::string, int>> getBestScores(int amount) const;
	const int getCurrentScore() const;
	const std::map<enemyType, int> getCurrentEnemyKills() const;
	const std::map<enemyType, int> getTotalEnemyKills() const;
	const std::string getName() const;

	//Functions
	void logKill(int points, enemyType type);
	void setName(const std::string name);
	void reset();
	void update();
	void save();
	void load();
	
};

