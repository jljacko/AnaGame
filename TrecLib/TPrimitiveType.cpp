#include "TPrimitiveType.h"

TPrimitiveType::TPrimitiveType(int size, BYTE type, TString& name): TType(name)
{
	this->size = size;
	this->type = type;
}

int TPrimitiveType::GetByteSize()
{
	return size;
}

code_type_cat TPrimitiveType::GetTypeCategory()
{
	return code_type_cat::code_type_cat_prim;
}

BYTE TPrimitiveType::GetType()
{
	return type;
}

TrecPointer<TTypeMetadata> TPrimitiveType::GetMetadata()
{
	return TrecPointer<TTypeMetadata>();
}

TrecPointer<TAttribute> TPrimitiveType::GetAttributeByName(TString&)
{
	return TrecPointer<TAttribute>();
}

TrecPointer<TProcedure> TPrimitiveType::GetProcedureByName(TString&)
{
	return TrecPointer<TProcedure>();
}
