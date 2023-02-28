#pragma once
#include <string>

//����� ��� �������������� ���������
class CylindricalCoord
{
private:
    double longitude = 0; //�������
    double cos = 0; 
    double sin = 0; 
public:
    //�������
    double get_longitude();
    double get_cos();
    double get_sin();

    bool is_empty(std::string value);

    //�������
    void set_longitude_from_string(std::string value);
    void set_cos_from_string(std::string value);
    void set_sin_from_string(std::string value);
};

