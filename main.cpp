#include "ParticipantClass.h"
#include "PublisherClass.h"
#include "SubscriberClass.h"
#include "DataWriter_Aux2Strings.h"
#include "DataReader_Aux2Strings.h"



#include "TimeSync.h"
#include <string> 
#include <thread>

#include <iostream> //cout

using std::cerr;
using std::cout;
using std::endl;
using std::string;




Mri::Aux2StringsDataWriter_var  writer_global;

int main(int argc, char* argv[]) {

	//ACE_TCHAR *argv2[] = { "-DCPSConfigFile","rtps.ini" };
	//-DCPSConfigFile rtps.ini



	// start thread
	std::thread threadTimestamp (TimestampThread);

	std::cout << "Timestamp: " << GetTimestamp() << std::endl << std::endl;
	
	
	ParticipantClass participant(argc, argv);
	SubscriberClass subscriber(participant.m_participant);
	PublisherClass publisher(participant.m_participant);

	TimeSynchronization(participant.m_participant, subscriber.m_subscriber, publisher.m_publisher);

	

	for (size_t i = 0; i < 30; i++)
	{

		Sleep(20000);
		TimeSynchronization(participant.m_participant, subscriber.m_subscriber, publisher.m_publisher);
		
	}

	threadTimestamp.detach();

	return 0;
}
