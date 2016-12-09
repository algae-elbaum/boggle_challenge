TARGETS = process_words
all: $(TARGETS)

process_words: process_words.o file_parsing.o word_arrays.o
	$(CXX) -std=c++98 -g -Wall -o $@ process_words.o file_parsing.o word_arrays.o

process_words.o: process_words.cc
	$(CXX) -std=c++98 -c -Wall -g -o $@ process_words.cc

file_parsing.o: file_parsing.cc
	$(CXX) -std=c++98 -c -Wall -g -o $@ file_parsing.cc

word_arrays.o: word_arrays.cc
	$(CXX) -std=c++98 -c -Wall -g -o $@ word_arrays.cc


clean:
	rm -f *.o $(TARGETS)

again: clean $(TARGETS)
