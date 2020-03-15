#ifndef RAY_TRACING_PHONG_H
#define RAY_TRACING_PHONG_H

#include "Image.hpp"
#include "Point.hpp"
#include "Maillage.hpp"

class RayTracingPhong
{
protected:
    Maillage maillageRT_;
    Image imgRT_;
    Point srcLumRT_; // Unique source de lumiere pour l'instant
    vector<double> Ia, Is;
    double ka, kd, kr;
    int n;

public:
    RayTracingPhong(const Maillage & maillageRT, const Image & imgRT, const Point & srcLumRT)
    : maillageRT_(maillageRT), imgRT_(imgRT), srcLumRT_(srcLumRT),
       ka(0.5), kd(0.5), kr(0.5), n(1) {
        Ia.resize(3);
        Is.resize(3);
        for (int i=0;i<3;++i){
            Ia[i]=1.0;
        }
        Is[0]=1.0; //Couleur rouge
        Is[1]=1.0; //Couleur verte
        Is[2]=1.0; //Couleur bleue
      }

    double intensiteFromAngle(double, double, int);
    double intensiteFromAngle();
    void colorImage();
    void afficherImage();
};




#endif // RAY_TRACING_PHONG_H
