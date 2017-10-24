#pragma once



//#include "idl/TopicsTypeSupportImpl.h"
#include <dds/DCPS/Service_Participant.h>
//#include <string>

class PublisherClass {
public:

	
	PublisherClass(DDS::DomainParticipant_var participant);
	~PublisherClass();
	
	

	
	DDS::Publisher_var m_publisher;
	

private:
	DDS::DomainParticipant_var m_participant;
	 
	void createPublisher();
	

	

};

