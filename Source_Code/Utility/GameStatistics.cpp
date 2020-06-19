#include "stdafx.h"
#include "GameStatistics.h"

void GameStatistics::initVariables()
{
	this->name = "";
	this->defaultName = "-unknown-";
	this->currentPoints = 0;
	this->totalPoints = 0;
	this->enemyTypes.push_back(enemyType::REGULAR);
	this->enemyTypes.push_back(enemyType::COMMANDO);
	this->enemyTypes.push_back(enemyType::STATIONARY);
	this->enemyTypes.push_back(enemyType::BOSS);

	for (enemyType type : this->enemyTypes) {
		this->currentEnemyKills[type] = 0;
		this->totalEnemyKills[type] = 0;
	}
}

//Constructor
GameStatistics::GameStatistics(std::map<std::string, std::string> filepaths)
	:filepaths(filepaths)
{
	this->initVariables();
	std::ofstream scores_file(this->filepaths.at("SCORE_BOARD"), std::ios::out|std::ios::app);
	std::ofstream enemies_file(this->filepaths.at("ENEMY_KILLS"), std::ios::out|std::ios::app);
	scores_file.close();
	enemies_file.close();
}

GameStatistics::~GameStatistics()
{
}

const std::vector<std::pair<std::string, int>> GameStatistics::getBestScores(int amount) const
{
	std::vector<std::pair<std::string, int>> best_scores = this->scoreBoard;
	std::sort(best_scores.begin(), best_scores.end(),
		[](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {return a.second > b.second; }
	);

	if (best_scores.size() > amount)
		best_scores.erase(best_scores.begin() + amount, best_scores.end());
	return best_scores;
}

const int GameStatistics::getCurrentScore() const
{
	return this->currentPoints;
}

const std::map<enemyType, int> GameStatistics::getCurrentEnemyKills() const
{
	return this->currentEnemyKills;
}

const std::map<enemyType, int> GameStatistics::getTotalEnemyKills() const
{
	return this->totalEnemyKills;
}

const std::string GameStatistics::getName() const
{
	return this->name;
}

//Functions
void GameStatistics::logKill(int points, enemyType type)
{
	this->currentEnemyKills.at(type) += 1;
	this->currentPoints += points;
}

void GameStatistics::setName(const std::string name)
{
	this->name = name;
}

void GameStatistics::setNameDefault()
{
	this->name = this->defaultName;
}

void GameStatistics::reset()
{
	/*for (enemyType type : this->enemyTypes) {
		this->currentEnemyKills.at(type) = 0;
	}*/

	for (auto &[type, enemy_kills] : this->currentEnemyKills) {
		enemy_kills = 0;
	}
	this->currentPoints = 0;
}

void GameStatistics::update()
{
	if (this->name != this->defaultName) {
		for (enemyType type : this->enemyTypes) {
			this->totalEnemyKills.at(type) += this->currentEnemyKills.at(type);
		}
		this->scoreBoard.emplace_back(this->name, this->currentPoints);
		this->totalPoints += this->currentPoints;
	}

	
	
	
}

void GameStatistics::save()
{
	if (this->name != this->defaultName) {
		std::ofstream scores_file;
		scores_file.open(this->filepaths.at("SCORE_BOARD"), std::ios::app);

		if (scores_file.is_open()) {
			scores_file << this->name << " " << std::to_string(this->currentPoints) << "\n";
		}
		scores_file.close();

		std::fstream enemies_file;
		enemies_file.open(this->filepaths.at("ENEMY_KILLS"), std::ios::out);

		if (enemies_file.is_open()) {
			for (enemyType type : this->enemyTypes) {
				enemies_file << this->totalEnemyKills.at(type) << " ";
			}
			enemies_file << "\n";
		}
		enemies_file.close();
	}
	
}

void GameStatistics::load()
{
	std::ifstream scores_file;
	scores_file.open(this->filepaths.at("SCORE_BOARD"), std::ios::in);

	if (scores_file.is_open()) {
		std::string score_temp = "";
		std::string name_temp = "";
		while (scores_file >>name_temp >>score_temp) {
			this->scoreBoard.emplace_back(name_temp, std::stoi(score_temp));
			this->totalPoints += std::stoi(score_temp);
			//std::cout << score_temp << "\n";
		}
	}
	scores_file.close();

	std::fstream enemies_file;
	enemies_file.open(this->filepaths.at("ENEMY_KILLS"), std::ios::in);

	if (enemies_file.is_open()) {
		for (enemyType type : this->enemyTypes) {
			enemies_file >> this->totalEnemyKills.at(type);
		}
	}
	enemies_file.close();
}
