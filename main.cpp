#include "ParticipantClass.h"
#include "PublisherClass.h"
#include "SubscriberClass.h"
#include "DataWriter_Aux2Strings.h"
#include "DataReader_Aux2Strings.h"
#include "DataReader_V2XMessage.h"
#include "DataWriter_V2XMessage.h"
#include "DataWriter_VehData.h"
#include "DataReader_VehData.h"

#include "Csv.h"



#include "TimeSync.h"
#include <string> 
#include <thread>

#include <vector>

#include <iostream> //cout


#include <iomanip>	
#include <ctime> //time format

#include <sstream>

#include <cstdlib> //create folder

#include "main.h"

using std::cerr;
using std::cout;
using std::endl;
using std::string;

std::vector<std::vector<std::string> > buffer(2, std::vector<std::string>(BUFFER_SIZE));
int fileName_index;
std::string fileName;


Mri::Aux2StringsDataWriter_var  writer_global_aux2strings;
Mri::V2XMessageDataWriter_var  writer_global_v2xmessage;
Mri::VehDataDataWriter_var writer_global_vehdata;

//std::vector<string> buffor;



int index_buffer = 1;
int full_buffer_count = 0;
long sample_count_buffer = 0;
long sample_count_total = 0;




void switchBuffer() {
	if (index_buffer==0)
	{
		index_buffer = 1;
		
	}
	else
	{
		index_buffer = 0;
	}
	sample_count_buffer = 0;	
	full_buffer_count += BUFFER_SIZE;
	fileName_index++;
	
}

void initialize() {


	std::stringstream bufferFileName;
	std::string _command;

	std::time_t t = std::time(nullptr);
	std::tm tm = *std::localtime(&t);

	bufferFileName  << std::put_time(&tm, "%Y%m%d_%I-%M-%p");
	
	fileName = bufferFileName.str();




	//const char* _command = bufferFileName.str().c_str();

	_command = "mkdir " + fileName;




	//const int dir = system("mkdir -p foo");

	const int dir = system(_command.c_str());
	if (dir< 0)
	{
		return;
	}
	cout << fileName << endl;



	fileName_index = 0;  //switchBuffer below will increase fileName_index
	switchBuffer();		// fileName_index = 1

}


int main(int argc, char* argv[]) {

	//ACE_TCHAR *argv2[] = { "-DCPSConfigFile","rtps.ini" };
	//-DCPSConfigFile rtps.ini


	// start thread
	std::thread threadTimestamp (TimestampThread);
	std::cout << "*****************   Timestamp: " << GetTimestamp() << std::endl << std::endl;
	
	initialize();

	

	Mri::VehData test_veh;

	test_veh.color = 12;
	test_veh.lane_index = 2;
	test_veh.link_name = "Winter Street 12";

	string z = "";

	cout << "Start filling buffor at: " << GetTimestamp() << endl;


	
	

	
	


	
	
	for (size_t i = 0; i < 19870; i++)
	{
		test_veh.timestamp = GetTimestamp();

		z=csvConvertVehDataToString(test_veh);
		sample_count_buffer++;
		sample_count_total++;
		//buffor.push_back(z);
		

		if (sample_count_total>=full_buffer_count)
		{
			//save file();
			csvWriteFile( index_buffer, sample_count_buffer);
			switchBuffer();
		}
		else
		{
			buffer[index_buffer][sample_count_buffer] = z;
			
		}		
		
		
	}


	if (sample_count_buffer>0) {
		//save the last samples
		csvWriteFile(index_buffer, sample_count_buffer);
	}
	
	
	


	//ParticipantClass participant(argc, argv);
	//SubscriberClass subscriber(participant.m_participant);
	//PublisherClass publisher(participant.m_participant);

	//TimeSynchronization(participant.m_participant, subscriber.m_subscriber, publisher.m_publisher);

	//
	//////create reader to receive V2X message  Mri_V2XfromNS3  Mri_V2XtoNS3
	////DataReader_V2XMessage reader_v2xmessage(participant.m_participant,subscriber.m_subscriber,"Mri_V2XfromNS3");
	////DataWriter_V2XMessage writer_v2xMessage(participant.m_participant, publisher.m_publisher,"Mri_V2XtoNS3");

	//// topics:	TrafficVeh SubjectCar
	//DataReader_VehData reader_vehdata(participant.m_participant, subscriber.m_subscriber, "Mri_TrafficVeh");
	//DataWriter_VehData writer_vehdata(participant.m_participant, publisher.m_publisher, "Mri_SubjectCar");

	//



	//Mri::VehData veh_message;
	//for (size_t i = 0; i < 11565530; i++)
	//{
	//	veh_message.vehicle_id = 33;
	//	veh_message.position_x = i;
	//	veh_message.position_y = -3450.01;
	//	veh_message.timestamp = GetTimestamp();
	//	writer_vehdata.sendMessage(veh_message);
	//	Sleep(50);	
	//}



	///*Mri::V2XMessage v2x_message;
	//for (size_t i = 0; i < 1156530; i++)
	//{
	//	v2x_message.sender_id = THIS_APP_ID;
	//	v2x_message.sender_timestamp = GetTimestamp();
	//	v2x_message.recipient_id = 0;
	//	v2x_message.message = "X=12.23;Y=-1636.34";
	//	writer_v2xMessage.sendMessage(v2x_message);
	//	Sleep(50);
	//}*/




	threadTimestamp.join();


	/*while (true)
	{
		Sleep(1000);
	}*/

	return 0;
}
