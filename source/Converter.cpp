#include "Converter.h"


CartesianCoord Converter::cylindrical_to_cartesian(CylindricalCoord cylindrical_coord)
{
    CartesianCoord cartesian_coord;
    if (cylindrical_coord.get_longitude() != 0) {
        if (cylindrical_coord.get_cos() != 0) {
            cartesian_coord.set_x(cos(cylindrical_coord.get_longitude()) * cylindrical_coord.get_cos() * EARTH_RADIUS);
            cartesian_coord.set_y(sin(cylindrical_coord.get_longitude()) * cylindrical_coord.get_cos() * EARTH_RADIUS);
        }
        if (cylindrical_coord.get_sin() != 0)
            cartesian_coord.set_z(cylindrical_coord.get_sin() * EARTH_RADIUS);
    }
    return cartesian_coord;
}


//»нтерпол€ци€ времени в TDB
void Converter::interpolation_date_to_tt_tdb(std::vector<Observation> observations, std::vector<InterpolationTimeFrame> interpolation_time) {
    double delta;
    int last_min = 0;
    for (int i = 0; i < observations.size(); i++) {
        int j = last_min;
        for (j; j < interpolation_time.size(); j++) {
            if (observations[i].get_julian_date().get_MJD() < interpolation_time[j].get_julian_date().get_MJD()) {
                last_min = j - 1;
                delta = interpolation_time[j - 1].get_TT_TDB() + (interpolation_time[j].get_TT_TDB() - interpolation_time[j - 1].get_TT_TDB()) / (interpolation_time[j].get_julian_date().get_MJD() - interpolation_time[j - 1].get_julian_date().get_MJD()) * (observations[i].get_julian_date().get_MJD() - interpolation_time[j - 1].get_julian_date().get_MJD());
                observations[i].get_julian_date().set_TT_TDB(delta);
                break;
            }
        }
    }
}


//»нтерпол€ци€ положени€ ’аббла
CartesianCoord Converter::interpolation_hubble_data(Date date, std::vector<InterpolationHubbleFrame> interpolation_data) {
    for (int i = 0; i < interpolation_data.size() - 1; i++) {
        if ((interpolation_data[i].get_date() < date) and (interpolation_data[i + 1].get_date() > date)) {
            CartesianCoord new_frame;

            double x = interpolation_data[i].get_data().get_x() + (interpolation_data[i + 1].get_data().get_x()
                - interpolation_data[i + 1].get_data().get_x()) * (date.get_MJD() - interpolation_data[i].get_date().get_MJD());

            double y = interpolation_data[i].get_data().get_y() + (interpolation_data[i + 1].get_data().get_y()
                - interpolation_data[i + 1].get_data().get_y()) * (date.get_MJD() - interpolation_data[i].get_date().get_MJD());

            double z = interpolation_data[i].get_data().get_z() + (interpolation_data[i + 1].get_data().get_z()
                - interpolation_data[i + 1].get_data().get_z()) * (date.get_MJD() - interpolation_data[i].get_date().get_MJD());

            new_frame.set_x(x);
            new_frame.set_y(y);
            new_frame.set_z(z);
            return new_frame;
        }
    }
};


//“ранспонирование матрицы 3х3
void Converter::transpose(double mtr[3][3]) {
    double tmp[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            tmp[i][j] = mtr[i][j];
        }
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            mtr[j][i] = tmp[i][j];
        }
    }
}


//ѕеревод из земных координат в геоцентрические небесные
GeocentricCoord Converter::cartesian_to_geocentric(CartesianCoord frame, Date date) {
    double transition_matrix[3][3];

    double x, y;
    iauXy06(date.get_TT(), 0, &x, &y);

    iauC2t06a(date.get_TT(), 0, date.get_JD(), 0, x, y, transition_matrix);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            std::cout << transition_matrix[i][j] << " ";
        }
        std::cout << "\n";
    }

    transpose(transition_matrix);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            std::cout << transition_matrix[i][j] << " ";
        }
        std::cout << "\n";
    }

    double geo_x = transition_matrix[0][0] * frame.get_x() + transition_matrix[0][1] * frame.get_y() + transition_matrix[0][2] * frame.get_z();
    double geo_y = transition_matrix[1][0] * frame.get_x() + transition_matrix[1][1] * frame.get_y() + transition_matrix[1][2] * frame.get_z();
    double geo_z = transition_matrix[2][0] * frame.get_x() + transition_matrix[2][1] * frame.get_y() + transition_matrix[2][2] * frame.get_z();

    GeocentricCoord new_frame;
    new_frame.set_x(geo_x);
    new_frame.set_y(geo_y);
    new_frame.set_z(geo_z);
    return new_frame;

};