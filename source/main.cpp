#include "Observation.h"

#include "Converter.h"
#include "DataReader.h"


int main() {
	//I love Oumuamua!!!
    Converter converter;
    DataReader data_reader;
    data_reader.read_observations();
    data_reader.read_observatory_data();
	return 0;
}