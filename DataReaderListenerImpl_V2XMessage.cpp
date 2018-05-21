/*
 *
 *
 * Distributed under the OpenDDS License.
 * See: http://www.opendds.org/license.html
 */

#include <ace/Log_Msg.h>
#include <ace/OS_NS_stdlib.h>

#include "DataReaderListenerImpl_V2XMessage.h"
#include "MriTypeSupportC.h"
#include "MriTypeSupportImpl.h"

#include <iostream>

#include"TimeSync.h"	//Parse Aux message
#include "QueueTs.h"




using std::cerr;
using std::cout;
using std::endl;
using std::string;

extern QueueTs<Mri::V2XMessage> v2x_queue;


void
DataReaderListenerImpl_V2XMessage::on_data_available(DDS::DataReader_ptr reader)
{

	Mri::V2XMessageDataReader_var reader_i =
		Mri::V2XMessageDataReader::_narrow(reader);

	if (!reader_i) {
		ACE_ERROR((LM_ERROR,
			ACE_TEXT("ERROR: %N:%l: on_data_available() -")
			ACE_TEXT(" _narrow failed!\n")));
		ACE_OS::exit(-1);
	}

	Mri::V2XMessage v2x_message;
	DDS::SampleInfo info;

	DDS::ReturnCode_t error = reader_i->take_next_sample(v2x_message, info);

	if (error == DDS::RETCODE_OK) {
	/*	cout << "SampleInfo.sample_rank = " << info.sample_rank << endl;
		cout << "SampleInfo.instance_state = " << info.instance_state << endl;*/

		if (info.valid_data) {

			//cout << "......................V2X: senderId  = " << v2x_message.message << endl;
		/*		<< "     receiverId = " << v2x_message.recipient_id << endl
				<< "sender_timestamp=" << v2x_message.sender_timestamp << endl;*/
			
			v2x_queue.push(v2x_message);
		}

	}
	else {
		ACE_ERROR((LM_ERROR,
			ACE_TEXT("ERROR: %N:%l: on_data_available() -")
			ACE_TEXT(" take_next_sample failed!\n")));
	}
}




























void
DataReaderListenerImpl_V2XMessage::on_requested_deadline_missed(
  DDS::DataReader_ptr /*reader*/,
  const DDS::RequestedDeadlineMissedStatus& /*status*/)
{

	cout << "on_requested_deadline_missed" << endl;

}

void
DataReaderListenerImpl_V2XMessage::on_requested_incompatible_qos(
  DDS::DataReader_ptr /*reader*/,
  const DDS::RequestedIncompatibleQosStatus& /*status*/)
{
	cout << "on_requested_incompatible_qos" << endl;
}

void
DataReaderListenerImpl_V2XMessage::on_sample_rejected(
  DDS::DataReader_ptr /*reader*/,
  const DDS::SampleRejectedStatus& /*status*/)
{
	cout << "on_sample_rejected" << endl;
}

void
DataReaderListenerImpl_V2XMessage::on_liveliness_changed(
  DDS::DataReader_ptr /*reader*/,
  const DDS::LivelinessChangedStatus& /*status*/)
{
	cout << "on_liveliness_Changed" << endl;
}



void
DataReaderListenerImpl_V2XMessage::on_subscription_matched(
  DDS::DataReader_ptr /*reader*/,
  const DDS::SubscriptionMatchedStatus& /*status*/)
{
	cout << "V2X on_subscription_matched" << endl;
}

void
DataReaderListenerImpl_V2XMessage::on_sample_lost(
  DDS::DataReader_ptr /*reader*/,
  const DDS::SampleLostStatus& /*status*/)
{
	cout << "on_samlpe lost" << endl;
}
