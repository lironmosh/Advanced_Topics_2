#include <string>
#include "Board.h"
using namespace std;
class Game
{
	Board* board;

	//private functions
	int parseInitStateForPlayerFromFile(string file, int player);

public:
	Game();
	void startGame();
	void endGame(int winner, string reason);
	bool isGameOver(bool player1haveMoves, bool player2haveMoves, int opponent);
	~Game();
};