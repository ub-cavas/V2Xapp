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
#include <conio.h> //key press

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
int file_index_to_save = 1;

bool getInput(char *c)
{
	if (_kbhit())
	{
		*c = _getch();
		return true; // Key Was Hit
	}
	return false; // No keys were pressed
}


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

	//cout << "Switch " << fileName_index << endl;
	
}

void initialize() {


	std::stringstream bufferFileName;
	std::string _command;

	std::time_t t = std::time(nullptr);
	std::tm tm = *std::localtime(&t);

	bufferFileName  << std::put_time(&tm, "%Y%m%d_%I-%M-%p");
	
	fileName = bufferFileName.str();
	_command = "mkdir " + fileName;


	//const int dir = system("mkdir -p foo"); 
	const int dir = system(_command.c_str()); // -p parameters doesn't work
	if (dir< 0)
	{
		return;
	}
	//cout << fileName << endl;



	fileName_index = 0;  //switchBuffer below will increase fileName_index
	switchBuffer();		// fileName_index = 1

}


int main(int argc, char* argv[]) {

	//ACE_TCHAR *argv2[] = { "-DCPSConfigFile","rtps.ini" };
	//-DCPSConfigFile rtps.ini

	char key = ' ';



	// start thread
	std::thread threadTimestamp (TimestampThread);
	//std::cout << "*****************   Timestamp: " << GetTimestamp() << std::endl << std::endl;
	
	initialize();
	cout << endl;

	ParticipantClass participant(argc, argv);
	SubscriberClass subscriber(participant.m_participant);
	PublisherClass publisher(participant.m_participant);
	cout << endl;
	TimeSynchronization(participant.m_participant, subscriber.m_subscriber, publisher.m_publisher);

	//
	//////create reader to receive V2X message  Mri_V2XfromNS3  Mri_V2XtoNS3
	////DataReader_V2XMessage reader_v2xmessage(participant.m_participant,subscriber.m_subscriber,"Mri_V2XfromNS3");
	////DataWriter_V2XMessage writer_v2xMessage(participant.m_participant, publisher.m_publisher,"Mri_V2XtoNS3");

	// topics:	TrafficVeh SubjectCar
	DataReader_VehData reader_vehdata(participant.m_participant, subscriber.m_subscriber, "Mri_TrafficVeh");
	//DataWriter_VehData writer_vehdata(participant.m_participant, publisher.m_publisher, "Mri_SubjectCar");

	//


	

	while (key != 'q')
	{
		Sleep(200);
		
		if (fileName_index > file_index_to_save) {

			int index_buffer_to_save;
			if (index_buffer == 0) {
				index_buffer_to_save = 1;
			}
			else {
				index_buffer_to_save =0;
			}

			csvWriteFile(index_buffer_to_save, BUFFER_SIZE);
			file_index_to_save = fileName_index;
		}
		getInput(&key);

	}


	//closing app


	if (sample_count_buffer>0) {
		//save the last samples
		csvWriteFile(index_buffer, sample_count_buffer);
	}



	threadTimestamp.detach();



	/*while (true)
	{
		Sleep(1000);
	}*/

	return 0;
}
