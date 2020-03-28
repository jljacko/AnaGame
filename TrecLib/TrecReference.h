#pragma once
#include <WinBase.h>
#include <Windows.h>

/**
 * File: TrecReference.h
 * This file provides the tools for managing memory in AnaGame and could theoretically be used in any Windows C++ project.
 *	The classes are either Anagame Smart Pointers or helper classes for the smart pointers
 *
 *		TrecPointer<T> - the standard smart pointer offered By Anagame, where T can be any type.
 *			Each TrecPointer points to a counter which keeps track of the number of TrecPointers referencing it
 *
 *		TrecPointerSoft<T> - a weak pointer that references counters used byt the standard TrecPointer, but does not
 *			increment the counter. Use for Object to have a TrecReference to itself so that they can produce a TrecPointer to itself
 *			This Pointer can be compared to the weak_ptr in the Standard C++ library
 *
 *		TrecSubPointer<T,U> - A smart pointer that casts the reference type T to it's presumed type U. The raw pointer is of type T,
 *			but the TrecSubPointer attempts to cast it to type U. Ideally, U should be a subclass of T.
 *		
 *		TrecSubPointerSoft<T,U> - a weak pointer that operates with the TrecSubPointer<T,U>. It has access to the counter used by
 *			the other TrecPointers but does not increment or decriment the counter
 *
 *		TrecComPointer<T> - A specialized pointer to be used when the type T inherits from IUnknown in a Mocrosoft Environment. This is
 *			because these types are created by a Factory method/function and deleting them requires calling the Release() method rather than
 *			manually deleting them.
 *
 *
 *		The Other class you will ue extensively is the TrecPointerKey class. This is a utility class necessary to operate on the TrecPointers
 *			This is because the TrecPointers restrict what you can do with them. 
 */



/**
 * class TrecBoxPointer
 * Purpose: holds the raw pointer and a counter to the object referenced by the various TrecPointers
 */
template<class T> class TrecBoxPointer
{
	/**
	 * Need to allow the utility class TrecPointerKey access for it to perform it's job properly
	 */
	friend class TrecPointerKey;
private:
	/**
	 * The raw pointer to the actual object being used
	 */
	T* rawPointer;

	/**
	 * tracks the number of TrecPointers referencing the object
	 */
	UINT counter;
public:

	/**
	 * Method: TrecBoxPointer::TrecBoxPointer
	 * Purpose: Constructor
	 * Parameters: T* t - raw pointer to the T type that this counter will be holding.
	 * Returns: New TrecBoxPointer Object
	 */
	TrecBoxPointer(T* t)
	{
		counter = (t)? 1 : 0;
		rawPointer = t;
	}

	/**
	 * Method: TrecBoxPointer::~TrecBoxPointer
	 * Purpose: Removes the rar pointer if still initialized
	 * Parameters:
	 * Returns:
	 */
	~TrecBoxPointer()
	{
		if (rawPointer)
			delete rawPointer;
	}

	/**
	 * Method: TrecBoxPointer::Increment
	 * Purpose: Increases the counter by one, intended to be called by all regular ans Sub TrecPointers when they first reference this object
	 * Parameters: void
	 * Returns: void
	 */
	void Increment()
	{
		counter++;
	}

	/**
	 * Method: TrecBoxPointer::Decrement
	 * Purpose: Reduces the counter. If the counter reaches zero, go ahead an commence clean up
	 * Parameters: void
	 * Returns: void
	 */
	void Decrement()
	{
		counter--;
		if (!counter) // if counter is zero, then there is no reference to the object and it is time to delete it.
			delete this;
	}

	/**
	 * Method: TrecBoxPointer::Get
	 * Purpose: Returns the raw object held by this object
	 * Parameters: void
	 * Returns: T* - the object held
	 */
	T* Get()
	{
		return rawPointer;
	}

	/**
	 * Method: TrecBoxPointer::Delete
	 * Purpose: Performs manual deletion of the object, in case the code deems it necessary to Delete the object early
	 * Parameters: void
	 * Returns: void
	 */
	void Delete()
	{
		if(rawPointer)
			delete rawPointer;
		// now that the object has been deleted, set it to null to avoid a dangling pointer
		rawPointer = nullptr;
	}
};

template<class T> class TrecComBoxPointer
{
private:
	T* rawPointer;
	UINT counter;
public:

	/**
	 * Method: TrecComBoxPointer::TrecComBoxPointer
	 * Purpose: Constructor
	 * Parameters: T* t - the raw pointer to the COM object
	 * Returns: New TrecComBoxPointer Object
	 */
	TrecComBoxPointer(T* t)
	{
		counter = (t) ? 1 : 0;
		rawPointer = t;
	}

	/**
	 * Method: TrecComBoxPointer::TrecComBoxPointer
	 * Purpose: Cleans up the Object
	 * Parameters: void
	 * Returns: void
	 */
	~TrecComBoxPointer()
	{
		if (rawPointer)
			// Because this is a COM object, we call Release() on it
			rawPointer->Release();
	}

	/**
	 * Method: TrecComBoxPointer::Increment
	 * Purpose: Increments the counter by 1
	 * Parameters: void
	 * Returns: void
	 */
	void Increment()
	{
		counter++;
	}

	/**
	 * Method: TrecComBoxPointer::Decrement
	 * Purpose: Reduces the country by 1, commencing deletion if the counter reaches 0
	 * Parameters: void
	 * Returns: void
	 */
	void Decrement()
	{
		counter--;
		if (!counter)
			delete this;
	}

	/**
	 * Method: TrecComBoxPointer::Get
	 * Purpose: Returns the raw pointer to the higher TrecComPoint
	 * Parameters: void
	 * Returns: T* - the raw object pointer
	 */
	T* Get()
	{
		return rawPointer;
	}

	/**
	 * Method: TrecComBoxPointer::Delete
	 * Purpose: Performs manual deletion of the object
	 * Parameters: void
	 * Returns: void
	 */
	void Delete()
	{
		if(rawPointer)
			rawPointer->Release();
		rawPointer = nullptr;
	}
};


template<class T> class TrecPointerSoft
{
	friend class TrecPointerKey;
private:
	TrecBoxPointer<T>* pointer;


	/**
	 * Method: TrecPointerSoft::TrecPointerSoft
	 * Purpose: Constructor
	 * Parameters:TrecBoxPointer<T>* pointer - pointer to the Object holding the raw pointer
	 * Returns: New TrecPointerSoft Object
	 */
	TrecPointerSoft(TrecBoxPointer<T>* pointer)
	{
		if(!pointer)
			throw L"Error! TrecPointers must be initialized with a pointer, not NULL!";
		this->pointer = pointer;
	}

public:
	/**
	 * Method: TrecPointerSoft::TrecPointerSoft
	 * Purpose: Default constructor
	 * Parameters: void
	 * Returns: New TrecPointerSoft Object
	 */
	TrecPointerSoft()
	{
		pointer = nullptr;
	}

	/**
	 * Method: TrecPointerSoft::Get
	 * Purpose: Retrieves the underlying object, or null if the object is no longer available
	 * Parameters: void
	 * Returns: T* - the object the pointer was referencing
	 */
	T* Get()
	{
		if (pointer)
			return pointer->Get();
		return nullptr;
	}

	/**
	 * Method: TrecPointerSoft::operator=
	 * Purpose: Allows for use of the = operator when assigning this type of object
	 * Parameters: const TrecPointerSoft<T>& other - the Pointer holding the object to assign to this Pointer
	 * Returns: void
	 */
	void operator=(const TrecPointerSoft<T>& other)
	{
		// First, make sure that the other TrecPointer isn't pointing to the same reference.
		// If it is, we don't want to decrement the reference lest it become 0 (and self-destructs)
		if (other.pointer == pointer)
			return;

		pointer = other.pointer;
	}
};

template<class T, class U> class TrecSubPointerSoft
{
	friend class TrecPointerKey;
private:
	TrecBoxPointer<T>* pointer;

	/**
	 * Method: TrecSubPointerSoft::TrecSubPointerSoft
	 * Purpose: Constructor
	 * Parameters: TrecBoxPointer<T>* pointer - the pointer to refer to 
	 * Returns: New TrecSubPointerSoft Object
	 */
	TrecSubPointerSoft(TrecBoxPointer<T>* pointer)
	{
		if (!pointer)
			throw L"Error! TrecPointers must be initialized with a pointer, not NULL!";
		this->pointer = pointer;
	}

public:
	/**
	 * Method: TrecSubPointerSoft::TrecSubPointerSoft
	 * Purpose: Default Constructor
	 * Parameters: void
	 * Returns: New TrecSubPointerSoft Object
	 */
	TrecSubPointerSoft()
	{
		pointer = nullptr;
	}

	/**
	 * Method: TrecSubPointerSoft::Get
	 * Purpose: Retrieves the Sub-type that this Pointer points to
	 * Parameters: void
	 * Returns: U* - the subtype of the object pointed to (or null if the type s not a U)
	 */
	U* Get()
	{
		if (pointer)
			return dynamic_cast<U*>(pointer->Get());
		return nullptr;
	}

	/**
	 * Method: TrecSubPointerSoft::operator=
	 * Purpose: Allows use of the = operator when setting a TrecSubPointerSoft
	 * Parameters:const TrecSubPointerSoft<T, U>& other - reference to the pointer to get the counter from 
	 * Returns: void
	 */
	void operator=(const TrecSubPointerSoft<T, U>& other)
	{
		// First, make sure that the other TrecPointer isn't pointing to the same reference.
		// If it is, we don't want to decrement the reference lest it become 0 (and self-destructs)
		if (other.pointer == pointer)
			return;

		pointer = other.pointer;
	}
};

template<class t, class u> class TrecSubPointer
{
	friend class TrecPointerKey;
	// friend class TrecPointer<t>;
protected:
	TrecBoxPointer<t>* pointer;


	/**
	 * Method: TrecSubPointer::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	TrecSubPointer(u* raw)
	{
		if(!raw)
			throw L"Error! TrecPointers must be initialized with a pointer, not NULL!";
		t* baseRaw = dynamic_cast<t*>(raw);
		if (!baseRaw)
			throw L"Error! U type must be a subclass of T type!";
		pointer = new TrecBoxPointer<t>(baseRaw);
	}

public:

	/**
	 * Method: TrecSubPointer::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	TrecSubPointer()
	{
		pointer = nullptr;
	}

	/**
	 * Method: TrecSubPointer::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	TrecSubPointer(const TrecSubPointer<t,u>& copy)
	{
		pointer = copy.pointer;
		if (pointer)
			pointer->Increment();
	}

	/**
	 * Method: TrecSubPointer::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	~TrecSubPointer()
	{
		Nullify();
	}

	/**
	 * Method: TrecSubPointer::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	void Nullify()
	{
		if (pointer)
			pointer->Decrement();
		pointer = nullptr;
	}

	/**
	 * Method: TrecSubPointer::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	void operator=(const TrecSubPointer<t,u>& other)
	{
		// First, make sure that the other TrecPointer isn't pointing to the same reference.
		// If it is, we don't want to decrement the reference lest it become 0 (and self-destructs)
		if (other.pointer == pointer)
			return;

		if (pointer)
			pointer->Decrement();

		pointer = other.pointer;

		if (pointer)
			pointer->Increment();
	}

	/**
	 * Method: TrecSubPointer::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	u* operator->()
	{
		if (!pointer) return nullptr;
		return dynamic_cast<u*>(pointer->Get());
	}

	/**
	 * Method: TrecSubPointer::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	u* Get()
	{
		if (!pointer) return nullptr;
		return dynamic_cast<u*>(pointer->Get());
	}

	/**
	 * Method: TrecSubPointer::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	t* GetBase()
	{
		if (!pointer) return nullptr;
		return pointer->Get();
	}

	/**
	 * Method: TrecSubPointer::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	void Delete()
	{
		if (!pointer) return;
		pointer->Delete();
		pointer->Decrement();
		pointer = nullptr;
	}
};

template<class t> class TrecPointer
{
	friend class TrecPointerKey;
protected:
	TrecBoxPointer<t>* pointer;


	/**
	 * Method: TrecPointer::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	TrecPointer(t* raw)
	{
		if (!raw)
			throw L"Error! TrecPointers must be initialized with a pointer, not NULL!";
		pointer = new TrecBoxPointer<t>(raw);
	}
public:
	/**
	 * Method: TrecPointer::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	TrecPointer(const TrecPointer<t>& copy)
	{
		pointer = copy.pointer;
		if(pointer)
		pointer->Increment();
	}

	/**
	 * Method: TrecPointer::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	TrecPointer()
	{
		pointer = nullptr;
	}

	/**
	 * Method: TrecPointer::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	~TrecPointer()
	{
		Nullify();
	}

	/**
	 * Method: TrecPointer::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	void Nullify()
	{
		if (pointer)
			pointer->Decrement();
		pointer = nullptr;
	}

	/**
	 * Method: TrecPointer::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	void operator=(const TrecPointer<t>& other)
	{
		// First, make sure that the other TrecPointer isn't pointing to the same reference.
		// If it is, we don't want to decrement the reference lest it become 0 (and self-destructs)
		if (other.pointer == pointer)
			return;

		if(pointer)
			pointer->Decrement();

		pointer = other.pointer;

		if (pointer)
			pointer->Increment();
	}

	/**
	 * Method: TrecPointer::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	template<class u> u* As()
	{
		if (!pointer)
			return nullptr;
		t* raw = pointer->Get();
		if (!raw)
			return nullptr;

		u* altRaw = dynamic_cast<u*>(raw);

		if (!altRaw)
			throw (L"Error! Attempted to retrieve a pointer of an incompatible type!");
		return altRaw;
	}

	/**
	 * Method: TrecPointer::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	t* Get()
	{
		if (!pointer) return nullptr;
		return pointer->Get();
	}

	/**
	 * Method: TrecPointer::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	t* operator->()
	{
		if (!pointer) return nullptr;
		return pointer->Get();
	}

	/**
	 * Method: TrecPointer::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	void Delete()
	{
		if (!pointer) return;
		pointer->Delete();
		pointer->Decrement();
		pointer = nullptr;
	}
};



template<class t> class TrecComPointer
{
	friend class TrecPointerKey;
protected:
	TrecComBoxPointer<t>* pointer;
public:
	class TrecComHolder
	{
		friend class TrecPointerKey;
	protected:
		t* holder;

	public:

		/**
		 * Method: TrecComHolder::
		 * Purpose:
		 * Parameters:
		 * Returns:
		 */
		TrecComHolder()
		{
			holder = nullptr;
		}

		/**
		 * Method: TrecComHolder::
		 * Purpose:
		 * Parameters:
		 * Returns:
		 */
		t** GetPointerAddress()
		{
			return &holder;
		}

		/**
		 * Method: TrecComHolder::
		 * Purpose:
		 * Parameters:
		 * Returns:
		 */
		void OfferValue(TrecComPointer<t>& point)
		{
			holder = point.Get();
		}

		/**
		 * Method: TrecComHolder::
		 * Purpose:
		 * Parameters:
		 * Returns:
		 */
		void Extract(TrecComPointer<t>& point)
		{
			if (!holder)
				throw L"Error! Cannot Extract Smart Pointer with raw value!";
			if (holder != point.Get())
			{
				point.Nullify();
				point = TrecComPointer<t>(holder);
			}
			holder = nullptr;
		}

		/**
		 * Method: TrecComHolder::
		 * Purpose:
		 * Parameters:
		 * Returns:
		 */
		TrecComPointer<t> Extract()
		{
			if (!holder)
				throw L"Error! Cannot Extract Smart Pointer with raw value!";
			TrecComPointer<t> ret(holder);
			holder = nullptr;
			return ret;
		}

	};

protected:

	/**
	 * Method: TrecComPointer::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	TrecComPointer(t* raw)
	{
		if (!raw)
			throw (L"Error! TrecPointers must be initialized with a pointer, not NULL!");
		pointer = new TrecComBoxPointer<t>(raw);
	}

public:

	/**
	 * Method: TrecComPointer::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	TrecComPointer()
	{
		pointer = nullptr;
	}

	/**
	 * Method: TrecComPointer::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	~TrecComPointer()
	{
		Nullify();
	}

	/**
	 * Method: TrecComPointer::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	TrecComPointer(const TrecComPointer<t>& other)
	{
		pointer = other.pointer;

		if (pointer)
			pointer->Increment();
	}

	/**
	 * Method: TrecComPointer::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	void operator=(const TrecComPointer<t>& other)
	{
		// First, make sure that the other TrecPointer isn't pointing to the same reference.
		// If it is, we don't want to decrement the reference lest it become 0 (and self-destructs)
		if (other.pointer == pointer)
			return;

		if(pointer)
		pointer->Decrement();
		pointer = other.pointer;
		if(pointer)
		pointer->Increment();
	}


	/**
	 * Method: TrecComPointer::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	void Nullify()
	{
		if (pointer)
			pointer->Decrement();
		pointer = nullptr;
	}

	/**
	 * Method: TrecComPointer::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	template<class u> u* As()
	{
		if (!pointer) return nullptr;

		t* raw = pointer->Get();
		if (!raw)
			return nullptr;

		u* altRaw = dynamic_cast<u*>(raw);

		if (!altRaw)
			throw (L"Error! Attempted to retrieve a pointer of an incompatible type!");
		return altRaw;
	}

	/**
	 * Method: TrecComPointer::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	t* Get()
	{
		if (!pointer)return nullptr;
		return pointer->Get();
	}

	/**
	 * Method: TrecComPointer::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	t* operator->()
	{
		if (!pointer)return nullptr;
		return pointer->Get();
	}

	/**
	 * Method: TrecComPointer::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	void Delete()
	{
		if (!pointer) return;
		pointer->Delete();
		pointer->Decrement();
		pointer = nullptr;
	}
};

class TrecPointerKey
{
public:

	/**
	 * Method: static TrecPointerKey::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	template <class T> static TrecPointer<T> GetTrecPointerFromSoft(TrecPointerSoft<T>& soft)
	{
		TrecPointer<T> ret;
		ret.pointer = soft.pointer;
		if(ret.pointer)
			ret.pointer->Increment();
		return ret;
	}

	/**
	 * Method: static TrecPointerKey::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	template <class T> static TrecPointerSoft<T> GetSoftPointerFromTrec(TrecPointer<T>& trec)
	{
		return TrecPointerSoft<T>(trec.pointer);
	}

	/**
	 * Method: static TrecPointerKey::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	template <class T, class...types> static TrecPointer<T> GetNewSelfTrecPointer(types&& ... args)
	{
		T* raw = new T(args...);
		TrecPointer<T> ret(raw);
		raw->SetSelf(ret);
		return ret;
	}

	/**
	 * Method: static TrecPointerKey::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	template <class t, class...types> static TrecPointer<t> GetNewTrecPointer(types&& ... args)
	{
		t* raw = new t(args...);
		TrecPointer<t> ret(raw);
		return ret;
	}

	/**
	 * Method: static TrecPointerKey::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	template <class T> static TrecPointer<T> GetNewTrecPointerPlain()
	{
		T* raw = new T();
		TrecPointer<T> ret(raw);
		return ret;
	}

	/**
	 * Method: static TrecPointerKey::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	template <class t, class u, class...types> static TrecPointer<t> GetNewSelfTrecPointerAlt(types&& ... args)
	{
		TrecPointer<t> ret = GetNewTrecPointerAlt<t, u>(args...);
		if (ret.Get())
			ret->SetSelf(ret);
		return ret;
	}

	/**
	 * Method: static TrecPointerKey::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	template <class t, class u, class...types> static TrecPointer<t> GetNewTrecPointerAlt(types&& ... args)
	{
		u* raw = new u(args...);
		t* raw_t = nullptr;


		// Need to make sure that the alternative type 'u' is compatible with this type 't'

		raw_t = dynamic_cast<t*>(raw);


		if (!raw_t)
		{
			delete raw;
			throw L"Error! Attempted to assign incompatible type!";
		}
		TrecPointer<t> ret(raw_t);
		return ret;
	}

	/**
	 * Method: static TrecPointerKey::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	template <class t, class u> static TrecPointer<t> GetTrecPointerFromSub(TrecSubPointer<t, u>& sub)
	{
		TrecPointer<t> ret;
		ret.pointer = sub.pointer;
		if (ret.pointer && ret.pointer)
			ret.pointer->Increment();
		return ret;
	}

	/**
	 * Method: static TrecPointerKey::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	template <class t, class u> static TrecSubPointer<t, u> GetTrecSubPointerFromTrec(const TrecPointer<t>& tPointer)
	{
		TrecSubPointer<t, u> ret;
		ret.pointer = tPointer.pointer;
		if (ret.pointer && ret.pointer)
			ret.pointer->Increment();
		return ret;
	}

	/**
	 * Method: static TrecPointerKey::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	template <class t, class u, class...types> static TrecSubPointer<t, u> GetNewTrecSubPointer(types&& ... args)
	{
		u* raw = new u(args...);
		TrecSubPointer<t, u> ret(raw);
		return ret;
	}

	/**
	 * Method: static TrecPointerKey::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	template <class t, class u, class...types> static TrecSubPointer<t, u> GetNewSelfTrecSubPointer(types&& ... args)
	{
		u* raw = new u(args...);
		TrecSubPointer<t, u> ret(raw);
		ret->SetSelf(GetTrecPointerFromSub<t>(ret));
		return ret;
	}

	/**
	 * Method: static TrecPointerKey::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	template <class t, class u> static TrecComPointer<t> GetComPointer(typename TrecComPointer<u>::TrecComHolder& holder)
	{
		if (!holder.holder)
			throw L"Error! Expected valid value";

		t* raw = dynamic_cast<t*>(holder.holder);

		if (!raw)
			throw L"Error! Dealing with incompatible types!";

		holder.holder = nullptr;

		return TrecComPointer<t>(raw);
	}

	/**
	 * Method: static TrecPointerKey::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	template <class T, class U> static TrecSubPointer<T, U> GetSubPointerFromSoft(TrecSubPointerSoft<T, U> s)
	{
		TrecSubPointer<T, U> ret;

		ret.pointer = s.pointer;
		if (ret.pointer)
			ret.pointer->Increment();
		return ret;
	}

	/**
	 * Method: static TrecPointerKey::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	template <class T, class U> static TrecSubPointerSoft<T, U> GetSoftSubPointerFromSub(TrecSubPointer<T, U> trec)
	{
		return TrecSubPointerSoft<T,U>(trec.pointer);
	}


};