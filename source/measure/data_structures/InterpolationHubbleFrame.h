#pragma once
#include "../coords/GeocentricCoord.h"
#include "../coords/BarycentricCoord.h"

#include "../time/Date.h"

// ласс дл€ хранени€ данных интерпол€ции ’аббла
class InterpolationHubbleFrame {
private:
    GeocentricCoord position_data;
    Date date;
    BarycentricCoord barycentric_frame;
public:
    InterpolationHubbleFrame() = default;

    void set_date(Date);
    void set_geocentric(GeocentricCoord);
    void set_barycentric(BarycentricCoord);

    Date get_date();
    GeocentricCoord get_geocentric();
    BarycentricCoord get_barycentric();
};