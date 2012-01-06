#ifndef NMAX_H
#define NMAX_H

#include <vector>
#include <algorithm>

template < class Type >
class nmax
{
private:
	int max;
	int (* compare)(const Type &b, const Type &a);
	typename std::vector < Type > :: iterator minEl;

public:
	std::vector < Type > elements;

	nmax(int maxElements, int (* ptFuncCompare)(const Type &b, const Type &a) );
	~nmax();

	void add(Type el);

};


template < class Type >
nmax < Type > :: nmax( int maxElements, int (* ptFuncCompare)(const Type &b, const Type &a) )
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
void nmax < Type > :: add(Type el)
{
	int curSize = elements.size();

	if( curSize < max )
	{
		elements.push_back(el);

		minEl = min_element( elements.begin(), elements.end() );
	}
	else
	{
		//if( el > *minEl )
		if( compare( el, *minEl ) > 0 )
		{
			*minEl = el;

			minEl = min_element( elements.begin(), elements.end() );


		}


	}
}

#endif