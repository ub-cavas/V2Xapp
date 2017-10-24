#include "ParticipantClass.h"
#include <dds/DCPS/Marked_Default_Qos.h>
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;
using std::string;


DDS::DomainId_t DomainID{ 246 };




ParticipantClass::ParticipantClass(int argc, char* argv[]) {
	createParticipant(argc, argv);
	
}

ParticipantClass::~ParticipantClass() {
	try {
		if (!CORBA::is_nil(m_participant.in())) {
			m_participant->delete_contained_entities();
		}
		if (!CORBA::is_nil(m_participantFactory.in())) {
			m_participantFactory->delete_participant(m_participant.in());
		}
	}
	catch (CORBA::Exception& e) {
		cerr << "Exception caught in cleanup." << endl << e << endl;
	}
	TheServiceParticipant->shutdown();
}




void ParticipantClass::createParticipant(int argc, char* argv[]) {
	auto m_participantFactory = TheParticipantFactoryWithArgs(argc, argv);

	m_participant = m_participantFactory->create_participant(
		DomainID,
		PARTICIPANT_QOS_DEFAULT,
		DDS::DomainParticipantListener::_nil(),
		::OpenDDS::DCPS::DEFAULT_STATUS_MASK);

	if (CORBA::is_nil(m_participant.in())) {
		cerr << "Participant: Failed to create participant..." << endl;
	}
	else {
		cout << "Participant: participant created successfully" << endl;
	}
}





