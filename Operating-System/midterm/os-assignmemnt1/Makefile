# makefile for scheduling program
#
# make rr - for round-robin scheduling
# make fcfs - for FCFS scheduling
# make sjf - for SJF scheduling
# make priority - for priority scheduling
# make priority_rr - for priority with round robin scheduling

CC=gcc
CFLAGS=-Wall
Pthread = -lpthread
Math = -lm
RealTime = -lrt

all: dph cons prod mycall

dph:
	$(CC) $(CFLAGS) -o dph dph.c $(Pthread) $(Math) $(RealTime)
cons:
	$(CC) $(CFLAGS) -o cons cons.c $(Pthread) $(Math) $(RealTime)
prod:
	$(CC) $(CFLAGS) -o prod prod.c $(Pthread) $(Math) $(RealTime)
mycall:
	$(CC) $(CFLAGS) -o mycall mycall.c $(Pthread) $(Math) $(RealTime)
clean:
	rm -rf *.o
	rm -rf dph
	rm -rf prod
	rm -rf cons
	rm -rf mycall


# fcfs: driver.o list.o CPU.o schedule_fcfs.o
# 	$(CC) $(CFLAGS) -o fcfs driver.o schedule_fcfs.o list.o CPU.o

# priority: driver.o list.o CPU.o schedule_priority.o
# 	$(CC) $(CFLAGS) -o priority driver.o schedule_priority.o list.o CPU.o

# schedule_fcfs.o: schedule_fcfs.c
# 	$(CC) $(CFLAGS) -c schedule_fcfs.c

# priority_rr: driver.o list.o CPU.o schedule_priority_rr.o
# 	$(CC) $(CFLAGS) -o priority_rr driver.o schedule_priority_rr.o list.o CPU.o

# driver.o: driver.c
# 	$(CC) $(CFLAGS) -c driver.c

# schedule_sjf.o: schedule_sjf.c
# 	$(CC) $(CFLAGS) -c schedule_sjf.c

# schedule_priority.o: schedule_priority.c
# 	$(CC) $(CFLAGS) -c schedule_priority.c

# schedule_rr.o: schedule_rr.c
# 	$(CC) $(CFLAGS) -c schedule_rr.c

# list.o: list.c list.h
# 	$(CC) $(CFLAGS) -c list.c

# CPU.o: CPU.c cpu.h
# 	$(CC) $(CFLAGS) -c CPU.c
