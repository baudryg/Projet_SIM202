#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vector>
#include "Point.hpp"

class Triangle
{
protected:
    Point A_;
    Point B_;
    Point C_;
    Point bary_; // Isobarycentre du triangle

public:
    Triangle();
    Triangle(const Point &, const Point &, const Point &);

    Triangle & operator=(const Triangle &);

    Point & operator[](int);
    Point operator[](int) const;
    Point & operator()(int);
    Point operator()(int) const;

    double aire();
    Point vectNormal();
    Point vectNormalBis(); // A supprimer
    double maxz();
    std::vector<double> coordBarycentric(const Point &);

    bool isIntersected(const Point &, const Point &);
    bool isIntersectedObs(const Point &, const Point &); // isIntersected pour P_obs et P_pixel
    Point ptIntersection(const Point &, const Point &);
    Point ptIntersectionObs(const Point &, const Point &); // ptIntersection pour P_obs et P_pixel

    std::vector<double> angleOmbragePlat(const Point &, const Point &);
};

std::ostream & operator<<(std::ostream & out, const Triangle &);


// CLASSE TRIPLET POUR AVOIR SEULEMENT LES INDICES DES SOMMETS
/*
class Triplet {
protected:
    int iA_, iB_, iC_;

public:
    Triplet() : iA_(0), iB_(0), iC_(0) {}
    Triplet(int iA, int iB, int iC) : iA_(iA), iB_(iB), iC_(iC) {}

    int operator[](int i) const
    {
        switch (i)
        {
            case 0:
                return(iA_);
            case 1:
                return(iB_);
            case 2:
                return(iC_);
            default:
                std::cout << "Indice different de 0, 1 ou 2." << std::endl;
                exit(1);
        }
    }
};
*/

#endif // TRIANGLE_H
