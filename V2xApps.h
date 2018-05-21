#pragma once

#include <string> 

#include "MriTypeSupportImpl.h"

struct Point2D
{
	double x;
	double y;
};

std::string createBSMcoreData(Mri::VehData veh);

Mri::VehData readVehDatafromString(std::string message);

Point2D RotatePoint(Point2D point, double angle);

float sign(Point2D p1, Point2D p2, Point2D p3);

bool PointInTriangle(Point2D pt, Point2D v1, Point2D v2, Point2D v3);

float doNotPassWarning(double h_x, double h_y, double h_h, double t_x, double t_y, double t_h);

void sendDNPWMessage(float distance_meters, long receiverAppId);
