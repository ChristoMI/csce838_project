// Assumes the input is correctly formatted

// import libraries
#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
using namespace std;

int main (int argc, char* argv[])
{
	// takes program input
	int errorCode = atoi(argv[1]);
	string errorDesc = argv[2];
	
	// gets log event time
	time_t rawTime;
	struct tm* eventTime;
	time(&rawTime);
	eventTime = localtime(&rawTime);
	string eventTimeString = asctime(eventTime);

	// checks reset error
	/*
	if(errorCode == 0)
	{
		// check /sys/kernel/pmc/tegra_reset_reason
		// set description to reset reason
	}
	*/
	
	// builds string to write
	string errorCodeString = to_string(errorCode);
	string outputString = eventTimeString + "ERR CODE: " + errorCodeString + " DESC: " + errorDesc + "\n\n"; 
	
	// checks for existing log file
	bool existFlag = false;
	ifstream filePointer;
	filePointer.open("errorLog.txt");
	if(filePointer)
	{
		existFlag = true;
	}
	filePointer.close();

	// opens log file
	ofstream outputFilePointer;
	if(existFlag == true)
	{
		outputFilePointer.open("errorLog.txt", ios_base::app);
	}
	else
	{
		outputFilePointer.open("errorLog.txt");
	}
	
	// writes to log file
	outputFilePointer << outputString;
	
	// closes file pointer
	outputFilePointer.close();
}