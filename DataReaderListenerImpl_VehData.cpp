/*
 *
 *
 * Distributed under the OpenDDS License.
 * See: http://www.opendds.org/license.html
 */

#include <ace/Log_Msg.h>
#include <ace/OS_NS_stdlib.h>

#include "DataReaderListenerImpl_VehData.h"
#include "MriTypeSupportC.h"
#include "MriTypeSupportImpl.h"

#include <iostream>

#include"TimeSync.h"	//Parse Aux message




using std::cerr;
using std::cout;
using std::endl;
using std::string;




void
DataReaderListenerImpl_VehData::on_data_available(DDS::DataReader_ptr reader)
{

	Mri::VehDataDataReader_var reader_i =
		Mri::VehDataDataReader::_narrow(reader);

	if (!reader_i) {
		ACE_ERROR((LM_ERROR,
			ACE_TEXT("ERROR: %N:%l: on_data_available() -")
			ACE_TEXT(" _narrow failed!\n")));
		ACE_OS::exit(-1);
	}

	Mri::VehData veh_message;
	DDS::SampleInfo info;

	DDS::ReturnCode_t error = reader_i->take_next_sample(veh_message, info);

	if (error == DDS::RETCODE_OK) {
		cout << "SampleInfo.sample_rank = " << info.sample_rank << endl;
		cout << "SampleInfo.instance_state = " << info.instance_state << endl;

		if (info.valid_data) {
			//ParseAux2StringsServer(aux_message);

			cout << "Veh vehicle_id  = " << veh_message.vehicle_id << endl
				<< "     position_x  = " << veh_message.position_x << endl
				<< "      timestamp  =" << veh_message.timestamp << endl;
				//<< "         str1       = " << aux_message.str1 << std::endl
				//<< "         str2       = " << aux_message.str2 << std::endl
				//<< "         tag        = " << aux_message.tag << std::endl;

		}

	}
	else {
		ACE_ERROR((LM_ERROR,
			ACE_TEXT("ERROR: %N:%l: on_data_available() -")
			ACE_TEXT(" take_next_sample failed!\n")));
	}
}




























void
DataReaderListenerImpl_VehData::on_requested_deadline_missed(
  DDS::DataReader_ptr /*reader*/,
  const DDS::RequestedDeadlineMissedStatus& /*status*/)
{
}

void
DataReaderListenerImpl_VehData::on_requested_incompatible_qos(
  DDS::DataReader_ptr /*reader*/,
  const DDS::RequestedIncompatibleQosStatus& /*status*/)
{
}

void
DataReaderListenerImpl_VehData::on_sample_rejected(
  DDS::DataReader_ptr /*reader*/,
  const DDS::SampleRejectedStatus& /*status*/)
{
}

void
DataReaderListenerImpl_VehData::on_liveliness_changed(
  DDS::DataReader_ptr /*reader*/,
  const DDS::LivelinessChangedStatus& /*status*/)
{
}



void
DataReaderListenerImpl_VehData::on_subscription_matched(
  DDS::DataReader_ptr /*reader*/,
  const DDS::SubscriptionMatchedStatus& /*status*/)
{
}

void
DataReaderListenerImpl_VehData::on_sample_lost(
  DDS::DataReader_ptr /*reader*/,
  const DDS::SampleLostStatus& /*status*/)
{
}
