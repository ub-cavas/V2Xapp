#include <thread>

#include <iostream> //cout

#include <dds/DCPS/Service_Participant.h>	//neccessary to start OpenDDSThreat without error
#include "OpenDDS.h"	//to start OpenDDSThread
#include "MriTypeSupportImpl.h"	//Mri data formats

#include "Sleep.h"	// Sleep()
#include "TimeSync.h"	//Sleep() 


extern bool finish_application;
extern std::map<long, Mri::VehData> vehs_map;
extern long veh_id_to_remove;


//char *argv2[] = { "-DCPSConfigFile","rtps.ini" };
//-DCPSConfigFile rtps.ini

int main(int argc, char* argv[]) {

	

	//startOpenDDSThread(argc, argv);

	std::thread threadOpenDDS(OpenDDSThread, argc, argv);
	std::thread threadVehsMap(vehsMapThread);
	//close thread

	

	//// start thread OpenDDS
	//std::thread threadOpenDDS(OpenDDSThread, argc, argv);
	
	long old_veh_timestamp = 0;

	std::map<long, Mri::VehData> vehs_map_copy;
	
	while (!finish_application)
	{
		//wait, wait....
		Sleep(100);
		old_veh_timestamp = GetTimestamp() - 50;	//to find veh data not updated for 50 x 10ms = 500 ms
		if (vehs_map.size()>0)
		{
			vehs_map_copy = vehs_map;

			std::cout << "------------------------------------------------"<< std::endl;
			for (auto& x : vehs_map_copy) {
				if (x.second.timestamp< old_veh_timestamp)
				{
					//select this veh_id to removing
					veh_id_to_remove = x.second.vehicle_id;

				}
				std::cout << "timestamp=" << x.second.timestamp << " veh_id=" << x.second.vehicle_id << " x=" << x.second.position_x << " y=" << x.second.position_y << std::endl;
			}
			std::cout << std::endl << std::endl;
		}
		
		
	}

	threadOpenDDS.detach();
	threadVehsMap.detach();

	return 0;
}
