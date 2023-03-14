#include "Observation.h"

#include "Converter.h"
#include "DataReader.h"

#include "./measure/CartesianCoord.h"
#include "./measure/GeocentricCoord.h"

int main() {
	//I love Oumuamua!!!
    Converter converter;
    DataReader data_reader;
    /*
        Read the data
    */
    data_reader.read_observations();
    data_reader.read_observatory_data();
    data_reader.read_hubble_data();

    data_reader.read_interpolation_time_data();
    converter.interpolation_date_to_tt_tdb(data_reader.get_observations(), data_reader.get_interpolation_time());

    //CartesianCoord coord;
    //coord.set_x(5);
    //coord.set_y(3);
    //coord.set_z(5);
    //Date date("2017 14 10.0");
    //date.set_JD();
    //date.set_TT(1);
    //auto ans = converter.cartesian_to_geocentric(coord, date);
    //std::cout << '\n';
    //std::cout << ans.get_x() << ' ' << ans.get_y() << ' ' << ans.get_z();
	return 0;
}