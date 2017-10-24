#pragma once

#include <dds/DCPS/Service_Participant.h>


class SubscriberClass
{
public:
	
	SubscriberClass(DDS::DomainParticipant_var participant);
	~SubscriberClass();

	DDS::Subscriber_var m_subscriber;

private:

	DDS::DomainParticipant_var m_participant;

	void createSubscriber();
};

