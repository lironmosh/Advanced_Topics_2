#ifndef __GAME_MANAGER_H_
#define __GAME_MANAGER_H_

#include <string>
#include "MyBoard.h"
#include "PlayerAlgorithm.h"
#include "MyFilePlayerAlgorithm.h"
#include "MyAutoPlayerAlgorithm.h"

using namespace std;
class GameManager
{
	MyBoard* board;

	//private functions
	int parseInitStateForPlayer(int player, vector<unique_ptr<PiecePosition>>& piecePositions, vector<unique_ptr<FightInfo>>& fightInfos);

public:
	GameManager();
	void startGame(string player1config, string player2config);
	void endGame(int winner, string reason);
	bool isGameOver(int opponent);
	~GameManager();
};

#endif