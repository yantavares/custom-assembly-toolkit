CC = g++
CFLAGS = -Wall -std=c++20 -I includes
SRCDIR = src
BINDIR = bin
TESTDIR = tests
SOURCES = $(wildcard $(SRCDIR)/*.cpp) main.cpp
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(BINDIR)/%.o)
TARGET = licc
TESTFILE = $(TESTDIR)/runTests.cpp

.PHONY: all clean create_memories

all: create_memories $(TARGET)

create_memories:
	python3 utils/createMemories.py

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

$(BINDIR)/%.o: $(SRCDIR)/%.cpp | $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR):
	mkdir -p $(BINDIR)

run: all
	./$(TARGET) $(FILE) $(MODE)

clean:
	rm -rf $(BINDIR)
	rm -f $(TARGET)

test: clean all
	$(CC) $(CFLAGS) $(TESTFILE) -o $(BINDIR)/test
	./$(BINDIR)/test

