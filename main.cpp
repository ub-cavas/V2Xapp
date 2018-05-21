#include <thread>
#include <chrono>         // std::chrono::seconds to test this_thread sleep

#include <iostream> //cout
#include <stdlib.h>     /* srand, rand */

#include <dds/DCPS/Service_Participant.h>	//neccessary to start OpenDDSThreat without error
#include "OpenDDS.h"	//to start OpenDDSThread
#include "MriTypeSupportImpl.h"	//Mri data formats

#include "Sleep.h"	// Sleep()
#include "MriTimeSync.h"	//Sleep() 

#include "V2xApps.h"
#include "main.h"



extern bool finish_application;
extern std::map<long, Mri::VehData> vehs_map;
extern long veh_id_to_remove;


//char *argv2[] = { "-DCPSConfigFile","rtps.ini" };
//-DCPSConfigFile rtps.ini






int main(int argc, char* argv[]) {

	

	//startOpenDDSThread(argc, argv);

	std::thread threadOpenDDS(OpenDDSThread, argc, argv);
	std::thread threadVehsMap(vehsMapThread);
	std::thread threadV2Xreading(v2xMapThread);
	//close thread

	

	//// start thread OpenDDS
	//std::thread threadOpenDDS(OpenDDSThread, argc, argv);
	
	long old_veh_timestamp = 0;

	std::map<long, Mri::VehData> vehs_map_copy;
	

	/* initialize random seed: */
	srand(time(NULL));
	
	int wait_time_random;
	int map_size;
	int wait_time_factor;	// more vehs in the map => factor become smaller, time between messages shorter
	std::string message_text;

	while (!finish_application)
	{
		//wait, wait....
		//Sleep(100);

		


		

		map_size = vehs_map.size();

		if (map_size>0)
		{
			vehs_map_copy = vehs_map;
			if (map_size>=400)
			{
				map_size = 400;
			}
			wait_time_factor = 400 / map_size;


			//std::cout << "------------------------------------------------"<< std::endl;




			for (auto& x : vehs_map_copy) {
				

				//send V2X message in random intervals of time
				//sendV2X

				//message_text = "Hi! This is vehicle " + std::to_string(x.second.vehicle_id) + ". My location is: " + std::to_string(x.second.position_x) + ";" + std::to_string(x.second.position_y);
				message_text = createBSMcoreData(x.second);
				
				sendV2X(x.second.vehicle_id, GetTimestamp(), message_text);

				wait_time_random = rand() % wait_time_factor;
				Sleep(wait_time_random);

				//std::cout << "wait time " << wait_time_random << std::endl;

				//std::cout << "timestamp=" << x.second.timestamp << " veh_id=" << x.second.vehicle_id << " x=" << x.second.position_x << " y=" << x.second.position_y << std::endl;
			}
			

			old_veh_timestamp = GetTimestamp() - 50;	//to find veh data not updated for 50 x 10ms = 500 ms

			for (auto& x : vehs_map_copy) {
				if (x.second.timestamp < old_veh_timestamp)
				{
					if (x.second.vehicle_id!=0)
					{
						//select this veh_id to removing
						veh_id_to_remove = x.second.vehicle_id;
						break;
					}
					

				}
			}




			std::cout << ".";
			//std::cout << std::endl << std::endl;
		}
		
		
	}

	std::cout << std::endl << "---   CLOSING APPLICATION   ---" << std::endl;

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	threadV2Xreading.detach();
	threadVehsMap.detach();
	threadOpenDDS.detach();
	

	return 0;
}
