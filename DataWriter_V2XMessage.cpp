#include "DataWriter_V2XMessage.h"
#include <dds/DCPS/Marked_Default_Qos.h>

#include <dds/DCPS/WaitSet.h>


#include <iostream>
#include "TimeSync.h"


using std::cerr;
using std::cout;
using std::endl;
using std::string;


//topic names: Mri_V2XfromNS3  Mri_V2XtoNS3

DataWriter_V2XMessage::DataWriter_V2XMessage(DDS::DomainParticipant_var participant, DDS::Publisher_var publisher, const char * topic_name)
{
	this->participant = participant;
	this->publisher = publisher;
	this->topic = createTopic(topic_name);
	this->writer = createDataWriter();
	this->msg_writer = Mri::V2XMessageDataWriter::_narrow(writer.in());  
	writer_global_v2xmessage = msg_writer;
	//waitForSubscriber();
	
}


DataWriter_V2XMessage::~DataWriter_V2XMessage()
{
}


void DataWriter_V2XMessage::waitForSubscriber() {
	// Block until Subscriber is available
	DDS::StatusCondition_var condition = writer->get_statuscondition();
	condition->set_enabled_statuses(DDS::PUBLICATION_MATCHED_STATUS);

	DDS::WaitSet_var ws = new DDS::WaitSet;
	ws->attach_condition(condition);

	while (true) {
		DDS::PublicationMatchedStatus matches;
		if (writer->get_publication_matched_status(matches) != ::DDS::RETCODE_OK) {
			/*ACE_ERROR((LM_ERROR,
				ACE_TEXT("ERROR: %N:%l: main() -")
				ACE_TEXT(" get_publication_matched_status failed!\n")),
				-1);*/
			throw std::string("ERROR: DataWriter V2XMessage get publication matched status failed");
		}

		if (matches.current_count >= 1) {
			break;
		}

		DDS::ConditionSeq conditions;
		DDS::Duration_t timeout = { 60, 0 };
		if (ws->wait(conditions, timeout) != DDS::RETCODE_OK) {
			/*ACE_ERROR((LM_ERROR,
				ACE_TEXT("ERROR: %N:%l: main() -")
				ACE_TEXT(" wait failed!\n")),
				-1);*/
			throw std::string("ERROR: DataWriter V2XMessage waitForSubscriber");
		}
	}

	ws->detach_condition(condition);
}






void DataWriter_V2XMessage::sendMessage(const Mri::V2XMessage& message) {
	
	int success = msg_writer->write(message, DDS::HANDLE_NIL);
	if (success != DDS::RETCODE_OK) {
		//ACE_ERROR((LM_ERROR, ACE_TEXT("(%P|%t) ERROR: Publisher::sendMessage write returned %d.\n"), success));
		throw std::string("ERROR: DataWriter V2XMessage sendMessage");
	}
}



DDS::Topic_var
DataWriter_V2XMessage::createTopic(const char * topic_name)
{
	// Register TypeSupport (Messenger::Message)
	Mri::V2XMessageTypeSupport_var  ts =
		new Mri::V2XMessageTypeSupportImpl;

	if (ts->register_type(participant, "") != DDS::RETCODE_OK) {
		throw std::string("failed to register type support");
	}

	// Create Topic (Mri_Control)
	CORBA::String_var type_name = ts->get_type_name();
	DDS::Topic_var topic =
		participant->create_topic(topic_name,
			type_name,
			TOPIC_QOS_DEFAULT,
			DDS::TopicListener::_nil(),
			OpenDDS::DCPS::DEFAULT_STATUS_MASK);

	// Check for failure
	if (!topic) {
		throw std::string("failed to create topic");
	}
	return topic;
}


DDS::DataWriter_var
DataWriter_V2XMessage::createDataWriter()
{
	// Create DataWriter
	DDS::DataWriter_var writer =
		publisher->create_datawriter(topic,
			DATAWRITER_QOS_DEFAULT,
			DDS::DataWriterListener::_nil(),
			OpenDDS::DCPS::DEFAULT_STATUS_MASK);

	// Check for failure
	if (!writer) {
		throw std::string("failed to create data writer");
	}

	return writer;
}
