#include "Ray_Tracing.hpp"
#include <cmath>
#include "Triangle.hpp"
#include <iostream>
#include <cstdlib>



double RayTracing::intensiteFromAngle(double theta, double alpha)
{
    if (kd_ * cos(theta) + kr_ * pow(cos(alpha), n_) < 0) {
        return(Ia_*ka_);
    }
    else {
        return(Ia_*ka_ + Is_ * (kd_ * cos(theta) + kr_ * pow(cos(alpha), n_)));
    }
}

double RayTracing::intensiteFromAngle()
{
    return(Ia_*ka_);
}

void RayTracing::colorImagePlat()
{
    std::cout << "Nb sommets : " << maillageRT_.gNbPt() << "  |  Nb triangles : " << maillageRT_.gNbTri() << std::endl;

    std::vector<double> vIntensite(maillageRT_.gNbTri());
    std::vector<double> vAng;
    double intensite;
    int id_Tij;
    Point P_intersection;

    for (int i=0; i<maillageRT_.gNbTri(); ++i) {
        vAng = maillageRT_(i).angleOmbragePlat(srcLumRT_, obsRT_);
        vIntensite[i] = intensiteFromAngle(vAng[0], vAng[1]);
    }

    for (int i=0; i<imgRT_.gnbx(); ++i) {
        std::cout << i+1 << " / " << imgRT_.gnbx() << std::endl;
        for (int j=0; j<imgRT_.gnby(); ++j) {
            Point Pij(imgRT_.glx()/double(imgRT_.gnbx())*(i+0.5), imgRT_.gly()/double(imgRT_.gnby())*(imgRT_.gnby() - 1 - j + 0.5), 0.0);

            if (maillageRT_.crossTriangleObs(obsRT_, Pij)) {

                id_Tij = maillageRT_.closestIntersectedTriangleIdObs(obsRT_, Pij);
                P_intersection = maillageRT_(id_Tij).ptIntersectionObs(obsRT_, Pij);

                if (!maillageRT_.crossOtherTriangle(id_Tij, P_intersection, srcLumRT_)) {
                    intensite = vIntensite[id_Tij];
                }
                else {
                    intensite = intensiteFromAngle();
                }
            }
            else {
                intensite = intensiteBack_;
            }

            imgRT_.vPixel_[i][j].fillRGB(uint8_t(intensite*255), uint8_t(intensite*255), uint8_t(intensite*255));
        }
    }
}

void RayTracing::colorImageGouraud()
{
    std::cout << "Nb sommets : " << maillageRT_.gNbPt() << "  |  Nb triangles : " << maillageRT_.gNbTri() << std::endl;

    std::vector<double> vIntensite(maillageRT_.gNbPt());
    std::vector<double> coordBary;
    std::vector<double> vAng;
    double intensite;
    int id_Tij;
    Point P_intersection;

    for (int i=0; i<maillageRT_.gNbPt(); ++i) {
        vAng = maillageRT_.angleOmbrage(i, srcLumRT_, obsRT_);
        vIntensite[i] = intensiteFromAngle(vAng[0], vAng[1]);
    }

    for (int i=0; i<imgRT_.gnbx(); ++i) {
        std::cout << i+1 << " / " << imgRT_.gnbx() << std::endl;
        for (int j=0; j<imgRT_.gnby(); ++j) {
            Point Pij(imgRT_.glx()/double(imgRT_.gnbx())*(i+0.5), imgRT_.gly()/double(imgRT_.gnby())*(imgRT_.gnby() - 1 - j + 0.5), 0.0);

            if (maillageRT_.crossTriangleObs(obsRT_, Pij)) {

                id_Tij = maillageRT_.closestIntersectedTriangleIdObs(obsRT_, Pij);
                P_intersection = maillageRT_(id_Tij).ptIntersectionObs(obsRT_, Pij);

                if (!maillageRT_.crossOtherTriangle(id_Tij, P_intersection, srcLumRT_)) {
                    coordBary = maillageRT_(id_Tij).coordBarycentric(P_intersection);
                    intensite = 0.0;

                    for (int l=0; l<3; ++l) {
                        intensite += coordBary[l] * vIntensite[maillageRT_.getPointAroundTriangle(id_Tij)[l]];
                    }
                }
                else {
                    intensite = intensiteFromAngle();
                }
            }
            else {
                intensite = intensiteBack_;
            }

            imgRT_.vPixel_[i][j].fillRGB(uint8_t(intensite*255), uint8_t(intensite*255), uint8_t(intensite*255));
        }
    }
}

void RayTracing::colorImagePhong()
{
    std::cout << "Nb sommets : " << maillageRT_.gNbPt() << "  |  Nb triangles : " << maillageRT_.gNbTri() << std::endl;

    std::vector<Point> vVectNormal(maillageRT_.gNbPt());
    std::vector<double> coordBary;
    std::vector<double> vAng;
    double intensite;
    int id_Tij;
    Point P_intersection;

    for (int i=0; i<maillageRT_.gNbPt(); ++i) {
        vVectNormal[i] = maillageRT_.vectNormPoint(i);
    }

    for (int i=0; i<imgRT_.gnbx(); ++i) {
        std::cout << i+1 << " / " << imgRT_.gnbx() << std::endl;
        for (int j=0; j<imgRT_.gnby(); ++j) {
            Point Pij(imgRT_.glx()/double(imgRT_.gnbx())*(i+0.5), imgRT_.gly()/double(imgRT_.gnby())*(imgRT_.gnby() - 1 - j + 0.5), 0.0);

            if (maillageRT_.crossTriangleObs(obsRT_, Pij)) {

                id_Tij = maillageRT_.closestIntersectedTriangleIdObs(obsRT_, Pij);
                P_intersection = maillageRT_(id_Tij).ptIntersectionObs(obsRT_, Pij);

                if (!maillageRT_.crossOtherTriangle(id_Tij, P_intersection, srcLumRT_)) {
                    coordBary = maillageRT_(id_Tij).coordBarycentric(P_intersection);
                    Point vectNorm;

                    for (int l=0; l<3; ++l) {
                        vectNorm += coordBary[l] * vVectNormal[maillageRT_.getPointAroundTriangle(id_Tij)[l]];
                    }
                    vAng = maillageRT_.angleOmbrageAvecNorm(P_intersection, vectNorm, srcLumRT_, Pij);
                    intensite = intensiteFromAngle(vAng[0], vAng[1]);
                }
                else{
                    intensite = intensiteFromAngle();
                }
            }
            else {
                intensite = intensiteBack_;
            }

            imgRT_.vPixel_[i][j].fillRGB(uint8_t(intensite*255), uint8_t(intensite*255), uint8_t(intensite*255));
        }
    }
}

void RayTracing::afficherImage(const std::string & filename, int methodNb)
{
    switch (methodNb)
    {
        case 1:
            colorImagePlat();
            break;
        case 2:
            colorImageGouraud();
            break;
        case 3:
            colorImagePhong();
            break;
        default:
            std::cout << "Method non-existent." << std::endl;
            exit(1);
    }

    imgRT_.displayImageBMP(filename);
}


