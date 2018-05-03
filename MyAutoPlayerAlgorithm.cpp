#include "MyAutoPlayerAlgorithm.h"

void MyAutoPlayerAlgorithm::getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill)
{
}

void MyAutoPlayerAlgorithm::notifyOnInitialBoard(const Board & b, const std::vector<unique_ptr<FightInfo>>& fights)
{
}

void MyAutoPlayerAlgorithm::notifyOnOpponentMove(const Move & move)
{
}

void MyAutoPlayerAlgorithm::notifyFightResult(const FightInfo & fightInfo)
{
}

unique_ptr<Move> MyAutoPlayerAlgorithm::getMove()
{
	return unique_ptr<Move>(board->getBestMove(playerNum));
}

unique_ptr<JokerChange> MyAutoPlayerAlgorithm::getJokerChange()
{
	JokerChange* bestJokerChange = board->getBestJokerChange(playerNum);
	return bestJokerChange != nullptr ? unique_ptr<JokerChange>(bestJokerChange) : nullptr;
}

MyAutoPlayerAlgorithm::~MyAutoPlayerAlgorithm()
{
}
