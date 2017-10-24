#pragma once
//#include "idl/TopicsTypeSupportImpl.h"
#include <dds/DCPS/Service_Participant.h>
#include <string>

class ParticipantClass {
public:

	ParticipantClass(int argc, char* argv[]);
	~ParticipantClass();


	DDS::DomainParticipant_var m_participant;


private:

	void createParticipant(int argc, char* argv[]);


	DDS::DomainParticipantFactory_var m_participantFactory;



};