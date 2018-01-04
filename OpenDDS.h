#pragma once
#include <string>

bool getInput(char * c);

void sendV2X(long sender_id, long sender_timestamp, std::string message);

void vehsMapThread();

void OpenDDSThread(int argc, char * argv[]);









