Serelex
=======

A tool for semantic relation extraction. The program finds pairs of semantically related words based on the text definitions coming from the Wikipedia articles (other texts may be also used). The extraction method implemented in this system is based on three similarity measures (cosine, gloss overlap, and Karaulov's measure) between texts and two nearest-neighbor algorithms (KNN and Mutual KNN). The tool is a cross-platform console application.

Licensed under [LGPLv3](http://www.gnu.org/licenses/lgpl.html).

Files and Catalogs
--------
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

*windows* - provides project to Microsoft VIsual Studio 2010

*test.sh* - 

*test.bat* - 

Build
-----

To build under Windows, use Visual Studio 2010 project in msvs folder. To build under *nix use makefile.

Using
-----

	serelex [-c <file>] [-d <file>] [-s <file>] [-o <file>] [-S o|c|k] [-M 1|2] [-K <K>] [-T <T1> <T2> <T3>]
	c - concepts file, default concepts.csv
	d - definitions file, default definitions.csv
	s - stopWords file, default stoplist.csv
	o - outputfile, default result.csv
	S - similarity method, default o
			o - OverWords
			c - Cos
			k - Karaulov method
	M - component analysis method, default 1
			1 - KNN
			2 - mKNN
	K, T1, T2, T3 - factors, default K = 2, T1 = 2, T1 = 1, T3 = 6
	