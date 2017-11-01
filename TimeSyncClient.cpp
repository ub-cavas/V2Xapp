
//rd timer
#include <chrono>
#include <thread>

#include <stdlib.h> //atol
#include <cmath>	//lround
#include <iostream> //cout


#include "TimeSync.h"
#include "DataWriter_Aux2Strings.h"

using std::cerr;
using std::cout;
using std::endl;
using std::string;


long timestamp_master;

long stepsCounter = 0;
long delaySum = 0;

void TimestampThread()
{
	// master timestamp
	//increase every 10 ms

	timestamp_master = 0;
	std::chrono::time_point<std::chrono::system_clock> t = std::chrono::system_clock::now();

	while (true)
	{
		timestamp_master += 1;
		t += std::chrono::milliseconds(10);
		std::this_thread::sleep_until(t);
	}


}



void AddNextDelay(Mri::Aux2Strings auxMessage, long timestampNow) {

	long substraction = timestampNow - atol(auxMessage.str1);
	delaySum += substraction;
	stepsCounter++;
	//std::cout << "Add next delay Steps: " << stepsCounter << std::endl << std::endl;

	if (stepsCounter>4)
	{	//after 5 times do
		//synchronization
		long average_delay = std::lround(delaySum / 10);		// from 5 samples, then take a half delay time (1 way) 
		long timestamp_perf = atol(auxMessage.str2) + average_delay;
		std::cout << "Average delay: " << average_delay << std::endl;
		std::cout << "Previous timestamp: " << GetTimestamp() << "  Current timestamp: " << timestamp_perf << std::endl;

		SetTimestamp(timestamp_perf);

		std::cout  << "Time synchronized" <<  std::endl << "##############################################" << std::endl << std::endl;
		
		

		//clean-up
		delaySum = 0;
		stepsCounter = 0;
	}

}



long GetTimestamp() {
	return timestamp_master;
}

void SetTimestamp(long timestamp_perfect) {

	timestamp_master = timestamp_perfect;
}

bool SendSyncMessage() {
	Mri::Aux2Strings auxMessage;

	std::string s = std::to_string(GetTimestamp());
	/*char* p = new char[s.length() + 1];
	memcpy(p, s.c_str(), s.length() + 1);*/


	auxMessage.receiverId = SERVER_ID;
	auxMessage.senderId = THIS_APP_ID;
	auxMessage.tag ="timesync";
	auxMessage.str1 = s.c_str();
	auxMessage.str2 = "";

	int success = writer_global_aux2strings->write(auxMessage, DDS::HANDLE_NIL);
	if (success != DDS::RETCODE_OK) {
		ACE_ERROR((LM_ERROR, ACE_TEXT("(%P|%t) ERROR: TimeSync send message write returned %d.\n"), success));
	}
	//sender.sendMessage(auxMessage);

	//std::cout << "Send timesync message at: " << GetTimestamp() << std::endl << std::endl;
	return true;
}

bool SynchronizeTime() {

	//reset variables
	stepsCounter = 0;
	delaySum = 0;


	for (size_t i = 0; i < 6; i++)
	{
		//send 5 + 1 sync messages
		// we need just 5 messages, we add one more just in case

		SendSyncMessage();

		Sleep(300);

	}

	
	return true;
}

bool ParseAux2Strings(Mri::Aux2Strings aux_message)
{

	if (aux_message.receiverId!=THIS_APP_ID)
	{
		return true;
	}


	if (aux_message.senderId==SERVER_ID && strcmp(aux_message.tag, TAG_TIME_SYNC.c_str()) == 0)
	{// time synchronization message

		AddNextDelay(aux_message, GetTimestamp());
		//std::cout << "Timestamp: " << GetTimestamp() << std::endl << std::endl;

	}

	return true;
}



void TimeSynchronization(DDS::DomainParticipant_var m_participant,DDS::Subscriber_var m_subscriber, DDS::Publisher_var m_publisher) {
	
	DataReader_Aux2Strings reader(m_participant, m_subscriber, "Mri_Control");
	DataWriter_Aux2Strings sender(m_participant, m_publisher, "Mri_Control");

	//we have to repeat SynchrinizeTime. It seems it loses a few packets at the very beginning !
	std::cout << "Time synchronization..."  << std::endl;
	SynchronizeTime();
	Sleep(100);
	SynchronizeTime();

	//wait 2 sec to finish synchronization process and then dispose reader and sender
	Sleep(2000);
}
