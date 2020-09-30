#Tsuf Cohen & Safi Azmi 4/12/2018
CC = gcc
OBJS = mtm_citizen.o mtm_city.o mtm_elections.o mtm_print.o uniqueOrderedList.o
EXEC = mtmElections
DEBUG_FLAG = -g -v
COMP_FLAG = -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG

$(EXEC): $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -L. -lmtm -o $@

mtm_citizen.o: mtm_citizen.c mtm_citizen.h uniqueOrderedList/uniqueOrderedList.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

mtm_city.o: mtm_city.c mtm_city.h mtm_citizen.h uniqueOrderedList/uniqueOrderedList.h set.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

mtm_elections.o: mtm_elections.c set.h mtm_elections.h uniqueOrderedList/uniqueOrderedList.h mtm_city.h mtm_citizen.h mtm_print.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

mtm_print.o: mtm_print.c mtm_print.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

uniqueOrderedList.o: uniqueOrderedList/uniqueOrderedList.c uniqueOrderedList/uniqueOrderedList.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) uniqueOrderedList/$*.c

clean:
	rm -f $(OBJS) $(EXEC)