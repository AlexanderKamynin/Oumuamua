#pragma once
#include "Date.h"
#include "GeocentricCoord.h"
#include "BarycentricCoord.h"

// ласс дл€ хранени€ данных интерпол€ции ’аббла
class InterpolationHubbleFrame {
private:
    GeocentricCoord position_data;
    Date date;
    BarycentricCoord barycentric_frame;
public:
    void set_date(Date);
    void set_geocentric(GeocentricCoord);
    void set_barycentric(BarycentricCoord);

    Date get_date();
    GeocentricCoord get_geocentric();
    BarycentricCoord get_barycentric();
};