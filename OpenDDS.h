#pragma once

bool getInput(char * c);

void sendV2X(long sender_id, long sender_timestamp, string message);

void startOpenDDSThread(int argc, char * argv[]);

void vehsMapThread();

void OpenDDSThread(int argc, char * argv[]);









