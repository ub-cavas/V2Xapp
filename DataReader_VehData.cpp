#include "DataReader_VehData.h"
#include "DataReaderListenerImpl_VehData.h"
#include <dds/DCPS/Marked_Default_Qos.h>




DataReader_VehData::DataReader_VehData(DDS::DomainParticipant_var m_participant, DDS::Subscriber_var subscriber, const char * topic_name)
{
	this->participant = m_participant;
	this->subscriber = subscriber;
	this->topic = createTopic(topic_name);

	// Create Listener
	DataReaderListenerImpl_VehData* listener_impl_VehData = new DataReaderListenerImpl_VehData;
	DDS::DataReaderListener_var m_listener(listener_impl_VehData);
	this->reader = createDataReader(subscriber, topic, m_listener);
}


DataReader_VehData::~DataReader_VehData()
{
}

DDS::Topic_var
DataReader_VehData::createTopic(const char * topic_name)
{
	// Register TypeSupport 
	Mri::VehDataTypeSupport_var  ts =
		new Mri::VehDataTypeSupportImpl;

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
DataReader_VehData::createDataReader(
	DDS::Subscriber_var subscriber,
	DDS::Topic_var topic,
	DDS::DataReaderListener_var listener)
{

	DDS::DataReaderQos dr_qos;
	subscriber->get_default_datareader_qos(dr_qos);
	dr_qos.history.kind = DDS::KEEP_LAST_HISTORY_QOS;
	dr_qos.reliability.kind = DDS::BEST_EFFORT_RELIABILITY_QOS;
	dr_qos.reliability.max_blocking_time.sec = 0;
	dr_qos.reliability.max_blocking_time.nanosec = 2000;
	dr_qos.resource_limits.max_samples_per_instance = DDS::LENGTH_UNLIMITED;


	// Create DataReader
	DDS::DataReader_var reader =
		subscriber->create_datareader(topic,
			dr_qos,
			listener,
			OpenDDS::DCPS::DEFAULT_STATUS_MASK);

	// Check for failure
	if (!reader) {
		throw std::string("failed to create data reader");
	}

	return reader;
}
