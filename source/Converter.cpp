#include "Converter.h"


CartesianCoord Converter::cylindrical_to_cartesian(CylindricalCoord cylindrical_coord)
{
    /*
        Convert cylindrical to cartesian coords for observatory
    */
    CartesianCoord cartesian_coord;
    if (cylindrical_coord.get_longitude() != 0) 
    {
        if (cylindrical_coord.get_cos() != 0) 
        {
            cartesian_coord.set_x(cos(cylindrical_coord.get_longitude()) * cylindrical_coord.get_cos() * EARTH_RADIUS);
            cartesian_coord.set_y(sin(cylindrical_coord.get_longitude()) * cylindrical_coord.get_cos() * EARTH_RADIUS);
        }
        if (cylindrical_coord.get_sin() != 0)
        {
            cartesian_coord.set_z(cylindrical_coord.get_sin() * EARTH_RADIUS);
        }
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
            if ((observations.at(i)).get_julian_date().get_MJD() < interpolation_time[j].get_julian_date().get_MJD()) {
                last_min = j - 1;
                delta = interpolation_time[j - 1].get_TT_TDB() + (interpolation_time[j].get_TT_TDB() - interpolation_time[j - 1].get_TT_TDB()) / (interpolation_time[j].get_julian_date().get_MJD() - interpolation_time[j - 1].get_julian_date().get_MJD()) * ((observations.at(i)).get_julian_date().get_MJD() - interpolation_time[j - 1].get_julian_date().get_MJD());
                (observations.at(i)).get_julian_date().set_TT_TDB(delta);
                break;
            }
        }
    }
}


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


//»нтерпол€ци€ центра «емли
BarycentricCoord Converter::interpolation_center_of_earth_for_observatory(Date date, GeocentricCoord frame, std::vector<InterpolationEarthFrame> interpolation_earth) {
    double delta_x;
    double delta_y;
    double delta_z;
    for (int j = 0; j < interpolation_earth.size(); j++) {
        if (date.get_MJD() < interpolation_earth[j].get_julian_date().get_MJD()) {
            delta_x = interpolation_earth[j - 1].get_x() + (interpolation_earth[j].get_x() - interpolation_earth[j - 1].get_x()) / (interpolation_earth[j].get_julian_date().get_MJD() - interpolation_earth[j - 1].get_julian_date().get_MJD()) * (date.get_MJD() - interpolation_earth[j - 1].get_julian_date().get_MJD());
            delta_y = interpolation_earth[j - 1].get_y() + (interpolation_earth[j].get_y() - interpolation_earth[j - 1].get_y()) / (interpolation_earth[j].get_julian_date().get_MJD() - interpolation_earth[j - 1].get_julian_date().get_MJD()) * (date.get_MJD() - interpolation_earth[j - 1].get_julian_date().get_MJD());
            delta_z = interpolation_earth[j - 1].get_z() + (interpolation_earth[j].get_z() - interpolation_earth[j - 1].get_z()) / (interpolation_earth[j].get_julian_date().get_MJD() - interpolation_earth[j - 1].get_julian_date().get_MJD()) * (date.get_MJD() - interpolation_earth[j - 1].get_julian_date().get_MJD());
            BarycentricCoord new_frame;
            new_frame.set_x(frame.get_x() + delta_x);
            new_frame.set_y(frame.get_y() + delta_y);
            new_frame.set_z(frame.get_z() + delta_z);
            return new_frame;
        }
    }
}