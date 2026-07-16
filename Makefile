# Makefile for SchedulerDES
# Default target builds the runnable scheduler by linking main with sources.

all: scheduler

scheduler: main.cpp scheduler.cpp flavors.cpp metrics.cpp scheduler_config.cpp
	g++ -std=c++17 -Wall -Wextra -O2 main.cpp scheduler.cpp flavors.cpp metrics.cpp scheduler_config.cpp -o scheduler

# Keep a standalone main target for quick smoke-tests
main: main.cpp
	g++ -std=c++17 -Wall -Wextra -O2 main.cpp -o main

clean:
	rm -f main scheduler *.o

.PHONY: all clean scheduler main
