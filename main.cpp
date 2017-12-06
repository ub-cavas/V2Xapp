#include "ParticipantClass.h"
#include "PublisherClass.h"
#include "SubscriberClass.h"
#include "DataWriter_Aux2Strings.h"
#include "DataReader_Aux2Strings.h"
#include "DataReader_V2XMessage.h"
#include "DataWriter_V2XMessage.h"
#include "DataWriter_VehData.h"
#include "DataReader_VehData.h"
#include "OpenDDSThread.h"

#include "Csv.h"



#include "TimeSync.h"
#include <string> 
#include <thread>

#include <vector>

#include <iostream> //cout


#include <iomanip>	
#include <ctime> //time format
#include <conio.h> //key press

#include <sstream>

#include <cstdlib> //create folder

#include "main.h"

using std::cerr;
using std::cout;
using std::endl;
using std::string;



//Mri::Aux2StringsDataWriter_var  writer_global_aux2strings;
//Mri::V2XMessageDataWriter_var  writer_global_v2xmessage;

bool finish_application;




int main(int argc, char* argv[]) {

	//ACE_TCHAR *argv2[] = { "-DCPSConfigFile","rtps.ini" };
	//-DCPSConfigFile rtps.ini

	finish_application = false;

	// start thread
	std::thread threadOpenDDS(OpenDDSThread,argc,argv);






	//// start thread
	//std::thread threadTimestamp (TimestampThread);
	////std::cout << "*****************   Timestamp: " << GetTimestamp() << std::endl << std::endl;
	//
	//initialize();
	//cout << endl;

	//ParticipantClass participant(argc, argv);
	//SubscriberClass subscriber(participant.m_participant);
	//PublisherClass publisher(participant.m_participant);
	//cout << endl;
	//TimeSynchronization(participant.m_participant, subscriber.m_subscriber, publisher.m_publisher);

	//
	////create reader to receive V2X message  Mri_V2XfromNS3  Mri_V2XtoNS3
	//DataReader_V2XMessage reader_v2xmessage(participant.m_participant,subscriber.m_subscriber,"Mri_V2XfromNS3");
	//DataWriter_V2XMessage writer_v2xMessage(participant.m_participant, publisher.m_publisher,"Mri_V2XtoNS3");

	//// topics:	Mri_TrafficVeh Mri_SubjectCar
	//DataReader_VehData reader_vehdata(participant.m_participant, subscriber.m_subscriber, "Mri_TrafficVeh");
	//
	//

	while (!finish_application)
	{
		Sleep(300);

	}




	threadOpenDDS.detach();



	/*while (true)
	{
		Sleep(1000);
	}*/

	return 0;
}
