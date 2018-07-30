#include "stdafx.h"
#include "TMap.h"


template<class t2>TMap<t2>::TMap()
{
}


template<class t2>TMap<t2>::~TMap()
{
	this->clear(true);
}

template<class t2>
void TMap<t2>::addEntry(WCHAR str, t2 *obj)
{
	
}

template<class t2>
t2 * TMap<t2>::retrieveEntry(WCHAR str)
{
	
}

template<class t2>
t2 * TMap<t2>::removeEntry(WCHAR str)
{
	
}

template<class t2>
void TMap<t2>::clear()
{
	map.Clear(true);
}

template<class t2>
int TMap<t2>::count()
{
	return map.Count();
}
