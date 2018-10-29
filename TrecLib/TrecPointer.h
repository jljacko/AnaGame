#pragma once
#include <Unknwn.h>
#include <WinBase.h>
#include <vector>
#include "TrecPointerBase.h"


/*
* Class TrecPointer
* Holds the Resources for addapting the TrecPointer to specific Regular Classes and references
*/
template <class T>class  /*_TREC_LIB_DLL*/ TrecPointer : public TrecPointerBase
{
	friend class TrecPointer<T>;
private:
	T* object;

public:

	/*
	* Method: (TrecPointer) (Constructor)
	* Purpose: Default Constructor to use
	* Parameters: void
	* Returns: void
	*/
	TrecPointer()
	{
		object = nullptr;
		init(nullptr);
	}

	/*
	* Method: (TrecPointer) (Constructor)
	* Purpose: Initializes TrecPointer based off of existing reference
	* Parameters: T* obj - raw reference to use
	* Returns: void
	*/
	TrecPointer(T* obj) 
	{
		object = obj;
		init(obj, typeid(T).name());
	}

	/*
	* Method: (TrecPointer) (Constructor)
	* Purpose: Copy Constructor
	* Parameters: TrecPointer<T>& obj - Trec Pointer to copy
	* Returns: void
	*/
	TrecPointer(TrecPointer<T>& obj) //: TrecPointerBase(obj, typeid(T).name())
	{
		object = obj.get();
		//init(obj.get());
		SetTrecPointer(obj);
		if (object)
			IncrementCount();
	}

	/*
	* Method: (TrecPointer) (Destructor)
	* Purpose: Cleans up resources used by TrecPointer
	* Parameters: void
	* Returns: void
	*/
	~TrecPointer()
	{
		if (!object)
			return;

		if (getRefStatus())
		{
			enableDeRef();
			return;
		}

			T* del = reinterpret_cast<T*>(DecrementCount(/*typeid(T).name()*/));
			if (del)
			{
				ASSERT(del == object);
				delete del;
			}
	}

	/*
	* Method: TrecPointer - get
	* Purpose: Gets underlying pointer of TrecPointer
	* Parameters: void
	* Returns: T* Reference to object
	*/
	T* get()
	{
		void* ref = Get();
		if (ref != object)
			object = nullptr;
		return object;
		//return reinterpret_cast<T*>(Get());
	}

	/*
	* Method: TrecPointer - operator->
	* Purpose: Overloads -> operator so TrecPointer can behave like a regular pointer
	* Parameters: void
	* Returns: T* Reference to object
	*/
	T * operator->()
	{
		return get();
	}

	/*
	* Method: TrecPointer - hold
	* Purpose: Nothing
	* Parameters: void
	* Returns: void
	*/
	void hold()
	{
		setHold();
	}

	/*
	* Method: TrecPointer - operator=
	* Purpose: Overloads assignment operator to enable assignment of a pointer to comparable type
	* Parameters: u* other - raw pointer to similar class
	* Returns: void
	*/
	template<class u> void operator=(u* other)
	{
		T* castOther = dynamic_cast<T*>(other);
		
		if (castOther == object)
			return;

		if (object)
		{
			T* del = reinterpret_cast<T*>(DecrementCount(/*typeid(T).name()*/));
			if (del)
				delete del;
		}
		if (castOther)
			IncrementCount(castOther, typeid(T).name());
		else
			nullify();
		object = castOther;
	}


	/*
	* Method: TrecPointer - operator=
	* Purpose: Ovrloads assignment operator to take in acceptable raw pointer
	* Parameters: T* other - the raw pointer to add
	* Returns: void
	*/
	void operator=(T* other)
	{
		if (other == object)
			return;
		// If pointer is already active, dereference it
		if (object)
		{
			T* del = reinterpret_cast<T*>(DecrementCount(/*typeid(T).name()*/));
			if (del)
			{
				ASSERT(del == object);
				delete del;
			}
		}
		if (other)
		{
			IncrementCount(other, typeid(T).name());
		}
		else
			nullify();

		object = other;
		//resetHold();
	}

	/*
	* Method: TrecPointer - operator=
	* Purpose: Allows another TrecPointer with a similar enough class to be assigned
	* Parameters: TrecPointer<u>& point - TrecPointer with reference to similar object type
	* Returns: void
	*/
	template<class u> void operator=(TrecPointer<u>& point)
	{
		u* altRaw = point.get();

		T* altRawCast = dynamic_cast<T*>(altRaw);

		if (altRawCast == object)
			return;

		if (object)
		{
			T* del = reinterpret_cast<T*>(DecrementCount(/*typeid(T).name()*/));
			if (del)
				delete del;
		}
		if (altRawCast)
		{
			SetTrecPointer(point);
			IncrementCount(typeid(T).name());
		}
		else
			nullify();
		object = altRawCast;
	}

	/*
	* Method: TrecPointer - operator=
	* Purpose: Overloads assignment operator for a second TrecPointer
	* Parameters: TrecPointer<T>& point - reference to the other TrecPointer
	* Returns: void
	*/
	void operator=(TrecPointer<T>& point)
	{
		if (point.get() == object)
			return;
		if (object)
		{
			T* del = reinterpret_cast<T*>(DecrementCount());
			if (del)
			{
				//ASSERT(del == object);
				delete del;
			}
		}
		object = point.get();
		SetTrecPointer(point);
		if(object)	
		IncrementCount(typeid(T).name());

		point.hold();
		//resetHold();
	}

	/*
	* Method: TrecPointer - Delete
	* Purpose: Allows Object to be deleted regardless of the count
	* Parameters:
	* Returns:
	*/
	void Delete()
	{
		if(object && object == NukeCount(/*typeid(T).name()*/))
			delete object;
		nullify();
	}

	/*
	* Method: TrecPointer - lock
	* Purpose: locks the pointer for deletion
	* Parameters: void
	* Returns: void
	*/
	void lock()
	{
		stopDeRef();
	}
};

template <class T, class... types> TrecPointer<T> makeTrecPointer(types&&... args)
{
	T* type = new T(args...);
	TrecPointer<T> tc;
	tc = type;

	return tc;
}

template <class T> T* null()
{
	return (T*)nullptr;
}

template <class T, class U> TrecPointer<U> GetAlternativeType(T orig)
{
	T* obj = orig.get();
	if (!obj)
		return TrecPointer<U>();
	TrecPointer<U> ret;
	ret = dynamic_cast<U*>(obj);
	return ret;
}

#define ComNull (IUnknown*)nullptr
