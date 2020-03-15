#ifndef RAY_TRACING_H
#define RAY_TRACING_H

#include <string>
#include "Image.hpp"
#include "Point.hpp"
#include "Maillage.hpp"

class RayTracing
{
protected:
    Maillage maillageRT_;
    Image imgRT_;
    Point srcLumRT_; // Unique source de lumiere pour l'instant
    Point obsRT_;
    double Ia_, Is_;
    double ka_, kd_, kr_;
    int n_;
    double intensiteBack_;

public:
    RayTracing(const Maillage & maillageRT, const Image & imgRT, const Point & srcLumRT, const Point & obsRT)
    : maillageRT_(maillageRT), imgRT_(imgRT), srcLumRT_(srcLumRT), obsRT_(obsRT),
      Ia_(1.0), Is_(1.0), ka_(0.05), kd_(0.7), kr_(0.15), n_(5), intensiteBack_(0.78) {}

    double intensiteFromAngle(double, double);
    double intensiteFromAngle();

    void colorImagePlat();
    void colorImageGouraud();
    void colorImagePhong();

    void afficherImage(const std::string &, int);
};

double aireTriangle1(Point, Point, Point);
double distanceDeuxPts1(Point, Point);

#endif // RAY_TRACING_H
