
#pragma once




#include "MriTypeSupportImpl.h"
#include <dds/DCPS/Service_Participant.h>
#include <string>

class DataReader_Aux2Strings
{
public:
	

	DataReader_Aux2Strings(DDS::DomainParticipant_var m_participant, DDS::Subscriber_var subscriber, const char * topic_name);

	~DataReader_Aux2Strings();


	

private:

	DDS::Topic_var createTopic(const char * topic_name);
	DDS::DataReader_var createDataReader(DDS::Subscriber_var subscriber, DDS::Topic_var topic, DDS::DataReaderListener_var listener);
	
	

	//Common
	DDS::DomainParticipant_var participant;
	DDS::Topic_var topic;
	DDS::Subscriber_var subscriber;
	DDS::DataReader_var reader;

};