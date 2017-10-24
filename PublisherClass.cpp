
#include "PublisherClass.h"
#include <dds/DCPS/Marked_Default_Qos.h>
#include <iostream>



using std::cerr;
using std::cout;
using std::endl;
using std::string;

PublisherClass::PublisherClass(DDS::DomainParticipant_var participant)
{
	this->m_participant = participant;
	createPublisher();
}

PublisherClass::~PublisherClass()
{
}



void PublisherClass::createPublisher() {
	m_publisher = m_participant->create_publisher(
		PUBLISHER_QOS_DEFAULT,
		DDS::PublisherListener::_nil(),
		::OpenDDS::DCPS::DEFAULT_STATUS_MASK
	);
	if (CORBA::is_nil(m_publisher.in())) {
		cerr << "create_publisher failed." << endl;
	}
	else {
		cout << "PublisherClass: publisher created correctly" << endl;
	}

}
