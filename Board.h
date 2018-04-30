#include <string>
#include "Soldier.h"
#include "Constants.h" 
using namespace std;
class Board
{
	Soldier*** board;
	string* rep = nullptr;

public:
	Board();
	bool putSoldier(Soldier* solider, int x, int y);
	bool makeMove(int x_start, int y_start, int x_end, int y_end);
	bool makeMove(int x_start, int y_start, int x_end, int y_end, int joker_x, int joker_y, char joker_rep);
	string* getSringRep();
	bool isPlayerHaveFlags(int player);
	bool isPlayerHaveMoveableSoliders(int player);
	~Board();
};