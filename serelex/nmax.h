#ifndef NMAX_H
#define NMAX_H

#include <vector>
#include <algorithm>

template < class Type >
class nmax
{
private:
	int max;
	typename std::vector < Type > :: iterator minEl;
	int (* compare)(const Type &b, const Type &a);

	void find_min();

public:
	std::vector < Type > elements;

	nmax(int maxElements , int (* ptFuncCompare)(const Type &b, const Type &a) );
	~nmax();

	void add(Type el);

};


template < class Type >
nmax < Type > :: nmax( int maxElements , int (* ptFuncCompare)(const Type &b, const Type &a) )
{
	max = maxElements;
	compare = ptFuncCompare;

	elements.reserve( sizeof( Type ) * max );

}

template < class Type >
nmax < Type > :: ~nmax()
{

}

template < class Type > 
void nmax < Type > :: find_min()
{
	typename  std::vector < Type > :: iterator it;

	minEl = elements.begin();

	int res;

	for( it = elements.begin(); it != elements.end(); ++ it )
	{
		res = compare( *it, *minEl );
		if( res < 0 )
			minEl = it;
	}

}

template < class Type > 
void nmax < Type > :: add(Type el)
{
	int curSize = elements.size();

	if( curSize < max )
	{
		elements.push_back(el);

		//minEl = min_element( elements.begin(), elements.end() );

		find_min();
	}
	else
	{
		//if( el > *minEl )
		int res = compare( el, *minEl );
		if( res > 0 )
		{
			*minEl = el;

			//minEl = min_element( elements.begin(), elements.end()  );
			find_min();

		}


	}
}

#endif