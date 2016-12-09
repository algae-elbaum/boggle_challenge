Interview challenge, took about 3 hours


The problem was to take as input a 4x4 grid of characters (just 16 sequential
characters) and the name of a word list file and to output all words that appear
in the character grid. A word is considered to appear in the grid if it's there
according to the Boggle rules AND if it does not repeat using any single entry
in the grid.


The main mistake here is that I should have used a prefix tree, and instead I
basically made a depth=1 prefix tree, which of course isn't quite as good. In
my defense, I had never seen nor heard of a prefix tree before (though maybe
that's already a point against me?), but also I should have been able to come
up with it on my own after realizing checking is_prefix(string) repeatedly on
a growing string would be important.
