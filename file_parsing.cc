#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "file_parsing.h"

using namespace std;

// Read through the word list file counting the number of words to be put in
// each bin in words. Then also allocating the space for those arrays
void init_word_arrays(ifstream &words_file, word_arrays &words)
{
    // First count up the numbers of words of each type
    string word;
    int bin_idx;
    while (getline(words_file, word))
    {
        bin_idx = get_bin_index(word);
        words.word_counts[bin_idx]++;
    }
    int max_bin = 0;
    int min_bin = words.word_counts[0];
    for(int i = 0; i < NUM_BINS; i++)
    {
        max_bin = max(max_bin, words.word_counts[i]);
        min_bin = min(min_bin, words.word_counts[i]);
    }

    // Now allocate space for each array of words according to the above counts
    for (int i = 0; i < NUM_BINS; i++)
    {
        int num_words = words.word_counts[i];
        words.words[i] = new string[num_words];
    }
}

// Read though the word list file populating the words parameter with the
// words in the file
void fill_word_arrays(ifstream &words_file, word_arrays &words)
{
    string word;
    int bin_idx, new_word_idx;
    int word_counts[NUM_BINS] = {0};
    while (getline(words_file, word))
    {
        bin_idx = get_bin_index(word);
        new_word_idx = word_counts[bin_idx]++;

        words.words[bin_idx][new_word_idx] = word;
    }
}

// Initialize and populate words parameter using the given file
void parse_file(char *words_filename, word_arrays &words)
{
    ifstream words_file(words_filename);
    if (!words_file.is_open())
    {
        cerr << "Couldn't open file" << endl;
        // No point going on from here
        exit(EXIT_FAILURE);
    }
    
    // We run through the file twice, which is a little ugly, but without STL
    // it gets nasty to build up a list of words that is easy to binary search
    // (ie not a linked list) without first knowing the length of the list, and
    // to find the lengths of any of our arrays we need to count up every word
    // in the file that will end up in the array
    init_word_arrays(words_file, words);
    words_file.clear();
    words_file.seekg(0);
    fill_word_arrays(words_file, words);
    words_file.close();
}

