
//rd timer
#include <chrono>
#include <thread>

#include <stdlib.h> //atol
#include <cmath>	//lround
#include <iostream> //cout


#include <string.h>	//strcomp


#include "TimeSync.h"

#include "DataWriter_Aux2Strings.h"



//this doesn't compile with timesync client

//long timestamp_master;
//
//long stepsCounter = 0;
//long delaySum = 0;







bool SendTimeSyncMessage(Mri::Aux2Strings auxMessage) {

	Mri::Aux2Strings m_Message;

	std::string s = std::to_string(GetTimestamp());
	/*char* p = new char[s.length() + 1];
	memcpy(p, s.c_str(), s.length() + 1);*/


	m_Message.receiverId =auxMessage.senderId;
	m_Message.senderId = THIS_APP_ID;
	m_Message.tag =TAG_TIME_SYNC.c_str();
	m_Message.str1 = auxMessage.str1;
	m_Message.str2 = s.c_str();

	int success = writer_global_aux2strings->write(m_Message, DDS::HANDLE_NIL);
	if (success != DDS::RETCODE_OK) {
		ACE_ERROR((LM_ERROR, ACE_TEXT("(%P|%t) ERROR: TimeSync send message write returned %d.\n"), success));
	}
	
	//sender_clrd.sendMessage(m_Message);
	return true;
}



bool ParseAux2StringsServer(Mri::Aux2Strings aux_message)
{

	if (aux_message.receiverId!=THIS_APP_ID)
	{
		return true;
	}


	if (strcmp(aux_message.tag, TAG_TIME_SYNC.c_str()) == 0 )
	{// time synchronization message

		SendTimeSyncMessage(aux_message);

	}

	return true;
}
