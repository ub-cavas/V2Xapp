#pragma once
#include <string>
#include "DataReader_Aux2Strings.h"

void TimestampThread();

void AddNextDelay(Mri::Aux2Strings auxMessage, long timestampNow);




long GetTimestamp();

void SetTimestamp(long timestamp_perfect);

bool SendSyncMessage();


bool SendTimeSyncMessage(Mri::Aux2Strings auxMessage); //timesync server

bool ParseAux2StringsServer(Mri::Aux2Strings aux_message);


bool SynchronizeTime();
bool ParseAux2Strings(Mri::Aux2Strings aux_message);

void TimeSynchronization(DDS::DomainParticipant_var m_participant, DDS::Subscriber_var m_subscriber, DDS::Publisher_var m_publisher);


const long THIS_APP_ID = 10;
const long SERVER_ID = 1;
//const char * MRI_TAG_TIME_SYNC = "timesync";

extern Mri::Aux2StringsDataWriter_var  writer_global_aux2strings;
extern Mri::V2XMessageDataWriter_var  writer_global_v2xmessage;


const std::string TAG_TIME_SYNC = "timesync";
