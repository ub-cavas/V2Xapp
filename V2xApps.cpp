#include "V2xApps.h"

#include <sstream>      // std::stringstream, std::stringbuf
#include <iostream> //cout

#include "TimeSync.h"


using std::cerr;
using std::cout;
using std::endl;
using std::string;

#define PI 3.14159265359



string createBSMcoreData(Mri::VehData veh) {


	std::stringstream tekst;
	tekst << veh.orient_heading << ";" << veh.position_x << ";" << veh.position_y << ";" << veh.position_z << ";" << veh.speed << ";" << veh.vehicle_id;
	return tekst.str();


}


Mri::VehData readVehDatafromString(std::string message) {


		Mri::VehData _veh;
		std::stringstream  lineStream(message);
		std::string cell;

		std::getline(lineStream, cell, ';');
		_veh.orient_heading = std::atof(cell.c_str());
	
		std::getline(lineStream, cell, ';');
		_veh.position_x = std::atof(cell.c_str());

		std::getline(lineStream, cell, ';');
		_veh.position_y = std::atof(cell.c_str());

		std::getline(lineStream, cell, ';');
		_veh.position_z = std::atof(cell.c_str());

		std::getline(lineStream, cell, ';');
		_veh.speed = std::atof(cell.c_str());

		std::getline(lineStream, cell, ';');
		_veh.vehicle_id = std::atol(cell.c_str());

		return _veh;



}


Point2D RotatePoint(Point2D point, double angle)
{
	Point2D _point;
	_point.x = point.x * cos(angle) - point.y * sin(angle);
	_point.y = point.y * cos(angle) + point.x * sin(angle);

	return _point;
};


float sign(Point2D p1, Point2D p2, Point2D p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool PointInTriangle(Point2D pt, Point2D v1, Point2D v2, Point2D v3)
{
	bool b1, b2, b3;

	b1 = sign(pt, v1, v2) < 0.0f;
	b2 = sign(pt, v2, v3) < 0.0f;
	b3 = sign(pt, v3, v1) < 0.0f;

	return ((b1 == b2) && (b2 == b3));
}





//void doNotPassWarning(double h_h)
float doNotPassWarning(double h_x, double h_y, double h_h, double t_x, double t_y, double t_h)
{
	// h_  human controlled veh
	// t_  tested veh

	//no warning -> distance = -1
	float distance = -1;




	double alpha_right = h_h + (0.65 * PI);
	if (alpha_right >(PI))
		alpha_right = alpha_right - (2 * PI);  // e.g. x= 3.16 => x = 3.16 - 6.28 = -3.12

	double alpha_left = h_h - (0.65 * PI);

	if (alpha_left < -(PI))
		alpha_left = 4 * PI + alpha_left;  // e.g  x = - 3.18 => x = 6.28 - 3.18 = 3.10 

	Point2D a_oryg = { 0,11 };
	Point2D b_oryg = { 260,18 };
	Point2D c_oryg = { 260,-3 };
	Point2D d_oryg = { 0,-3 };


	Point2D a = RotatePoint(a_oryg, h_h);
	Point2D b = RotatePoint(b_oryg, h_h);
	Point2D c = RotatePoint(c_oryg, h_h);
	Point2D d = RotatePoint(d_oryg, h_h);

	Point2D x_veh_point;

	if ((t_h < alpha_left && t_h > alpha_right) || ((alpha_left<(-PI / 2) && alpha_right>(PI / 2)) && (t_h < alpha_left || t_h > alpha_right)))
	{
		a = { a.x + h_x, a.y + h_y };
		b = { b.x + h_x, b.y + h_y };
		c = { c.x + h_x, c.y + h_y };
		d = { d.x + h_x, d.y + h_y };

		x_veh_point = { t_x, t_y };

		if (PointInTriangle(x_veh_point, a, b, c) || PointInTriangle(x_veh_point, a, c, d))
		{
			//show warning and distance
			distance = sqrt(pow((t_x - h_x), 2.0) + pow((t_y - h_y), 2.0));

		}
		

	}

	return distance;

}


// DNPW - Do Not Pass Warning
void sendDNPWMessage(float distance_meters, long receiverAppId) {

	Mri::Aux2Strings auxMessage;
	//long distance_feets = distance_meters / 3.28084;

	std::string s =  std::to_string((long) distance_meters) ;

	auxMessage.receiverId = receiverAppId;
	auxMessage.senderId = THIS_APP_ID;
	auxMessage.tag = "dnpw";
	auxMessage.str1 = s.c_str();
	auxMessage.str2 = "";

	int success = writer_global_aux2strings->write(auxMessage, DDS::HANDLE_NIL);
	if (success != DDS::RETCODE_OK) {
		//ACE_ERROR((LM_ERROR, ACE_TEXT("(%P|%t) ERROR: TimeSync send message write returned %d.\n"), success));
		throw std::string("ERROR: sending DoNotPassWarning failed");
	}
	//sender.sendMessage(auxMessage);

	//std::cout << "Send timesync message at: " << GetTimestamp() << std::endl << std::endl;
	
}