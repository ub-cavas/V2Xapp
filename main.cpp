
#include <dds/DCPS/Service_Participant.h>	//neccessary to start OpenDDSThreat without error
#include "OpenDDSThread.h"	//to start OpenDDSThread


extern bool finish_application;



//char *argv2[] = { "-DCPSConfigFile","rtps.ini" };
//-DCPSConfigFile rtps.ini

int main(int argc, char* argv[]) {

	

	startOpenDDSThread(argc, argv);

	//// start thread OpenDDS
	//std::thread threadOpenDDS(OpenDDSThread, argc, argv);
	

	while (!finish_application)
	{
		//wait, wait....
	}

	

	return 0;
}
