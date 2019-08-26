#pragma once

#include "TStringTester.h"

class TestRunner
{
public:
	void DoTests();

	void Draw(HWND window, PAINTSTRUCT structure);

private:
	std::string testResults;
};


void RunTests();
void DoDraw(HWND window, PAINTSTRUCT structure);

