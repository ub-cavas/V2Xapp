#include <chrono>
#include <thread>
#include <iostream> //cout
#include <vector>
#include <stdlib.h> //atol
#include <algorithm> // sort


#include "DataWriter_Aux2Strings.h"
#include "DataReader_Aux2Strings.h"
#include "MriTimeSync.h"

using std::cerr;
using std::cout;
using std::endl;
using std::string;


long timestamp_master;

long stepsCounter = 0;
long delaySum = 0;

std::vector<long> packageDelay;

std::vector<long> timestampOffsetServerApp;	

long lastTimestampFromServer;

long THIS_APP_ID = -1;	// you have to set unique THIS_APP_ID

void TimestampThread()
{
	// master timestamp
	//increase every 10 ms

	//initialization
	timestamp_master = 0;

	

	THIS_APP_ID = GenerateAPP_ID();

	std::chrono::time_point<std::chrono::system_clock> t = std::chrono::system_clock::now();

	while (true)
	{
		timestamp_master += 1;
		t += std::chrono::milliseconds(10);
		std::this_thread::sleep_until(t);
	}

}


long GenerateAPP_ID() {
	
	long _id=0;
	srand(time(NULL));

	while (_id==0)
	{
		_id = rand() % 2147483600;  //  2147483600 < max(long)
	}
	return _id;
}



bool TimeSynchronization(DDS::DomainParticipant_var m_participant, DDS::Subscriber_var m_subscriber, DDS::Publisher_var m_publisher)
{

	DataReader_Aux2Strings reader(m_participant, m_subscriber, "Mri_Control");
	DataWriter_Aux2Strings sender(m_participant, m_publisher, "Mri_Control");

	packageDelay.clear();
	lastTimestampFromServer = 0;

	std::chrono::time_point<std::chrono::system_clock> t = std::chrono::system_clock::now();

	for (size_t i = 0; i < 10; i++)
	{
		SendSyncMessage();
		t += std::chrono::milliseconds(50);	//each loop 50 ms
		std::this_thread::sleep_until(t);
	}

	//wait 500 ms additionaly
	t += std::chrono::milliseconds(500);
	std::this_thread::sleep_until(t);

	if (packageDelay.size() < 5)
	{
		//repeat proccess

		for (size_t i = 0; i < 10; i++)
		{
			SendSyncMessage();
			t += std::chrono::milliseconds(50);	//each loop 50 ms
			std::this_thread::sleep_until(t);
		}
		//wait 1000 ms additionaly
		t += std::chrono::milliseconds(1000);
		std::this_thread::sleep_until(t);

		if (packageDelay.size() < 5) {
			//there is a problem with time synchronization
			//quit
			return false;
		}
	}

	

	long  delayToAndFrom = MedianPackageDelay();
	long  offsetServerApp = MedianOffsetServerApp();

	cout << "Delay = " << delayToAndFrom / 2 << endl;

	SetTimestamp(GetTimestamp() + offsetServerApp + delayToAndFrom/2);

	return true;
}

void SetTimestamp(long perfectTimestamp)
{
	timestamp_master = perfectTimestamp;
}

long GetTimestamp()
{
	return timestamp_master;
}

bool ParseAux2Strings(Mri::Aux2Strings aux_message)
{

	if (aux_message.receiverId != THIS_APP_ID)
	{
		return true;
	}


	if (aux_message.senderId == SERVER_ID && strcmp(aux_message.tag, TAG_TIME_SYNC.c_str()) == 0)
	{// time synchronization message

		long _delay = GetTimestamp() - atol(aux_message.str1);				//aux_message.str2 - timestamp from server
																			//aux_message.str1 - timestamp from this app
		long _offset = atol(aux_message.str2) - atol(aux_message.str1);

		packageDelay.emplace(packageDelay.end(), _delay);
		timestampOffsetServerApp.emplace(timestampOffsetServerApp.end(), _offset);

		/*if (atol(aux_message.str2)> lastTimestampFromServer)
		{
			lastTimestampFromServer = atol(aux_message.str2);
		}*/
		//std::cout << "Timestamp: " << GetTimestamp() << std::endl << std::endl;
	}

	

	return true;
}


long MedianPackageDelay() {
	
	int size = packageDelay.size();


	std::sort(packageDelay.begin(), packageDelay.end());

	long _median = size % 2 ? packageDelay[size / 2] : (packageDelay[size / 2 - 1] + packageDelay[size / 2]) / 2;

	return _median;
}


long MedianOffsetServerApp() {

	int size = timestampOffsetServerApp.size();


	std::sort(timestampOffsetServerApp.begin(), timestampOffsetServerApp.end());

	long _median = size % 2 ? timestampOffsetServerApp[size / 2] : (timestampOffsetServerApp[size / 2 - 1] + timestampOffsetServerApp[size / 2]) / 2;

	return _median;
}



bool SendSyncMessage() {
	Mri::Aux2Strings auxMessage;

	std::string s = std::to_string(GetTimestamp());

	auxMessage.receiverId = SERVER_ID;
	auxMessage.senderId = THIS_APP_ID;
	auxMessage.tag = "timesync";
	auxMessage.str1 = s.c_str();
	auxMessage.str2 = "";

	int success = writer_global_aux2strings->write(auxMessage, DDS::HANDLE_NIL);
	if (success != DDS::RETCODE_OK) {
		ACE_ERROR((LM_ERROR, ACE_TEXT("(%P|%t) ERROR: TimeSync send message write returned %d.\n"), success));
	}

	std::cout << "Sent timesync message at: " << GetTimestamp() << std::endl << std::endl;
	return true;
}




