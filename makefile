COMP = g++-5.3.0
OBJS = Soldier.o Board.o Game.o
EXEC = ex1
CPP_COMP_FLAG = -std=c++14 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) -o $@
Soldier.o: Soldier.cpp Soldier.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Board.o: Board.cpp Board.h Soldier.h Constants.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Game.o: Game.cpp Game.h Board.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
	
clean:
	rm -f $(OBJS) $(EXEC)