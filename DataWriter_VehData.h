#pragma once




#include "MriTypeSupportImpl.h"
#include <dds/DCPS/Service_Participant.h>
#include <string>

class DataWriter_VehData
{
public:
	

	DataWriter_VehData(DDS::DomainParticipant_var participant, DDS::Publisher_var publisher, const char * topic_name);

	//DataWriter_VehData();

	~DataWriter_VehData();


	void waitForSubscriber();
	void sendMessage(const Mri::VehData & message);

	Mri::VehDataDataWriter_var msg_writer;


private:

	

	
	DDS::Topic_var createTopic(const char * topic_name);
	DDS::DataWriter_var createDataWriter();
	
	

	//Common
	DDS::DomainParticipant_var participant;
	DDS::Topic_var topic;
	DDS::Publisher_var publisher;
	DDS::DataWriter_var writer;

	std::string topic_name;


};