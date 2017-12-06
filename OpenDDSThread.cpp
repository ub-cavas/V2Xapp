#include "MriTypeSupportImpl.h"
#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/Service_Participant.h>

#include "TimeSync.h"
#include <string> 
#include <thread>
#include <iostream> //cout
#include "OpenDDSThread.h"
#include "DataWriter_Aux2Strings.h"
#include "DataReader_Aux2Strings.h"
#include "DataReader_V2XMessage.h"
#include "DataWriter_V2XMessage.h"
#include "DataReader_VehData.h"
//#include <boost/lockfree/queue.hpp>	
#include "QueueTs.h"

#include <ctime> //time format
#include <conio.h> //key press

using std::cerr;
using std::cout;
using std::endl;
using std::string;


Mri::Aux2StringsDataWriter_var  writer_global_aux2strings;
Mri::V2XMessageDataWriter_var  writer_global_v2xmessage;

//boost::lockfree::queue<Mri::VehData> vehdata_queue{ 1000 };		//size = 1000

QueueTs<Mri::VehData> vehdata_queue;



extern bool finish_application;

bool getInput(char *c)
{
	if (_kbhit())
	{
		*c = _getch();
		return true; // Key Was Hit
	}
	return false; // No keys were pressed
}
//void OpenDDSThread(int argc, char* argv[])
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
		//std::cout << "*****************   Timestamp: " << GetTimestamp() << std::endl << std::endl;
		
		



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
		TimeSynchronization(participant, subscriber, publisher);


		//create reader to receive V2X message  Mri_V2XfromNS3  Mri_V2XtoNS3
		DataReader_V2XMessage reader_v2xmessage(participant, subscriber, "Mri_V2XfromNS3");
		// writer
		DataWriter_V2XMessage writer_v2xMessage(participant, publisher, "Mri_V2XtoNS3");


		// topics:	Mri_TrafficVeh Mri_SubjectCar
		//WARNING: both readers uses the same "on_data_available" code in DataReaderListenerImpl_VehData
		//we can do this, because formats of the messages are the same
		DataReader_VehData reader_traffic_vehs(participant, subscriber, "Mri_TrafficVeh");
		DataReader_VehData reader_subject_car(participant, subscriber, "Mri_SubjectCar");


		
		////VehData DataReaders

		//// Register TypeSupport 
		//Mri::VehDataTypeSupport_var  ts =
		//	new Mri::VehDataTypeSupportImpl;

		//if (ts->register_type(participant, "") != DDS::RETCODE_OK) {
		//	throw std::string("Failed to register VehData type support");
		//}


		//// Reader Mri_TrafficVeh

		//// Create Topic Mri_TrafficVeh
		//CORBA::String_var type_name = ts->get_type_name();
		//DDS::Topic_var topic_traffic_vehs =
		//	participant->create_topic(topic_traffic_vehs_name,
		//		type_name,
		//		TOPIC_QOS_DEFAULT,
		//		DDS::TopicListener::_nil(),
		//		OpenDDS::DCPS::DEFAULT_STATUS_MASK);

		//// Check for failure
		//if (!topic_traffic_vehs) {
		//	throw std::string("Failed to create topic Mri_TrafficVeh");
		//}


		//// Create DataReader
		//DDS::DataReaderListener_var listener(new DataReaderListenerImpl_VehData);


		//DDS::DataReaderQos dr_qos;
		//subscriber->get_default_datareader_qos(dr_qos);
		//dr_qos.history.kind = DDS::KEEP_ALL_HISTORY_QOS;
		//dr_qos.reliability.kind = DDS::BEST_EFFORT_RELIABILITY_QOS;
		//dr_qos.reliability.max_blocking_time.sec = 0;
		//dr_qos.reliability.max_blocking_time.nanosec = 2000;
		//dr_qos.resource_limits.max_samples_per_instance = DDS::LENGTH_UNLIMITED;


		//DDS::DataReader_var reader_traffic_vehs =
		//	subscriber->create_datareader(topic_traffic_vehs,
		//		DATAREADER_QOS_DEFAULT,
		//		listener,
		//		OpenDDS::DCPS::DEFAULT_STATUS_MASK);

		//// Check for failure
		//if (!reader_traffic_vehs) {
		//	throw std::string("failed to create data reader traffic_vehs");
		//}





		//// Create Topic "Mri_SubjectCar"
		////CORBA::String_var type_name = ts->get_type_name();
		//DDS::Topic_var topic_subject_car =
		//	participant->create_topic(topic_subject_car_name,
		//		type_name,	//this is the same type like in topic_traffic_vehs
		//		TOPIC_QOS_DEFAULT,
		//		DDS::TopicListener::_nil(),
		//		OpenDDS::DCPS::DEFAULT_STATUS_MASK);

		//// Check for failure
		//if (!topic_subject_car) {
		//	throw std::string("Failed to create topic Mri_TrafficVeh");
		//}

		//DDS::DataReader_var reader_subject_car =
		//	subscriber->create_datareader(topic_subject_car,
		//		DATAREADER_QOS_DEFAULT,
		//		listener,
		//		OpenDDS::DCPS::DEFAULT_STATUS_MASK);

		//// Check for failure
		//if (!reader_subject_car) {
		//	throw std::string("failed to create data reader subject car");
		//}


		Mri::VehData _veh;

		while (key != 'q')
		{
			//Sleep(200);
			getInput(&key);

			vehdata_queue.pop(_veh);
			{
				cout << "Timestamp: " << _veh.timestamp << " id=" << _veh.vehicle_id << endl;
			}

		}


		//close thread

		

		// Clean-up!
		participant->delete_contained_entities();
		dpf->delete_participant(participant);

		TheServiceParticipant->shutdown();

		threadTimestamp.detach();
		finish_application = true; //it breaks waiting "while .. " and close app 
		
	}
	catch (const CORBA::Exception& e) {
		e._tao_print_exception("Exception caught in main():");
		
	}



	/*while (true)
	{
	Sleep(1000);
	}*/


}