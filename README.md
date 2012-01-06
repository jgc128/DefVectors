Serelex
=======

Extracting semantic relations from Wikipedia with the help of the nearest-neighbor algorithm

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
