#include "Observation.h"

#include "Converter.h"
#include "DataReader.h"


int main() {
	//I love Oumuamua!!!
    Converter converter;
    DataReader data_reader;
    data_reader.read_observations();
    data_reader.read_observatory_data();
    data_reader.read_hubble_data();

    //data_reader.read_interpolation_time_data();
    //converter.interpolation_date_to_tt_tdb(data_reader.get_observations(), data_reader.get_interpolation_time());
	return 0;
}