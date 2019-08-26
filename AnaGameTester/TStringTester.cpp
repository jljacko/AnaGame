#include "TStringTester.h"

void TStringTester::RunTests(std::string& output)
{
	TestFind(output);
	TestInsert(output);
	TestAppendChar(output);
	TestFormet(output);
	TestDelete(output);
	TestLower(output);
	TestUpper(output);
	TestReverse(output);
	TestTrim(output);
	TestReplace(output);
}

std::string TStringTester::GetTestName()
{
	return std::string("TString");
}

void TStringTester::Before()
{
}

void TStringTester::After()
{
}

void TStringTester::TestFind(std::string& output)
{
	TString str(L"Star Wars Revenge of the Sith is the best movie!");
	std::string findStr("Find");
	ExpectEquals(output, findStr, static_cast<UINT>(48), str.GetSize());

	ExpectEquals(output, findStr, 4, static_cast<long long>(str.Find(L' ')));
	ExpectEquals(output, findStr, 41, static_cast<long long>(str.FindLast(L' ')));
	ExpectEquals(output, findStr, 9, static_cast<long long>(str.Find(L' ', 5)));
	ExpectEquals(output, findStr, 36, static_cast<long long>(str.FindLast(L' ', 40)));
	
	TString str2(L"'Why do you insist on torturing me?' asked the character!");
	TString s(L's');
	ExpectEquals(output, findStr, 38, static_cast<long long>(str2.FindOutOfQuotes(s)));
}

void TStringTester::TestInsert(std::string& output)
{
}

void TStringTester::TestAppendChar(std::string& output)
{
}

void TStringTester::TestFormet(std::string& output)
{
}

void TStringTester::TestDelete(std::string& output)
{
}

void TStringTester::TestLower(std::string& output)
{
}

void TStringTester::TestUpper(std::string& output)
{
}

void TStringTester::TestReverse(std::string& output)
{
}

void TStringTester::TestTrim(std::string& output)
{
}

void TStringTester::TestReplace(std::string& output)
{
}
