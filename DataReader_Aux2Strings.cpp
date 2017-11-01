#include "DataReader_Aux2Strings.h"
#include "DataReaderListenerImpl_Aux2Strings.h"
#include <dds/DCPS/Marked_Default_Qos.h>




DataReader_Aux2Strings::DataReader_Aux2Strings(DDS::DomainParticipant_var m_participant, DDS::Subscriber_var subscriber, const char * topic_name)
{
	this->participant = m_participant;
	this->subscriber = subscriber;
	this->topic = createTopic(topic_name);

	// Create Listener
	DataReaderListenerImpl_Aux2Strings* listener_impl = new DataReaderListenerImpl_Aux2Strings;
	DDS::DataReaderListener_var m_listener(listener_impl);
	this->reader = createDataReader(subscriber, topic, m_listener);
}


DataReader_Aux2Strings::~DataReader_Aux2Strings()
{
}

DDS::Topic_var
DataReader_Aux2Strings::createTopic(const char * topic_name)
{
	// Register TypeSupport 
	Mri::Aux2StringsTypeSupport_var  ts =
		new Mri::Aux2StringsTypeSupportImpl;

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



DDS::DataReader_var
DataReader_Aux2Strings::createDataReader(
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
