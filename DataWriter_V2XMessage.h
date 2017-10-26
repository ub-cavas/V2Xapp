#pragma once




#include "MriTypeSupportImpl.h"
#include <dds/DCPS/Service_Participant.h>
#include <string>

class DataWriter_V2XMessage
{
public:
	

	DataWriter_V2XMessage(DDS::DomainParticipant_var participant, DDS::Publisher_var publisher, const char * topic_name);

	~DataWriter_V2XMessage();


	void waitForSubscriber();
	void sendMessage(const Mri::V2XMessage & message);

	Mri::V2XMessageDataWriter_var msg_writer;


private:

	

	
	DDS::Topic_var createTopic(const char * topic_name);
	DDS::DataWriter_var createDataWriter();
	
	

	//Common
	DDS::DomainParticipant_var participant;
	DDS::Topic_var topic;
	DDS::Publisher_var publisher;
	DDS::DataWriter_var writer;


};