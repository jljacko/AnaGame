#include "stdafx.h"
#include "TArray.h"


template<class t>TArray<t>::TArray()
{
}


template<class t>TArray<t>::~TArray()
{

}

template<class t>
bool TArray<t>::Add(t* loc)
{
	if (loc == NULL)
		return false;
	if (count == 100) // first container is full
	{
		extension = new TArray<t>();
		extension->Add(loc);
	}
	else if (count > 100)
	{
		if (!extension)
			return false;
		extension->Add(loc);
	}
	else
	{
		data[count / 10][count % 10] = loc;
	}
	count++;
	return true;
}

template<class t>
t * TArray<t>::ElementAt(int at)
{
	if (at > count)
		return NULL;
	if (int >= 100)
	{
		if (!extension)
			return NULL;
		return extension->ElementAt(at - 100);
	}
	return data[at / 10][at % 10];
}

template<class t>
int TArray<t>::Count()
{
	return count;
}

template<class t>
t * TArray<t>::RemoveAt(int loc)
{
	if(loc > count)
	return nullptr;
	count--;
	if (loc > 99)
	{
		if (!extension)
			return NULL;
		return extension->RemoveAt(loc - 100);
	}
	else
	{
		t* point = data[loc/10][loc%10];
		data[loc / 10][loc % 10] = NULL;
		clearNull();
		return point;
	}
}

template<class t>
bool TArray<t>::DeleteAt(int loc)
{
	if(loc > count)
	return false;
	count--;
	if (loc > 99)
	{
		if (!extension)
			return false;
		return extension->DeleteAt(loc - 100);
	}
	else
	{
		delete data[loc / 10][loc % 10];
		data[loc / 10][loc % 10] = NULL;
		clearNull();
		return true;
	}
}

template<class t>
void TArray<t>::Clear(bool deleteData = false)
{
	if (extension)
	{
		extension->Clear();
		delete extension;
		extension = NULL;
	}
	else
	{
		for (int c = 0; c < 10; c++)
		{
			for (int d = 0; d < 10; d++)
			{
				if (deleteData)
					delete data[c][d];
				data[c][d] = NULL;

			}
		}
	}

}

template<class t>
void TArray<t>::clearNull()
{
}
