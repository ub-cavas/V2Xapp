#pragma once




#include "MriTypeSupportImpl.h"
#include <dds/DCPS/Service_Participant.h>
#include <string>

class DataWriter_Aux2Strings
{
public:

	DataWriter_Aux2Strings(DDS::DomainParticipant_var participant, DDS::Publisher_var publisher, const char * topic_name);

	~DataWriter_Aux2Strings();


	void waitForSubscriber();
	void sendMessage(const Mri::Aux2Strings& message);

	Mri::Aux2StringsDataWriter_var msg_writer;

private:

	
	DDS::Topic_var createTopic(const char * topic_name);
	DDS::DataWriter_var createDataWriter();
	
	

	//Common
	DDS::DomainParticipant_var participant;
	DDS::Topic_var topic;
	DDS::Publisher_var publisher;
	DDS::DataWriter_var writer;


	//#######################

	// Safely downcast data writer to type-specific data writer
	//Messenger::MessageDataWriter_var msg_writer

		//TopicSample::MessageTypeSupport_var m_topicSupport;


	//DDS::Topic_var m_messageTopic;
	//TopicSample::MessageDataWriter_var m_dataWriter;
	//DDS::InstanceHandle_t m_topicHandler;
	int m_counter = 1;
};