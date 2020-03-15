#include "Triangle.hpp"
#include <iostream>
#include <cmath>
#include <cstdlib>


// FONCTIONS MEMBRES DE LA CLASSE TRIANGLE

Triangle::Triangle()
{
    Point A, B, C;
    A_ = A;
    B_ = B;
    C_ = C;
    bary_ = (A_ + B_ + C_)/3;
}

Triangle::Triangle(const Point & A, const Point & B, const Point & C)
: A_(A), B_(B), C_(C)
{
    bary_ = (A_ + B_ + C_)/3;
}

Triangle & Triangle::operator=(const Triangle & T)
{
    A_ = T.A_;
    B_ = T.B_;
    C_ = T.C_;
    bary_ = (A_ + B_ + C_)/3;
    return(*this);
}

Point Triangle::operator[](int i) const
{
    switch (i)
      {
         case 0:
            return(A_);
         case 1:
            return(B_);
         case 2:
            return(C_);
         default:
            std::cout << "Indice different de 0, 1 ou 2." << std::endl;
            exit(1);
      }
}

Point & Triangle::operator[](int i)
{
    switch (i)
      {
         case 0:
            return(A_);
         case 1:
            return(B_);
         case 2:
            return(C_);
         default:
            std::cout << "Indice different de 0, 1 ou 2." << std::endl;
            exit(1);
      }
}

Point Triangle::operator()(int i) const
{
    switch (i)
      {
         case 1:
            return(A_);
         case 2:
            return(B_);
         case 3:
            return(C_);
         default:
            std::cout << "Indice different de 1, 2 ou 3." << std::endl;
            exit(1);
      }
}

Point & Triangle::operator()(int i)
{
    switch (i)
      {
         case 1:
            return(A_);
         case 2:
            return(B_);
         case 3:
            return(C_);
         default:
            std::cout << "Indice different de 1, 2 ou 3." << std::endl;
            exit(1);
      }
}

double Triangle::aire()
{
    return(0.5 * norm_euc((B_-A_) ^ (C_-A_)));
}

Point Triangle::vectNormal()
{
    Point norm((A_-B_) ^ (B_-C_));
    if (norm_euc(norm) == 0.0) {
        std::cout << "Triangle plat -> vecteur normal nul" << std::endl;
        exit(1);
    }
    norm /= norm_euc(norm);
    return(norm);
}

Point Triangle::vectNormalBis()
{
    Point norm((A_-B_) ^ (B_-C_));
    return(norm);
}

double Triangle::maxz()
{
    return(std::max(std::max(A_[2], B_[2]), C_[2]));
}

std::vector<double> Triangle::coordBarycentric(const Point & P_intersection)
{
    Triangle T1(P_intersection, B_, C_);
    Triangle T2(A_, P_intersection, C_);
    Triangle T3(A_, B_, P_intersection);
    double aireT = aire();
    std::vector<double> vCoordBary(3);
    vCoordBary[0] = T1.aire()/aireT;
    vCoordBary[1] = T2.aire()/aireT;
    vCoordBary[2] = T3.aire()/aireT;
    return(vCoordBary);
}

bool Triangle::isIntersected(const Point & P1, const Point & P2)
{
    double vol_1, vol_2, vol_3, vol_4, vol_5;
    vol_1 = SignedVolume(P1, A_, B_, C_);
    vol_2 = SignedVolume(P2, A_, B_, C_);
    vol_3 = SignedVolume(P1, P2, A_, B_);
    vol_4 = SignedVolume(P1, P2, B_, C_);
    vol_5 = SignedVolume(P1, P2, C_, A_);
    return((vol_1 * vol_2 < 0) && (vol_3 * vol_4 > 0) && (vol_4 * vol_5 > 0));
}

bool Triangle::isIntersectedObs(const Point & P_obs, const Point & P_pixel)
{
    Point P_max((P_pixel - P_obs) * (1 + 2 * maxz()/(P_pixel[2] - P_obs[2])) + P_obs);
    return(isIntersected(P_obs, P_max));
}

Point Triangle::ptIntersection(const Point & P1, const Point & P2)
{
    Point N((B_-A_) ^ (C_-A_));
    double t = - ((P1-A_) | N) / ((P2-P1) | N);
    return(P1 + t*(P2-P1));
}

Point Triangle::ptIntersectionObs(const Point & P_obs, const Point & P_pixel)
{
    Point P_max((P_pixel - P_obs) * (1 + 2 * maxz()/(P_pixel[2] - P_obs[2])) + P_obs);
    Point N((B_-A_) ^ (C_-A_));
    double t = - ((P_obs-A_) | N) / ((P_max-P_obs) | N);
    return(P_obs + t*(P_max-P_obs));
}

std::vector<double> Triangle::angleOmbragePlat(const Point & P_lum, const Point & P_pixel)
{
    Point norm = vectNormal();
    std::vector<double> vAng(2);
    vAng[0] = acos((norm | (P_lum-bary_)) / norm_euc(P_lum-bary_)); //theta
    vAng[1] = acos((norm | (P_pixel-bary_))/norm_euc(P_pixel-bary_)) - vAng[0]; //alpha
    return(vAng);
}

// FONCTIONS EXTERNES DE LA CLASSE TRIANGLE

std::ostream & operator<<(std::ostream & out, const Triangle & T)
{
    std::cout << "A : " << T[0] << " | B : " << T[1] << " | C : " << T[2];
    return(out);
}
