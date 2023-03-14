#include <string>
#include <map>

#include "./measure/CartesianCoord.h"
#include "./measure/Date.h"

class HubbleInterpolator {
private:
    std::map<Date, CartesianCoord> interpolation_data;
public:
    CartesianCoord interpolate(Date date);
    void set_data_frame(Date, CartesianCoord);
};