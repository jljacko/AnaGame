#include "TestRunner.h"


TestRunner tester;


void TestRunner::DoTests()
{
	TStringTester tStringTester;
	tStringTester.Run(testResults);
}

void TestRunner::Draw(HWND window, PAINTSTRUCT structure)
{
	const char* stringRes = testResults.c_str();
	char* paramStr = new char[testResults.size() + 1];
	for (UINT c = 0; c < testResults.size(); c++)
	{
		paramStr[c] = stringRes[c];
	}

	paramStr[testResults.size()] = '\0';


	DrawTextExA(structure.hdc, paramStr, testResults.size(), &structure.rcPaint, 
		DT_INTERNAL | DT_LEFT | DT_TOP, nullptr);
}

void RunTests()
{
	tester.DoTests();
}

void DoDraw(HWND window, PAINTSTRUCT structure)
{
	tester.Draw(window, structure);
}
