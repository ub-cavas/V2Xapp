
#pragma once




#include "MriTypeSupportImpl.h"
#include <dds/DCPS/Service_Participant.h>
#include <string>

class DataReader_V2XMessage
{
public:
	

	DataReader_V2XMessage(DDS::DomainParticipant_var participant, DDS::Subscriber_var subscriber);

	~DataReader_V2XMessage();


	

private:

	DDS::Topic_var createTopic();
	DDS::DataReader_var createDataReader(DDS::Subscriber_var subscriber, DDS::Topic_var topic, DDS::DataReaderListener_var listener);
	
	

	//Common
	DDS::DomainParticipant_var participant;
	DDS::Topic_var topic;
	DDS::Subscriber_var subscriber;
	DDS::DataReader_var reader;

};