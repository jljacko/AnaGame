#pragma once
#include <WinBase.h>

template<class T> class TrecBoxPointer
{
private:
	T* rawPointer;
	UINT counter;
public:
	TrecBoxPointer(T* t)
	{
		counter = (t)? 1 : 0;
		rawPointer = 0;
	}

	~TrecBoxPointer()
	{
		if (rawPointer)
			delete rawPointer;
	}

	void Increment()
	{
		counter++;
	}

	void Decrement()
	{
		counter--;
		if (!counter)
			delete this;
	}

	T* Get()
	{
		return rawPointer;
	}

	void Delete()
	{
		delete rawPointer;
		rawPointer = nullptr;
	}
};

template<class T> class TrecComBoxPointer
{
private:
	T* rawPointer;
	UINT counter;
public:
	TrecComBoxPointer(T* t)
	{
		counter = (t) ? 1 : 0;
		rawPointer = 0;
	}

	~TrecComBoxPointer()
	{
		if (rawPointer)
			rawPointer->Release();
	}

	void Increment()
	{
		counter++;
	}

	void Decrement()
	{
		counter--;
		if (!counter)
			delete this;
	}

	T* Get()
	{
		return rawPointer;
	}

	void Delete()
	{
		rawPointer->Release();
		rawPointer = nullptr;
	}
};

template<class t> class TrecPointer
{
	friend class TrecPointerKey;
protected:
	TrecBoxPointer<t>* pointer;
	TrecPointer(t* raw)
	{
		if (!raw)
			throw L"Error! TrecPointers must be initialized with a pointer, not NULL!";
		pointer = new TrecBoxPointer<t>(raw);
	}
public:
	TrecPointer(const TrecPointer<t>& copy)
	{
		pointer = copy.pointer;
		pointer->Increment();
	}

	TrecPointer()
	{
		pointer = nullptr;
	}

	void Nullify()
	{
		if (pointer)
			pointer->Decrement();
		pointer = nullptr;
	}

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

	t* Get()
	{
		if (!pointer) return nullptr;
		return pointer->Get();
	}

	t* operator->()
	{
		if (!pointer) return nullptr;
		return pointer->Get();
	}

	void Delete()
	{
		if (!pointer) return;
		pointer->Delete();
	}
};

class TrecPointerKey
{
public:
	template <class t, class...types> static TrecPointer<t> GetNewTrecPointer(types&& ... args)
	{
		t* raw = new t(args...);
		TrecPointer<t> ret(raw);
		return ret;
	}

	template <class t, class u, class...types> static TrecPointer<t> GetNewTrecPointerAlt(types&& ... args)
	{
		u* raw = new u(args);
		t* raw_t = nullptr;


		// Need to make sure that the alternative type 'u' is compatible with this type 't'
		try
		{
			raw_t = dynamic_cast<t*>(raw);
		}
		catch (std::bad_cast& ex)
		{
			raw_t = nullptr;
		}

		if (!raw_t)
		{
			delete raw;
			throw L"Error! Attempted to assign incompatible type!";
		}
		TrecPointer<t> ret(raw_t);
		return raw;
	}
};

template<class t> class TrecComPointer
{
protected:
	TrecComBoxPointer<t>* pointer;
public:
	class TrecComHolder
	{
	protected:
		t* holder;

	public:
		t** GetPointerAddress()
		{
			return &holder;
		}

		void OfferValue(TrecComPointer<t>& point)
		{
			holder = point.Get();
		}

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

		TrecComPointer<t> Extract()
		{
			if (!holder)
				throw L"Error! Cannot Extract Smart Pointer with raw value!";
			TrecComPointer<t> ret(holder);
			holder = nullptr;
			return ret;
		}

		template<class u> TrecComPointer<u> ExtractAs()
		{
			if (!holder)
				throw (L"Error!, Cannot Extract Smart Pointer with raw value!");

			u* u_holder = dynamic_cast<u*>(holder);

			if (!u_holder)
			{
				holder->Release();
				holder = nullptr;
				throw (L"Error! Attempted to Extract Com Object into incompatible type!");
			}

			holder = nullptr;

			TrecComPointer<u> ret(u_holder);
			return ret;
		}
	};

protected:
	TrecComPointer(t* raw)
	{
		if (!raw)
			throw (L"Error! TrecPointers must be initialized with a pointer, not NULL!");
		pointer = new TrecComBoxPointer<t>(raw);
	}

public:

	TrecComPointer()
	{
		pointer = nullptr;
	}

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


	void Nullify()
	{
		if (pointer)
			pointer->Decrement();
		pointer = nullptr;
	}

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

	t* Get()
	{
		if (!pointer)return nullptr;
		return pointer->Get();
	}

	t* operator->()
	{
		if (!pointer)return nullptr;
		return pointer->Get();
	}

	void Delete()
	{
		if(pointer)
		pointer->Delete();
	}

	TrecComPointer(const TrecComPointer<t>& copy)
	{
		pointer = copy.pointer;
		pointer->Increment();
	}
};