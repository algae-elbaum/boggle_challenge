#pragma once

#include <string>

// 26 for each letter a-z, and then 1 more since sometimes non a-z chars show up
#define NUM_BINS 27

// Stores words from words list file
typedef struct
{
    // One array of words for each possible starting character
    // (counting all non a-z as equivalent)
    std::string *words[NUM_BINS];
    // The length of each words array
    int word_counts[NUM_BINS];
} word_arrays;

int get_bin_index(std::string word);
// Determine whether the candidate string is a prefix for any word
bool is_word_prefix(std::string candidate, word_arrays &words);
// Determine whether the candidate string is a word
bool is_word(std::string candidate, word_arrays &words);
// Sorts the arrays of words so that we can binary search them later
void sort_word_arrays(word_arrays &words);
// Cleanup
void delete_word_arrays(word_arrays &words);

