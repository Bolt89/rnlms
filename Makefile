CC=gcc
CFLAGS=-Wall -std=gnu89 -O0 -ggdb -Wall -Wextra -Wformat=2 -pedantic -Winit-self -Wstrict-overflow=5 -Wcast-qual -Wcast-align -Wconversion -Waggregate-return 
# CFLAGS=-Wall -std=c99 -O0 -ggdb
OBJS=rnlms.o tester.o  CircularBuffer.o global.o utils.o
HEAD=global.h rnlms.h CircularBuffer.h utils.h tester.h
BIN_FILES=$(wildcard g165/*.dat)
DAT_FILES=$(BIN_FILES=$:.dat=_ccs.dat)

all:main bin_to_ccs_dat_converter

# ${OBJS}: *.c *.h Makefile
# 		$(CC) %.c %.h $(CFLAGS)

# %.o: %.c 
# 		$(CC) $%.c $%.h $(CFLAGS)


$(OBJS): %.o: %.c %.h Makefile
		$(CC) -c $(CFLAGS) $< -o $@

main: $(HEAD) $(OBJS)
		$(CC) $(OBJS) $(CFLAGS) -o $@ -lm

# .PHONY: test
# test:
# 		bash -c "cd Debug; ../main | awk 'BEGIN {print \"plot \\\"-\\\" with lines \"} {print \$$1} END{print  \"e\npause mouse keypress \\\"Hit return to continue\\\" \"} ' | gnuplot ; "

.PHONY: test
test:
		bash -c "cd Debug; ../main"
		gnuplot -e "plot 'Debug/error.dat' binary format='%short' using 1 with lines; pause mouse keypress \"Hit return to continue\";"

bin_to_ccs_dat_converter: bin_to_dat.c
		$(CC) $(CFLAGS) $< -o $@


.PHONY: clean
clean:
		rm -f *.o main bin_to_ccs_dat_converter
