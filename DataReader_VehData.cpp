#include "DataReader_V2XMessage.h"
#include "DataReaderListenerImpl_V2XMessage.h"
#include <dds/DCPS/Marked_Default_Qos.h>




DataReader_V2XMessage::DataReader_V2XMessage(DDS::DomainParticipant_var m_participant, DDS::Subscriber_var subscriber, const char * topic_name)
{
	this->participant = m_participant;
	this->subscriber = subscriber;
	this->topic = createTopic(topic_name);

	// Create Listener
	DataReaderListenerImpl_V2XMessage* listener_impl_V2XMessage = new DataReaderListenerImpl_V2XMessage;
	DDS::DataReaderListener_var m_listener(listener_impl_V2XMessage);
	this->reader = createDataReader(subscriber, topic, m_listener);
}


DataReader_V2XMessage::~DataReader_V2XMessage()
{
}

DDS::Topic_var
DataReader_V2XMessage::createTopic(const char * topic_name)
{
	// Register TypeSupport 
	Mri::V2XMessageTypeSupport_var  ts =
		new Mri::V2XMessageTypeSupportImpl;

	if (ts->register_type(participant, "") != DDS::RETCODE_OK) {
		throw std::string("failed to register type support");
	}

	// Create Topic (Mri_Control)
	CORBA::String_var type_name = ts->get_type_name();
	DDS::Topic_var topic =
		participant->create_topic(topic_name,		//Mri_V2XtoNS3
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



DDS::DataReader_var
DataReader_V2XMessage::createDataReader(
	DDS::Subscriber_var subscriber,
	DDS::Topic_var topic,
	DDS::DataReaderListener_var listener)
{
	// Create DataReader
	DDS::DataReader_var reader =
		subscriber->create_datareader(topic,
			DATAREADER_QOS_DEFAULT,
			listener,
			OpenDDS::DCPS::DEFAULT_STATUS_MASK);

	// Check for failure
	if (!reader) {
		throw std::string("failed to create data reader");
	}

	return reader;
}
