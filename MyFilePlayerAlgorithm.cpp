#include "MyFilePlayerAlgorithm.h"


void MyFilePlayerAlgorithm::getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill) {
	playerNum = player;
	string file = "player" + to_string(player) + ".rps_board";
	unique_ptr<PiecePosition> illigal((PiecePosition*)new MyPiecePosition(new MyPoint(*new int(-1), *new int(-1)),*new char('Z'), *new char('Z'), *new int(player)));
	ifstream positionsFile(file);
	if (!positionsFile.is_open()) {
		cout << "Usage: the following input file is required in the working directory: " << file << endl;
		vectorToFill.push_back(std::move(illigal));
		return;
	}
	regex move("[ ]*[R|P|S|B|F][ ]+[[:digit:]]+[ ]+[[:digit:]]+[ ]*");
	regex jokerMove("[ ]*J[ ]+[[:digit:]]+[ ]+[[:digit:]]+[ ]+[R|P|S|B][ ]*");
	regex skip("[ ]*");
	string line;

	while (getline(positionsFile, line))
	{
		char type;
		int x;
		int y;
		bool isJoker = false;
		if (regex_match(line, move)) {
			//sscanf(line.c_str(), " %c %d %d ", &type, &x, &y);
			sscanf_s(line.c_str(), " %c %d %d ", &type, 1, &x, &y);
		}
		else if ((regex_match(line, jokerMove))) {
			//sscanf(line.c_str(), " J %d %d %c ", &x, &y, &type);
			sscanf_s(line.c_str(), " J %d %d %c ", &x, &y, &type, 1);
			isJoker = true;
		}
		else if ((regex_match(line, skip))) {
			continue; // this allows empty position lines
		}
		else {
			vectorToFill.push_back(std::move(illigal));
			return; // regex failed
		}
		char piece = isJoker ? 'J' : type;
		char jokerRep = isJoker ? type : '#';
		unique_ptr<PiecePosition> position = std::move(unique_ptr<PiecePosition>((PiecePosition*)new MyPiecePosition(new MyPoint(*new int(x), *new int(y)), *new char(piece), *new char(jokerRep), *new int(player))));
		vectorToFill.push_back(std::move(position));
	}

}

void MyFilePlayerAlgorithm::notifyOnInitialBoard(const Board& b, const std::vector<unique_ptr<FightInfo>>& fights) {

}

void MyFilePlayerAlgorithm::notifyOnOpponentMove(const Move& move) {

}

void MyFilePlayerAlgorithm::notifyFightResult(const FightInfo& fightInfo) {

}

unique_ptr<Move> MyFilePlayerAlgorithm::getMove() {
	string file = "player" + to_string(playerNum) + ".rps_moves";
	unique_ptr<Move> illigal((Move*)new MyMove(new MyPoint(*new int(-1), *new int(-1)), new MyPoint(*new int(-1), *new int(-1))));
	ifstream positionsFile = ifstream(file);
	if (!positionsFile.is_open()) {
		cout << "Usage: the following input file is required in the working directory: player1.rps_moves" << endl;
		return illigal;
	}
	if (positionsFile.peek() == EOF) {
		cout << "Usage: no move for player" + to_string(playerNum) << endl;
		return illigal;
	}
	regex move_joker("[ ]*[[:digit:]]+[ ]+[[:digit:]]+[ ]+[[:digit:]]+[ ]+[[:digit:]]+[ ]+J:[ ]+[[:digit:]]+[ ]+[[:digit:]]+[ ]+[R|P|S|B][ ]*");
	regex move("[ ]*[[:digit:]]+[ ]+[[:digit:]]+[ ]+[[:digit:]]+[ ]+[[:digit:]]+[ ]*");
	regex skip("[ ]*");
	string line;
	getline(positionsFile, line);

	int from_x;
	int from_y;
	int to_x;
	int to_y;
	int joker_x;
	int joker_y;
	char joker_rep;

	if (regex_match(line, move_joker)) {
		//sscanf(line.c_str(), " %d %d %d %d J: %d %d %c ", &from_x, &from_y, &to_x, &to_y, &joker_x, &joker_y, &joker_rep);
		sscanf_s(line.c_str(), " %d %d %d %d J: %d %d %c ", &from_x, &from_y, &to_x, &to_y, &joker_x, &joker_y, &joker_rep, 1);
		didJokerChange = true;
		lastJokerX = joker_x;
		lastJokerY = joker_y;
		lastJokerRep = joker_rep;
	}
	else if (regex_match(line, move)) {
		//sscanf(line.c_str(), " %d %d %d %d ", &from_x, &from_y, &to_x, &to_y);
		sscanf_s(line.c_str(), " %d %d %d %d ", &from_x, &from_y, &to_x, &to_y);
		didJokerChange = false;
	}
	else if (regex_match(line, skip)) {
		positionsFile.close();
		nextMoveLine++;
		return getMove();
	}
	else {
		// move parse failed
		return illigal;
	}
	positionsFile.close();
	nextMoveLine++;

	return unique_ptr<Move>((Move*)new MyMove(new MyPoint(*new int(from_x), *new int(from_y)), new MyPoint(*new int(to_x), *new int(to_y))));
}

unique_ptr<JokerChange> MyFilePlayerAlgorithm::getJokerChange() {
	if (didJokerChange) {
		return unique_ptr<JokerChange>((JokerChange*)new MyJokerChange(new MyPoint(*new int(lastJokerX), *new int(lastJokerY)), *new char(lastJokerRep)));
	}
	else {
		return nullptr;
	}
}

MyFilePlayerAlgorithm::~MyFilePlayerAlgorithm()
{

}
