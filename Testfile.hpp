#ifndef TESTFILE_HPP_INCLUDED
#define TESTFILE_HPP_INCLUDED

#include <iostream>
#include <sstream>
#include <vector>
#include "Point.hpp"
#include "Triangle.hpp"
#include "Maillage.hpp"
#include "Ray_Tracing.hpp"


template <typename T>
std::string to_string(const T & value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

void testClassePoint()
{
    Point P0;
    std::cout << "P0 vide : " << P0 << std::endl;
    P0[0] = 2.0;
    P0(3) = 1.0;
    std::cout << "P0 apres chgmt 1er et 3eme coords : " << P0 << std::endl;
    Point P = P0;
    std::cout << "P = P0 --> " << P << std::endl;

    Point P1(1.0, 1.0, 1.0);
    Point P2(1.0, 2.0, 2.0);

    std::cout << "P1 : " << P1 << std::endl;
    std::cout << "P2 : " << P2 << std::endl;

    std::cout << "P1/2 - 3 P2 = " << P1/2 - 3 * P2 << std::endl;
    std::cout << "P1 == P2 : " << (P1 == P2) << std::endl;
    std::cout << "P2 == P2 : " << (P2 == P2) << std::endl;

    std::cout << "(P1 | P2) = " << (P1 | P2) << std::endl;
    std::cout << "P1 ^ P2 = " << (P1 ^ P2) << std::endl;
    std::cout << "|| P2 || = " << norm_euc(P2) << std::endl;
}

void testClasseTriangle()
{
    Point P1(1.0, 1.0, 1.0);
    Point P2(1.0, 2.0, 2.0);
    Point P3(0.0, 1.0, 2.0);
    Point P4(2.0, 1.0, 2.0);
    Point P5(1.0, 0.0, 2.0);

    Triangle T0;
    Triangle T1(P1, P2, P3);
    Triangle T = T1;

    std::cout << "T0 : " << T0 << std::endl;
    std::cout << "T1 : " << T1 << std::endl;
    std::cout << "T = T1 --> " << T << std::endl;

    std::cout << "T1[0] " << T1[0] << std::endl;
    std::cout << "T1(3) " << T1(3) << std::endl;

    std::cout << "Aire de T1 = " << T1.aire() << std::endl;
    std::cout << "Vecteur normal de T1 = " << T1.vectNormal() << std::endl;
    std::cout << "maxz de T1 = " << T1.maxz() << std::endl;

    Point P6(0.0, 0.0, 0.0);
    Point P7(1.0, 0.0, 0.0);
    Point P8(0.0, 1.0, 1.0);
    Point P9(0.2, 0.5, 0.5);
    Triangle T2(P6, P7, P8);

    std::vector<double> vCoord(T2.coordBarycentric(P9));

    for (unsigned int i=0; i<vCoord.size(); ++i) {
        std::cout << "Coord " << i+1 << " : " << vCoord[i] << std::endl;
    }

    Point P_obs(1.0, 1.0, -2.0);
    Point P_pixel(0.9, 1.1, 0.0);
    Point P_test(0.7, 1.3, 4.0);

    Point Pi(0.9, 1.1, 0.0);
    Point Pj(-0.1, 0.1, -1.0);
    Point Pk(0.1, 0.9, 2.0);
    std::cout << "Is T1 intersected by P_obs --- P_pixel ? " << T1.isIntersectedObs(P_obs, P_pixel) << std::endl;
    std::cout << "Is T1 intersected by Pi ? " << T1.isIntersected(P_obs, P_test) << std::endl;
    Point P_intersection = T1.ptIntersectionObs(P_obs, P_pixel);
    std::cout << "Intersection worked ? " << T1.isIntersectedObs(P_intersection, Pk) << std::endl;
}

void testClasseMaillage()
{
    Point P1(1.0, 1.0, 1.0);
    Point P2(1.0, 2.0, 2.0);
    Point P3(0.0, 1.0, 2.0);
    Point P4(2.0, 1.0, 2.0);
    Point P5(1.0, 0.0, 2.0);

    Triangle T1(P1, P2, P3);
    Triangle T2(P1, P4, P2);
    Triangle T3(P1, P5, P4);
    Triangle T4(P1, P3, P5);

    std::vector<Point> vPts(5);
    vPts[0] = P1; vPts[1] = P2; vPts[2] = P3; vPts[3] = P4; vPts[4] = P5;

    std::vector<Triangle> vTris(4);
    vTris[0] = T1; vTris[1] = T2; vTris[2] = T3; vTris[3] = T4;

    Maillage M(vPts, vTris);

    std::cout << "Nb de sommets : " << M.gNbPt() << std::endl;
    std::cout << "Nb de triangles : " << M.gNbTri() << std::endl;
    std::cout << "Sommet 3 : " << M[2] << std::endl;
    std::cout << "Triangle 1 : " << 1 << M(0) << std::endl;
}

void testClasseKDNode()
{



}

void testClasseRayTracing_1Image()
{
    Image img(2.0, 2.0, 300, 300);
    Point P_src(0.0, 3.0, 0.5);
    Point P_observ(0.5, 0.5, -4.0);

    Maillage M("Fichiers_GMSH/test_KDNode.msh");

    RayTracing RT(M, img, P_src, P_observ);
    RT.afficherImage("img/test_KDNode.bmp", 3);
}

void testClasseRayTracing_Gif()
{
    Image img(1.0, 2.0, 100, 200);
    Point P_observ(0.5, 1.0, -4.0);

    Maillage M("Fichiers_GMSH/test_deux_spheres.msh");

    for (int frame=0; frame<40; ++frame) {
        Point P_src(0.0, 3.2-frame*0.05, 0.0);
        RayTracing RT(M, img, P_src, P_observ);
        RT.afficherImage("img/gif/img_test_phong_3/frame_" + to_string(frame+1) + ".bmp", 3);
    }
}



#endif // TESTFILE_HPP_INCLUDED
