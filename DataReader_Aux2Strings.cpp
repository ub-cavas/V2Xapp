#include "DataReader_Aux2Strings.h"
#include "DataReaderListenerImpl.h"
#include <dds/DCPS/Marked_Default_Qos.h>




DataReader_Aux2Strings::DataReader_Aux2Strings(DDS::DomainParticipant_var m_participant, DDS::Subscriber_var subscriber)
{
	this->participant = m_participant;
	this->subscriber = subscriber;
	this->topic = createTopic();

	// Create Listener
	DataReaderListenerImpl* listener_impl = new DataReaderListenerImpl;
	DDS::DataReaderListener_var m_listener(listener_impl);
	this->reader = createDataReader(subscriber, topic, m_listener);
}


DataReader_Aux2Strings::~DataReader_Aux2Strings()
{
}

DDS::Topic_var
DataReader_Aux2Strings::createTopic()
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
		participant->create_topic("Mri_Control",
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
