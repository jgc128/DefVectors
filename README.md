Serelex
=======

Name
----
Serelex - a tool for semantic relation extraction.

Synopsis
--------
serelex [*options*]

Description
-----------
A tool for semantic relation extraction. The program finds pairs of semantically related words based on the text definitions coming from the Wikipedia articles (other texts may be also used). The extraction method implemented in this system is based on three similarity measures (cosine, gloss overlap, and Karaulov's measure) between texts and two nearest-neighbor algorithms (KNN and Mutual KNN). The tool is a cross-platform console application.

Licensed under [LGPLv3](http://www.gnu.org/licenses/lgpl.html).

Options
-------
**-c** *file*  
Concepts file, default concepts.csv. A text file containing a set of input words (one word per line). The program will try to find semantic relations between these input words. For instance, if words 'crocodile, alligator, house, and building' were given as input the program will try to return pairs 'crocodile,alligator' and 'house,building' among all possible combinations.

**-d** *file*  
Definitions file, default definitions.csv. A text file containing a set of definitions for words specified in the concepts.csv file. If this file contains definitions for more words that given in concepts.csv the program will skip the words which do not appear in the concepts.csv. The sample-data directory contains files with "definitions" derived from the introduction of Wikipedia articles. Other texts may be used as definitions (traditional dictionary glosses, WordNet, etc.) if provided in the same format.

**-s** *file*  
Stop-words file, default stoplist.csv. A text file containing stop words (one word per line). Words from this list will not be used by a similarity measure. All occurrences of these words in definitons.csv will be removed.

**-o** *file*  
Output file, default result.csv. A text file containing set of found semantic relations between words specified in the file concepts.csv. Each line of this file contains a pair of semantically related words, according to the specified method.

**-S** *o|c|k*  
Similarity method, default o. Semantic similarity measure used for relation extraction.  
 o - Gloss overlap measure equal to number of common words in the definitions of two words.  
 c - Cosine between bag-of-word vectors build from definitions of respective of two words  
 k - Karaulov's semantic similarity measure. 
 
**-M** *1|2*  
Component analysis method, default knn. An algorithm used to derive semantic relations from pairwise similarity scores between the words.  
 1 - knn. Standard nearest-neighbor algorithm (KNN). Here K most similar words are related to a target word.  
 2 - mknn. Mutual nearest-neighbor algorithm (MKNN). Here K mutually most similar words are related to a target word.

**-K** *NUM*  
Number of nearest-neighbors, default K = 2.

**-T** *T1 T2 T3*  
T1, T2, T3 - parameters of Karaulov's semantic similarity measure, default T1 = 2, T1 = 1, T3 = 6.

Files and Catalogs
------------------
*bin* - contains binary excutable file:  
 * serelex_win32.exe - excutable file for 32-bit Windows
 * serelex_win64.exe - excutable file for 64-bit Windows
 * serelex_i686 - executable file for i686 Debian-based systems
 * serelex_amd64 - executable file for amd64 Debian-based systems

*docs* - provides documentation in html format

*sample-data* - contains sample source data:  
 * concepts.csv - a text file containing a set of input words (one word per line)
 * definitions.csv - a text file containing a set of definitions for words specified in the concepts.csv file
 * stoplist.csv - a text file containing stop words (one word per line)

*src* - the source code

*windows* - provides project to Microsoft Visual Studio 2010

*test.sh* and *test.bat*  - run analysis for algorithm KNN, MutalKNN, with overlap and cosinus method and K = 1, 2, 5, 10

Build
-----
To build under Windows, use MS Visual Studio 2010 project in *windows* folder. To select configuration - x32 or x64 - use select list on tools panel.  
To build under *nix use command `make`. To build on amd64 use command `make ARCH=-m64`

	