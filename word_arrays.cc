#include "word_arrays.h"

using namespace std;

// Must return:
// -1 if (first string < second string)
// 0 if (first string == second string)
// 1 if (first string > second string)
// According to the function's definition of >, <, ==
typedef int (*comp_fun)(string, string);

int normal_strcmp(string str1, string str2)
{
    if (str1 < str2)
        return -1;
    else if (str1 == str2)
        return 0;
    else
        return 1;
}

int substring_strcmp(string str1, string str2)
{
    return normal_strcmp(str1.substr(0, str2.length()), str2);
}

// The bins made this way aren't great, but they will at least shave off a
// few iterations in searching for whether a string is a word. Not necessary,
// but nice and doesn't really complicate anything
int get_bin_index(string word)
{
    char c = word[0];
    if (isalpha(c))
        return tolower(c) - 'a';
    // else
    return NUM_BINS - 1;
}

// Binary search according to provided comparison operation.
// Returns bool for whether anything in list compares equal.
bool bin_search(string candidate, word_arrays words, comp_fun compare)
{
    int bin_idx = get_bin_index(candidate);
    string *bin = words.words[bin_idx];

    int low_idx = 0;
    int high_idx = words.word_counts[bin_idx] - 1;
    int curr_idx = (low_idx + high_idx) / 2;
    while (compare(bin[curr_idx], candidate) && high_idx - low_idx > 1)
    {
        if (compare(bin[curr_idx], candidate) < 0)
            low_idx = curr_idx;
        else
            high_idx = curr_idx;
        
        curr_idx = (low_idx + high_idx) / 2;
    }
    return    !compare(bin[curr_idx], candidate)
           || !compare(bin[low_idx], candidate)
           || !compare(bin[high_idx], candidate);
}

// Determine whether the candidate string is a prefix for any word
bool is_word_prefix(string candidate, word_arrays &words)
{
    return bin_search(candidate, words, substring_strcmp);
}

// Determine whether the candidate string is a word
bool is_word(string candidate, word_arrays &words)
{
    return bin_search(candidate, words, normal_strcmp);
}

// Using insertion sort based on the assumption that the words file will be
// mostly presorted. Would be easy to replace this with any other in-place
// sort if need be.
void sort_words(string *&words, int len)
{
    for (int i = 1; i < len; i++)
    {
        // As long as the j element is smaller than the j-1 element,
        // swap j and j-1
        for (int j = i; j > 0 && words[j] < words[j-1]; j--) 
        {
            string temp = words[j];
            words[j] = words[j-1];
            words[j-1] = temp;
        }
    } 
}

// Sorts the arrays of words so that we can binary search them later
void sort_word_arrays(word_arrays &words)
{
    for (int i = 0; i < NUM_BINS; i++)
        sort_words(words.words[i], words.word_counts[i]); 
}

// Cleanup
void delete_word_arrays(word_arrays &words)
{
    for (int i = 0; i < NUM_BINS; i++)
    {
        delete[] words.words[i];
    }
}


