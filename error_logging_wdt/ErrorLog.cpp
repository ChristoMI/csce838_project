// Assumes the input is correctly formatted

// import libraries
#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
using namespace std;

string reg_path = "/sys/kernel/pmc/tegra_reset_reason";
string error_log_path = "/home/car/ryan_brown_ws/src/bluetooth_com/output/errorLog.txt";

int main (int argc, char* argv[])
{
	// takes program input
	int errorCode = atoi(argv[1]);

	string errorDesc = "";

	if(argc > 2)
		errorDesc = argv[2];

	cout << "Received an error: [" << errorCode << "] " << errorDesc << endl;
	
	// gets log event time
	time_t rawTime;
	struct tm* eventTime;
	time(&rawTime);
	eventTime = localtime(&rawTime);
	string eventTimeString = asctime(eventTime);

	// checks reset error
	if(errorCode == 0)
	{
		// check /sys/kernel/pmc/tegra_reset_reason, set description to reset reason
		cout << "Checking the last reset...\n";

		ifstream resetReasonPointer;
		resetReasonPointer.open(reg_path);
		getline(resetReasonPointer, errorDesc);
		//errorDesc = "test";
		resetReasonPointer.close();	
	}
	
	// builds string to write
	cout << "Composing the error log info...\n";
	string errorCodeString = to_string(errorCode);
	string outputString = eventTimeString + "ERR CODE: " + errorCodeString + " DESC: " + errorDesc + "\n\n"; 
	
	// checks for existing log file
	bool existFlag = false;
	ifstream filePointer;
	filePointer.open(error_log_path);
	if(filePointer)
	{
		existFlag = true;
	}
	filePointer.close();

	// opens log file
	ofstream outputFilePointer;
	if(existFlag == true)
	{
		outputFilePointer.open(error_log_path, ios_base::app);
	}
	else
	{
		outputFilePointer.open(error_log_path);
	}
	
	cout << "Updating the log...\n";
	
	// writes to log file
	outputFilePointer << outputString;
	
	// closes file pointer
	outputFilePointer.close();
}