#include "SubscriberClass.h"
#include <dds/DCPS/Marked_Default_Qos.h>
#include <iostream>



using std::cerr;
using std::cout;
using std::endl;
using std::string;



SubscriberClass::SubscriberClass(DDS::DomainParticipant_var participant)
{
	this->m_participant = participant;

	createSubscriber();

}


SubscriberClass::~SubscriberClass()
{
}




void SubscriberClass::createSubscriber()
{
	// Create Subscriber
	m_subscriber = m_participant->create_subscriber(
		SUBSCRIBER_QOS_DEFAULT,
		DDS::SubscriberListener::_nil(),
		OpenDDS::DCPS::DEFAULT_STATUS_MASK);

	if (CORBA::is_nil(m_subscriber.in())) {
		cerr << "create_subscriber failed." << endl;
	}
	else {
		cout << "SubscriberClass: subscriber created correctly" << endl;
	}
}