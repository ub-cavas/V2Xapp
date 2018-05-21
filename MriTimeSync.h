#pragma once


#include <string>
#include "DataReader_Aux2Strings.h"



void TimestampThread();

long GenerateAPP_ID();

bool TimeSynchronization(DDS::DomainParticipant_var m_participant, DDS::Subscriber_var m_subscriber, DDS::Publisher_var m_publisher);

void SetTimestamp(long perfectTimestamp);

long GetTimestamp();

bool ParseAux2Strings(Mri::Aux2Strings aux_message);

long MedianPackageDelay();

long MedianOffsetServerApp();

bool SendSyncMessage();




const std::string TAG_TIME_SYNC = "timesync";
const long SERVER_ID = 0;

extern Mri::Aux2StringsDataWriter_var  writer_global_aux2strings;
extern Mri::V2XMessageDataWriter_var  writer_global_v2xmessage;
extern Mri::VehDataDataWriter_var  writer_global_vehdata;



