
#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/Service_Participant.h>

#include <string> 
#include <thread>
#include <iostream> //cout
#include <ctime> //time format
#include <conio.h> //key press
#include <map>	//map - list of vehicles



#include "MriTypeSupportImpl.h"
#include "DataWriter_Aux2Strings.h"
#include "DataReader_Aux2Strings.h"
#include "DataReader_V2XMessage.h"
#include "DataWriter_V2XMessage.h"
#include "DataReader_VehData.h"

#include "QueueTs.h"
#include "MriTimeSync.h"
#include "OpenDDS.h"
#include "V2xApps.h"

using std::cerr;
using std::cout;
using std::endl;
using std::string;


Mri::Aux2StringsDataWriter_var  writer_global_aux2strings;
Mri::V2XMessageDataWriter_var  writer_global_v2xmessage;
long veh_id_to_remove;

QueueTs<Mri::VehData> vehdata_queue;
std::map<long, Mri::VehData> vehs_map;
bool finish_application;
QueueTs<Mri::V2XMessage> v2x_queue;


bool getInput(char *c)
{
	if (_kbhit())
	{
		*c = _getch();
		return true; // Key Was Hit
	}
	return false; // No keys were pressed
}

void sendV2X(long sender_id, long sender_timestamp, string message) {
	
	Mri::V2XMessage v2x;
	
	v2x.sender_id = sender_id;
	v2x.sender_timestamp = sender_timestamp;
	v2x.message = message.c_str();
	v2x.recipient_id = -1;
	v2x.recipient_timestamp = -1;

	int success = writer_global_v2xmessage->write(v2x, DDS::HANDLE_NIL);
	if (success != DDS::RETCODE_OK) {
		//ACE_ERROR((LM_ERROR, ACE_TEXT("(%P|%t) ERROR: TimeSync send message write returned %d.\n"), success));
		throw std::string("ERROR: SendV2X message failed");
	}
	else
	{
		//cout << endl <<"     *****  SEND V2X message:" << v2x.message <<  " sender_id=" << v2x.sender_id << " sender_timestamp=" << v2x.sender_timestamp << endl;
	}
	
}

void vehsMapThread() {
	

	Mri::VehData _veh;
	
	//std::map<long, Mri::VehData> vehs_map;
	std::map<long, Mri::VehData>::iterator it;
	
	veh_id_to_remove = -1; // initial value
	
	while (!finish_application)
	{
		//wait for something at the queue
		vehdata_queue.pop(_veh);
		
			it = vehs_map.find(_veh.vehicle_id);
			if (it != vehs_map.end()) {
				//there is a car with id = _veh.vehicle_id


				//check if new data is older than in vehs_map
				if (_veh.timestamp >  it->second.timestamp)
				{
					//update data in vehs_map
					vehs_map[it->first] = _veh;
				}
				else
				{
					cout << "OpenDDS data is older than data in vehs_list. Veh_id=" << _veh.vehicle_id
						<< " timestamp_vehs_list=" << it->second.timestamp
						<< " timestamp_OpenDDS=" << _veh.timestamp << endl;
				}
			}
			else
			{
				//no car with id = _veh.vehicle_id
				vehs_map.emplace(_veh.vehicle_id, _veh);

			}



			//check if we have to remove a car

			if (veh_id_to_remove!=-1)
			{
				it = vehs_map.find(veh_id_to_remove);

				veh_id_to_remove = -1; //reset this variable

				if (it != vehs_map.end()) 
				{
					vehs_map.erase(it);
					//cout << endl << "####################################################################################" << endl;
					cout << endl << "#######  Removed from vehsMap vehicle_id =" << veh_id_to_remove << "   #####" << endl<<endl;
					
				}
				//veh_id_to_remove = -1; //reset this variable
			}

		
	}
}



void v2xMapThread() {

	// thread where app receives v2x messages from NS-3
	Mri::V2XMessage _v2x;


	while (!finish_application)
	{
		

		Mri::VehData subjectCar1;
		Mri::VehData _veh;

		
		float distance = -1;




		//wait for something at the queue
		v2x_queue.pop(_v2x);
		{
			
			//we do not process v2x dnpw => moved to opendds3 unity client
			
			//_veh = readVehDatafromString((string)_v2x.message);
			//subjectCar1 = vehs_map[1];
			//
			//if (subjectCar1.vehicle_id!= _veh.vehicle_id)
			//{
			//	distance = doNotPassWarning(subjectCar1.position_x, subjectCar1.position_y, subjectCar1.orient_heading, _veh.position_x, _veh.position_y, _veh.orient_heading);

			//	if (distance>0 && distance <160)
			//	{
			//		cout << endl <<" *** WARNING  distance= " << distance << " ***" <<  endl << endl;
			//		sendDNPWMessage(distance, 5); // 5 is an app id of OpenDS 1
			//	}

				

	
			//}

			
		}
	}
}

void OpenDDSThread(int argc, char* argv[]){
	// start thread
	

	/*int argc;
	char* argv[]={ "-DCPSConfigFile","rtps.ini" };*/


	char key = ' ';

	DDS::DomainId_t DomainID{ 246 };

	const char * topic_traffic_vehs_name = "Mri_TrafficVeh";
	const char * topic_subject_car_name = "Mri_SubjectCar";
	
	try
	{
		std::thread threadTimestamp(TimestampThread);

		// Initialize DomainParticipantFactory
 		DDS::DomainParticipantFactory_var dpf =
			TheParticipantFactoryWithArgs(argc, argv);

		// Create DomainParticipant
		DDS::DomainParticipant_var participant =
			dpf->create_participant(DomainID,
				PARTICIPANT_QOS_DEFAULT,
				0,
				OpenDDS::DCPS::DEFAULT_STATUS_MASK);
		if (CORBA::is_nil(participant.in())) {
			cerr << "Participant: Failed to create participant..." << endl;
		}
		else {
			cout << "Participant: participant created successfully" << endl;
		}



		// Create Subscriber
		DDS::Subscriber_var subscriber = participant->create_subscriber(
			SUBSCRIBER_QOS_DEFAULT,
			DDS::SubscriberListener::_nil(),
			OpenDDS::DCPS::DEFAULT_STATUS_MASK);

		if (CORBA::is_nil(subscriber.in())) {
			cerr << "create_subscriber failed." << endl;
		}
		else {
			cout << "SubscriberClass: subscriber created correctly" << endl;
		}

		// Create Publisher
		DDS::Publisher_var publisher = participant->create_publisher(
			PUBLISHER_QOS_DEFAULT,
			DDS::PublisherListener::_nil(),
			::OpenDDS::DCPS::DEFAULT_STATUS_MASK
		);
		if (CORBA::is_nil(publisher.in())) {
			cerr << "create_publisher failed." << endl;
		}
		else {
			cout << "PublisherClass: publisher created correctly" << endl;
		}



		// run timestamp synchronization
		if (!TimeSynchronization(participant, subscriber, publisher))
		{
			//there was a problem with time synchronization

			cout << "####    THERE WAS A PROBLEM WITH TIMESTAMP SYNCHRONIZATION. PROGRAM WILL CLOSE...    ####" << endl;

			// Clean-up!
			participant->delete_contained_entities();
			dpf->delete_participant(participant);

			TheServiceParticipant->shutdown();

			threadTimestamp.detach();


			finish_application = true;
			Sleep(5000);

			exit(-1);



		}



		//Timestamps  synchronized !
		cout << "Timestamp synchronized. t=" << GetTimestamp() << endl << endl;


		cout << "To quit press 'q'" << endl << endl;


		//create reader to receive V2X message  Mri_V2XfromNS3  Mri_V2XtoNS3
		//DataReader_V2XMessage reader_v2xmessage(participant, subscriber, "Mri_V2XfromNS3");


		// writer
		DataWriter_V2XMessage writer_v2xMessage(participant, publisher, "Mri_V2XtoNS3");


		// topics:	Mri_TrafficVeh Mri_SubjectCar
		//WARNING: both readers uses the same "on_data_available" code in DataReaderListenerImpl_VehData
		//we can do this, because formats of the messages are the same
		DataReader_VehData reader_traffic_vehs(participant, subscriber, "Mri_TrafficVeh");
		
		// 8/3/2018 rd disabled datareader, subjectCar sends v2x BSM messages independently
		//DataReader_VehData reader_subject_car(participant, subscriber, "Mri_SubjectCar");


		//// to send dnpw message we need:
		//DataWriter_Aux2Strings sender(participant, publisher, "Mri_Control");
		

		while (key != 'q')
		{
			//Sleep(200);
			getInput(&key);

			

		}
		//close thread
		finish_application = true; //it breaks waiting "while .. " and close app 
		threadTimestamp.detach();
		
		Sleep(2500);

		// Clean-up!
		participant->delete_contained_entities();
		dpf->delete_participant(participant);

		TheServiceParticipant->shutdown();
		
		
		
	}
	catch (const CORBA::Exception& e) {
		e._tao_print_exception("Exception caught in OpenDDSThread:");
		
	}




}