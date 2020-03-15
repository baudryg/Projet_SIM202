#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <vector>

class Point
{
protected:
    double x_, y_, z_;

public:
    Point() : x_(0.0), y_(0.0), z_(0.0) {}
    Point(double x, double y, double z) : x_(x), y_(y), z_(z) {}

    double operator()(int) const;
    double & operator()(int);
    double operator[](int) const;
    double & operator[](int);

    Point & operator=(const Point &);
    Point operator+=(const Point &);
    Point operator-=(const Point &);
    Point operator*=(double);
    Point operator/=(double);
    Point operator^=(const Point &);

    int maxCoord();
};

Point operator+(const Point &, const Point &);
Point operator-(const Point &, const Point &);
Point operator*(double, const Point &);
Point operator*(const Point &, double);
Point operator/(const Point &, double);
double operator|(const Point &, const Point &);
Point operator^(const Point &, const Point &);
bool operator==(const Point &, const Point &);
bool operator!=(const Point &, const Point &);
bool operator<=(const Point &, const Point &); // Regarde si l'une des coordonnees est superieure (utilise ds le KDTree)
bool operator>=(const Point &, const Point &); // Regarde si l'une des coordonnees est inferieure (utilise ds le KDTree)
std::ostream & operator<<(std::ostream &, const Point &);
double norm_euc(const Point &);

double SignedVolume(const Point &, const Point &, const Point &, const Point &);

#endif // M
