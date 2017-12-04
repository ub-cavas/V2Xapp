#include "Csv.h"

#include <sstream>      // std::stringstream, std::stringbuf
#include <iostream>
#include <fstream>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::string;

//extern std::vector<string> buffor;
extern std::vector<std::vector<std::string>> buffer;
extern std::string fileName;
extern int file_index_to_save;

string csvConvertVehDataToString(Mri::VehData _veh) {
	string text_line;

	std::stringstream tekst;

	tekst   << _veh.timestamp << ',' << "Mri_TrafficVeh" << ',' << _veh.color <<','<< _veh.lane_index << ',' << _veh.leading_vehicle_id
			<< ',' << _veh.link_coordinate << ',' << _veh.link_id << ',' << _veh.link_name << ',' << _veh.model_file_name << ',' << _veh.orient_heading
			<< ',' << _veh.orient_pitch << ',' << _veh.orient_roll << ',' << _veh.position_x << ',' << _veh.position_y << ',' << _veh.position_z 
			<< ',' << _veh.speed  << ',' << _veh.trailing_vehicle_id << ',' << _veh.turning_indicator << ',' << _veh.vehicle_id << ',' << _veh.vehicle_type << ',' << GetTimestamp()<<'\n';

	// '\n'
	
	
	return tekst.str();
}



void csvWriteFile( int indx_buff_to_save, long number_of_samples) {
	
	

	std::stringstream buffer_file_name;

	ofstream csvFile;
	cout << endl<< "Start writing file at: " << GetTimestamp() << endl;


	//buffer_file_name << fileName << '_' << fileName_index << ".csv";
	buffer_file_name << fileName << '_' << file_index_to_save << ".csv";

	std::string s =fileName + "/" + buffer_file_name.str();
	const char* c_filename = s.c_str();

	//cout << c_filename << endl;




	/*for (int i = 0; i < BUFFER_SIZE; ++i)
		cout << (buffer[indx_buff_to_save][i]);
*/
	

	//std::string *p;
	//p = buffor.data();


	//csvFile.open(filename, ios::out | ios::ate | ios::app);
	//csvFile.open(filename, ios::out | ios::trunc);
	
	
	
	
	csvFile.open(c_filename, ios::out | ios::trunc);

	csvFile << "timestamp,topic,veh_color,lane_index,leading_veh_id,link_coordinate,link_id,link_name,veh_model_file_name,orient_heading,orient_pitch,orient_roll,position_x,position_y,position_z,speed,trailing_veh_id,turning_indicator,veh_id,veh_type,recorder_timestamp" << endl;
	cout << c_filename << " samples: " << number_of_samples << endl;
	
	for (int i = 0; i < number_of_samples; ++i) {
		csvFile << (buffer[indx_buff_to_save][i]);
		//(buffer[indx_buff_to_save][i]="");
	}
		

	
	
	//csvFile << *p++;
	csvFile.close();
	cout << "End of  writing file at: " << GetTimestamp() << endl;

}