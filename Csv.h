#pragma once
#include <string>
#include "TimeSync.h"

#include "DataReader_VehData.h"


#define BUFFER_SIZE 1000


std::string csvConvertVehDataToString(Mri::VehData _veh);

void csvWriteFile(int indx_buff_to_save, long number_of_samples);





