#include "ParticipantClass.h"
#include "PublisherClass.h"
#include "SubscriberClass.h"
#include "DataWriter_Aux2Strings.h"
#include "DataReader_Aux2Strings.h"
#include "DataReader_V2XMessage.h"
#include "DataWriter_V2XMessage.h"



#include "TimeSync.h"
#include <string> 
#include <thread>

#include <iostream> //cout

using std::cerr;
using std::cout;
using std::endl;
using std::string;




Mri::Aux2StringsDataWriter_var  writer_global_aux2strings;
Mri::V2XMessageDataWriter_var  writer_global_v2xmessage;
Mri::VehDataDataWriter_var writer_global_vehdata;

int main(int argc, char* argv[]) {

	//ACE_TCHAR *argv2[] = { "-DCPSConfigFile","rtps.ini" };
	//-DCPSConfigFile rtps.ini



	// start thread
	std::thread threadTimestamp (TimestampThread);
	std::cout << "*****************   Timestamp: " << GetTimestamp() << std::endl << std::endl;
	
	
	ParticipantClass participant(argc, argv);
	SubscriberClass subscriber(participant.m_participant);
	PublisherClass publisher(participant.m_participant);

	TimeSynchronization(participant.m_participant, subscriber.m_subscriber, publisher.m_publisher);

	
	//create reader to receive V2X message  Mri_V2XfromNS3  Mri_V2XtoNS3
	DataReader_V2XMessage reader_v2xmessage(participant.m_participant,subscriber.m_subscriber,"Mri_V2XfromNS3");
	DataWriter_V2XMessage writer_v2xMessage(participant.m_participant, publisher.m_publisher,"Mri_V2XtoNS3");

	Mri::V2XMessage v2x_message;
	for (size_t i = 0; i < 1156530; i++)
	{
		v2x_message.sender_id = THIS_APP_ID;
		v2x_message.sender_timestamp = GetTimestamp();
		v2x_message.recipient_id = 0;
		v2x_message.message = "X=12.23;Y=-1636.34";
		writer_v2xMessage.sendMessage(v2x_message);
		Sleep(50);	
	}

	threadTimestamp.join();

	return 0;
}
