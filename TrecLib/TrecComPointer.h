#pragma once
#include <Unknwn.h>
#include <WinBase.h>
#include <vector>
#include "TrecPointerBase.h"
#include "Logger.h"

/*
* Class TrecComPointer
* Holds the Resources for addapting the TrecComPointer to specific Regular Classes and references
*/
template <class T>class  /*_TREC_LIB_DLL*/ TrecComPointer : public TrecPointerBase
{
	friend class TrecComPointer<T>;
private:
	T* object;

public:

	/*
	* Method: (TrecComPointer) (Constructor)
	* Purpose: Default Constructor to use
	* Parameters: void
	* Returns: void
	*/
	TrecComPointer()
	{
		object = nullptr;
		init(object);
	}

	/*
	* Method: (TrecComPointer) (Constructor)
	* Purpose: Initializes TrecComPointer based off of existing reference
	* Parameters: T* obj - raw reference to use
	* Returns: void
	*/
	TrecComPointer(T* obj)
	{
		object = obj;
		init(object,typeid(T).name());
		TString log;
		log.Format(L"Pointer Assigned - %p - %S - Count = %i (COM)", object, TString(typeid(T).name()), getCount());
		Log(lt_pointer, log);
	}

	/*
	* Method: (TrecComPointer) (Constructor)
	* Purpose: Copy Constructor
	* Parameters: TrecComPointer<T>& obj - Trec Pointer to copy
	* Returns: void
	*/
	TrecComPointer(TrecComPointer<T>& obj)// : TrecComPointerBase(obj , typeid(T).name())
	{
		object = obj.get();
		//init(object, typeid(T).name());
		SetTrecPointer(obj);
		if (object)
		{
			IncrementCount(/*typeid(T).name()*/);
			TString log;
			log.Format(L"Pointer Assigned - %p - %S - Count = %i (COM)", object, TString(typeid(T).name()), getCount());
			Log(lt_pointer, log);
		}
	}

	/*
	* Method: (TrecComPointer) (Destructor)
	* Purpose: Cleans up resources used by TrecComPointer
	* Parameters: void
	* Returns: void
	*/
	~TrecComPointer()
	{
		if (!object)
			return;
		T* del = reinterpret_cast<T*>(DecrementCount(/*typeid(T).name()*/));
		if (del)
		{
			ASSERT(object == del);
			object->Release();
			TString log;
			log.Format(L"Pointer Unassigned - %p - %S - Count = %i (COM-RELEASED)", object, TString(typeid(T).name()), getCount());
			Log(lt_pointer, log);
		}
		
	}


	void hold()
	{
		setHold();
	}

	/*
	* Method: TrecComPointer - get
	* Purpose: Gets underlying pointer of TrecComPointer
	* Parameters: void
	* Returns: T* Reference to object
	*/
	T* get()
	{
		void* ref = Get();
		if (ref != object)
			object = nullptr;
		return object;
	}

	/*
	* Method: TrecComPointer - operator->
	* Purpose: Overloads -> operator so TrecComPointer can behave like a regular pointer
	* Parameters: void
	* Returns: T* Reference to object
	*/
	T * operator->()
	{
		return get();
	}

	/*
	* Method: TrecComPointer - operator=
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
			T* del = reinterpret_cast<T*>(DecrementCount(/* typeid(T).name()*/));
			if (del)
			{
				ASSERT(object == del);
				object->Release();
				TString log;
				log.Format(L"Pointer Unassigned - %p - %S - Count = %i (COM-RELEASED)", object, TString(typeid(T).name()), getCount());
				Log(lt_pointer, log);
			}
			else
			{
				TString log;
				log.Format(L"Pointer Unassigned - %p - %S - Count = %i (COM)", object, TString(typeid(T).name()), getCount());
				Log(lt_pointer, log);
			}
		}
		if (other)
		{
			IncrementCount(other,  typeid(T).name());
			TString log;
			log.Format(L"Pointer Assigned - %p - %S - Count = %i (COM)", other, TString(typeid(T).name()), getCount());
			Log(lt_pointer, log);
		}
		resetHold();
		object = other;
	}



	/*
	* Method: TrecComPointer - operator=
	* Purpose: Overloads assignment operator for a second TrecComPointer
	* Parameters: TrecComPointer<T>& point - reference to the other TrecComPointer
	* Returns: void
	*/
	void operator=(TrecComPointer<T>& point)
	{
		if (point.get() == object)
			return;
		if (object)
		{
			T* del = reinterpret_cast<T*>(DecrementCount( /*typeid(T).name()*/));
			if (del)
			{
				ASSERT(object == del);
				object->Release();
				TString log;
				log.Format(L"Pointer Unassigned - %p - %S - Count = %i (COM-RELEASED)", object, TString(typeid(T).name()), getCount());
				Log(lt_pointer, log);
			}
			else
			{
				TString log;
				log.Format(L"Pointer Unassigned - %p - %S - Count = %i (COM)", object, TString(typeid(T).name()), getCount());
				Log(lt_pointer, log);
			}
		}

		T* pointer = point.get();
		if (pointer)
		{
			SetTrecPointer(point);
			IncrementCount(/*typeid(T).name()*/);
			TString log;
			log.Format(L"Pointer Assigned - %p - %S - Count = %i (COM)", pointer, TString(typeid(T).name()), getCount());
			Log(lt_pointer, log);
		}
		else nullify();

		object = pointer;
		resetHold();
	}

	/*
	* Method: TrecComPointer - Delete
	* Purpose: Allows Object to be deleted regardless of the count
	* Parameters:
	* Returns:
	*/
	void Delete()
	{
		if (object && object == NukeCount(/*typeid(T).name()*/))
		{
			object->Release();
			TString log;
			log.Format(L"Pointer Unassigned - %p - %S - Count = %i (COM-RELEASED) (FORCE)", object, TString(typeid(T).name()), getCount());
			Log(lt_pointer, log);
		}
		else
			object = nullptr;
		nullify();
	}

};
