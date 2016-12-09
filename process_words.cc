#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>

//#define NDEBUG 
#include <cassert>

#include "word_arrays.h"
#include "file_parsing.h"

#define GRID_WIDTH 4

using namespace std;

/*
 *
 * Using C++ specifically because string and file processing is much nicer
 * than in C
 *
 */

typedef struct word_llist
{
    string word;
    word_llist *next;
} word_llist;

// Insert a word into a word_llist iff the word is not already present
void insert_word(string word, word_llist *&list)
{
    word_llist *curr = list;
    while (curr != NULL)
    {
        if (curr->word == word)
            return;
        curr = curr->next;
    }

    word_llist *new_head = new word_llist;
    new_head->word = word;
    new_head->next = list;
    list = new_head;
}

void delete_llist(word_llist *list)
{
    word_llist *curr = list;
    word_llist *old;
    while (curr != NULL)
    {
        old = curr;
        curr = curr->next;
        delete old;
    }
}

// Fill neighbors parameter as a -1 terminated list of valid neighbors of the
// given index.
void get_adjacent_indices(int *neighbors, int idx)
{
    int neighbor_count = 0;
    // if not left
    if (idx % GRID_WIDTH)
    {
        neighbors[neighbor_count] = idx - 1;
        neighbor_count++;
    }
    // if not right
    if ((idx + 1) % GRID_WIDTH)
    {
        neighbors[neighbor_count] = idx + 1;
        neighbor_count++;
    }
    // if not up
    if (idx >= GRID_WIDTH)
    {
        neighbors[neighbor_count] = idx - GRID_WIDTH;
        neighbor_count++;
        
        // if not left
        if (idx % GRID_WIDTH)
        {
            neighbors[neighbor_count] = (idx - GRID_WIDTH) - 1;
            neighbor_count++;
        }
        // if not right
        if ((idx + 1) % GRID_WIDTH)
        {
            neighbors[neighbor_count] = (idx - GRID_WIDTH) + 1;
            neighbor_count++;
        }
    }
    // if not down
    if (idx < GRID_WIDTH * (GRID_WIDTH - 1))
    {
        neighbors[neighbor_count] = idx + GRID_WIDTH;
        neighbor_count++;
        
        // if not left
        if (idx % GRID_WIDTH)
        {
            neighbors[neighbor_count] = (idx + GRID_WIDTH) - 1;
            neighbor_count++;
        }
        // if not right
        if ((idx + 1) % GRID_WIDTH)
        {
            neighbors[neighbor_count] = (idx + GRID_WIDTH) + 1;
            neighbor_count++;
        }
    }

    neighbors[neighbor_count] = -1;
}

// Add all words in grid with prefix prefix to results.
// Previously visited characters are skipped in finding valid suffixes for prefix
void find_prefixed_words(string prefix, int curr_idx, bool *visited, char *grid, word_arrays &words, word_llist *&results)
{
    // Eight possible neighbors for any index, terminated by -1 just in case of <8
    int neighbors[8 + 1];
    get_adjacent_indices(neighbors, curr_idx);
    
    string saved_prefix = prefix;

    for (int i = 0; neighbors[i] != -1; i++)
    {
        if (visited[neighbors[i]])
            continue;

        visited[neighbors[i]] = true;
        prefix.push_back(grid[neighbors[i]]);

        if (is_word(prefix, words))
            insert_word(prefix, results);
        if (is_word_prefix(prefix, words))
            find_prefixed_words(prefix, neighbors[i], visited, grid, words, results);
        
        // Reset for next round through
        prefix = saved_prefix;
        visited[neighbors[i]] = false;
    }

}

// Add all words in grid to results
void find_words(char *grid, word_arrays &words, word_llist *&results)
{
    // Current plan: iterate through all possible paths, abandoning a path as
    //               soon as it's not a prefix.
    //
    // Would like to be able to do some sort of memoization, but my best idea
    // for that is along the lines of precomputing compound words, and the
    // payoff would be minimal to none.

    // Visited flag per index
    bool visited[GRID_WIDTH * GRID_WIDTH] = {};

    for (int i = 0; i < GRID_WIDTH * GRID_WIDTH; i++)
    {
        visited[i] = true;
        string prefix = "";
        prefix.push_back(grid[i]);

        if (is_word(prefix, words))
            insert_word(prefix, results);
        find_prefixed_words(prefix, i, visited, grid, words, results);
        
        visited[i] = false;
    }
}


int main(int argc, char **argv)
{
    if (argc != 3)
    {
        cerr << "Usage: " << argv[0] << " <wordsfile> <16 char string>" << endl;
        exit(EXIT_FAILURE);
    }
   
    // Get word_arrays object for the words file
    char *words_filename = argv[1];
    char *grid = argv[2];
    word_arrays words = {};
    parse_file(words_filename, words);
    sort_word_arrays(words);

    assert(!is_word("somethi", words));
    assert(is_word("something", words));
    assert(!is_word_prefix("asdf", words));
    assert(is_word_prefix("somethi", words));

    // Find all words in the grid
    word_llist *results = NULL;
    find_words(grid, words, results);
   
    // Print words 
    word_llist *curr = results;
    while (curr != NULL)
    {
        cout << curr->word << endl;
        curr = curr->next;
    }

    // Cleanup
    delete_llist(results);
    delete_word_arrays(words); 
    return 0;
}
