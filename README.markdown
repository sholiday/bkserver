## bkserver ##

By [Stephen Holiday](http://stephenholiday.com) 2011



### Tools ###

Aside from the server structure itself, I wrote two tools to play with BKTrees.

Too make the tools just run:
    make tools
    
This assumes that you have the boost serialization libraries [installed and compiled]((http://www.boost.org/doc/libs/release/more/getting_started/index.html).


#### bkLoad ####
This is a utility for creating a BKTree from a text file and saving to disk.
The input files should have one entry per line.

    ./bkLoad [from] [to]
    ./bkLoad /usr/share/dict/words wordTree.dat

#### bkSearch ####
This is a utility for loading a BKTree from a serialized file.

    ./bkSearch [serialized tree] [word] [max edxit distance]
    ./bkSearch bkTree.dat word 3
    
### Comments/Pull Requests ###
If you think something could be improved, I'm happy to accept pull requests.