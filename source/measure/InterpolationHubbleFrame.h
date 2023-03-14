#pragma once
#include "Date.h"
#include "CartesianCoord.h"


class InterpolationHubbleFrame {
private:
    CartesianCoord position_data;
    Date date;
public:
    void set_date(Date);
    void set_data(CartesianCoord);

    Date get_date();
    CartesianCoord get_data();
};