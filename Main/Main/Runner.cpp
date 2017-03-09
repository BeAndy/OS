#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <windows.h>
#include <conio.h>
#include <string>
#include <wchar.h>

using namespace std;


struct Order {
	int    code;
	char   name[40];
	double price;
	int    amountRealized;
	int    amountUnrealized;
};

wchar_t *getWC(const char *c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);
	return wc;
}

void outData(ostream & out, Order &currentOrder) {
	out << "\nOrder report"<<endl;
	out << "Code: " << currentOrder.code << endl;
	out << "Name: " << currentOrder.name << endl;
	out << "Price per one: " << currentOrder.price << endl;
	out << "Relized: " << currentOrder.amountRealized << endl;
	out << "Unrelized: " << currentOrder.amountUnrealized <<"\n"<<endl;
}

int main(){
	wchar_t firstUtilite[300] = L"C:\\Users\\Andrew\\Source\\Repos\\OS-1\\Main\\Debug\\Creatoor.exe ";
	wchar_t secondUtilite[300] = L"C:\\Users\\Andrew\\Source\\Repos\\OS-1\\Main\\Debug\\Reporter.exe ";
	STARTUPINFO firstProcess;
	PROCESS_INFORMATION firstProcessInfo;
	ZeroMemory(&firstProcess, sizeof(STARTUPINFO));
	firstProcess.cb = sizeof(STARTUPINFO);

	STARTUPINFO secondProcess;
	PROCESS_INFORMATION secondProcessInfo;
	ZeroMemory(&secondProcess, sizeof(STARTUPINFO));
	secondProcess.cb = sizeof(STARTUPINFO);

	char *binPath=new char[20];
	cout << "Enter binary file name \n";
	cin >> binPath;
	wcscat(firstUtilite, getWC(binPath));
	wcscat(secondUtilite, getWC(binPath));
	wcscat(secondUtilite, L" ");

	CreateProcess(NULL,firstUtilite, NULL, NULL,FALSE,
		CREATE_NEW_CONSOLE, NULL, NULL, &firstProcess, &firstProcessInfo);
	WaitForSingleObject(firstProcessInfo.hProcess, INFINITE);

	string pathResult = binPath;
	pathResult += ".bin";
	Order currentOrder;
	ifstream inBinary(pathResult, ios::in | ios::binary);
	while (!inBinary.eof()) {
		inBinary.read((char*)&currentOrder, sizeof(Order));
		if(!inBinary.eof())
			outData(cout, currentOrder);
	}
	inBinary.close();

    char* reportFileName=new char[20];
	cout << "Enter report file name \n";
	cin >> reportFileName;
	wcscat(secondUtilite, getWC(reportFileName));
	wcscat(secondUtilite, L" ");

	char* reportCode = new char[20];
	cout << "Enter report code (1-relized/2-unrelized):\n";
	cin >> reportCode;
	cout << endl;
	wcscat(secondUtilite, getWC(reportCode));
	CreateProcess(NULL, secondUtilite, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE, NULL, NULL, &secondProcess, &secondProcessInfo);
	WaitForSingleObject(secondProcessInfo.hProcess, INFINITE);

	pathResult = reportFileName;
	pathResult +=".txt";
	ifstream inTxt(pathResult, ios::in);
	string aboutReport;
	while (!inTxt.eof()) {
		getline(inTxt, aboutReport);
		cout <<aboutReport<< endl;
	}
	inTxt.close();

	CloseHandle(firstProcessInfo.hThread);
	CloseHandle(firstProcessInfo.hProcess);
	CloseHandle(secondProcessInfo.hThread);
	CloseHandle(secondProcessInfo.hProcess);

	system("pause");
	return 0;
}