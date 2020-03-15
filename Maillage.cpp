#include "Maillage.hpp"

#include <string>
#include <fstream>
#include <ostream>
#include <iostream>
#include <cstdlib>
#include <limits>
#include <math.h>
#include "Triangle.hpp"


// FONCTIONS MEMBRES DE LA CLASSE MAILLAGE

void Maillage::loadGMSH(const std::string & filename)
{
	vPoint_.resize(0);
	vTriangle_.resize(0);
	vTriangleAroundPoint_.resize(0);
	vPointAroundTriangle_.resize(0);
	char p_space = 32;

	std::ifstream infile;
	infile.open(filename.c_str(), std::ifstream::in);

	/*if !(infile) {
        std::cout << "Le fichier n'a pas ete trouve.";
        infile.close();
        exit(1);
	}*/

    std::string line;

    getline(infile, line);
    while (line != "$Nodes")
    {
        getline(infile, line);
    }
    getline(infile, line);

    std::vector<std::string> line_split = split(line, p_space);
    int numEntityBlocks = atoi(line_split[0].c_str());
    //int numNodes = atoi(line_split[1].c_str());
    //int minNodeTag = atoi(line_split[2].c_str());
    //int maxNodeTag = atoi(line_split[3].c_str());
    for (int i=0; i<numEntityBlocks; ++i) {
        getline(infile, line);
        std::vector<std::string> node_info = split(line, p_space);
        int entityDim = atoi(node_info[0].c_str());
        //int entityTag = atoi(node_info[1].c_str());
        //int parametric = atoi(node_info[2].c_str());
        int numNodesInBlock = atoi(node_info[3].c_str());

        if (entityDim != 2 && false) { // On prend tous les sommets pour éviter de reeindexe les triangles
            for (int j=0; j<2*numNodesInBlock; ++j) {
                getline(infile, line);
            }
        }
        else{
            int prev_size = vPoint_.size();
            vPoint_.resize(prev_size + numNodesInBlock);
            for (int j=0; j<numNodesInBlock; ++j) {
                getline(infile, line);
            }
            for (int j=0; j<numNodesInBlock; ++j) {
                getline(infile, line);
                std::vector<std::string> node_coor = split(line, p_space);
                Point P(atof(node_coor[0].c_str()), atof(node_coor[1].c_str()), atof(node_coor[2].c_str()));
                vPoint_[prev_size + j] = P;
            }
        }
    }

	vTriangleAroundPoint_.resize(gNbPt());

    getline(infile, line);
    if (line != "$EndNodes") {
        std::cout << "Issue when reading .gmsh file" << std::endl;
    }
    getline(infile, line);
    getline(infile, line);

    line_split = split(line, p_space);
    numEntityBlocks = atoi(line_split[0].c_str());
    //int numElements = atoi(line_split[1].c_str());
    //int minElementTag = atoi(line_split[2].c_str());
    //int maxElementTag = atoi(line_split[3].c_str());
    for (int i=0; i<numEntityBlocks; ++i) {
        getline(infile, line);
        std::vector<std::string> element_info = split(line, p_space);
        //int entityDim = atoi(element_info[0].c_str());
        //int entityTag = atoi(element_info[1].c_str());
        int elementType = atoi(element_info[2].c_str());
        int numElementsInBlock = atoi(element_info[3].c_str());

        if (elementType != 2) {
            for (int j=0; j<numElementsInBlock; ++j) {
                getline(infile, line);
            }
        }
        else{
            int prev_size = vTriangle_.size();
            vTriangle_.resize(prev_size + numElementsInBlock);
            vPointAroundTriangle_.resize(prev_size + numElementsInBlock);
            for (int j=0; j<numElementsInBlock; ++j) {
                getline(infile, line);
                std::vector<std::string> element_coor = split(line, p_space);
                Triangle T(vPoint_[atoi(element_coor[1].c_str())-1], vPoint_[atoi(element_coor[2].c_str())-1], vPoint_[atoi(element_coor[3].c_str())-1]);
                vTriangle_[prev_size + j] = T;

                vPointAroundTriangle_[prev_size + j].resize(3);
                vPointAroundTriangle_[prev_size + j][0] = atoi(element_coor[1].c_str())-1;
                vPointAroundTriangle_[prev_size + j][1] = atoi(element_coor[2].c_str())-1;
                vPointAroundTriangle_[prev_size + j][2] = atoi(element_coor[3].c_str())-1;

                vTriangleAroundPoint_[atoi(element_coor[1].c_str())-1].push_back(prev_size + j);
                vTriangleAroundPoint_[atoi(element_coor[2].c_str())-1].push_back(prev_size + j);
                vTriangleAroundPoint_[atoi(element_coor[3].c_str())-1].push_back(prev_size + j);
            }

        }
    }

	infile.close();
}

Point Maillage::operator[](unsigned int i) const
{
    if (i >= vPoint_.size()) {
        std::cout << "Indice incorrect pour le nombre de points" << std::endl;
        exit(1);
    }
    return(vPoint_[i]);
}

Triangle Maillage::operator()(unsigned int i) const
{
    if (i >= vTriangle_.size()) {
        std::cout << "Indice incorrect pour le nombre de triangles" << std::endl;
        exit(1);
    }
    return(vTriangle_[i]);
}

bool Maillage::crossTriangle(const Point & P, const Point & Q)
{
    bool crossT(false);

    for (int i=0; i<gNbTri(); ++i) {
        if (vTriangle_[i].isIntersected(P, Q)) {
            crossT = true;
            break;
        }
    }
    return(crossT);
}

bool Maillage::crossTriangleObs(const Point & P_obs, const Point & P_pixel)
{
    bool crossT(false);

    for (int i=0; i<gNbTri(); ++i) {
        if (vTriangle_[i].isIntersectedObs(P_obs, P_pixel)) {
            crossT = true;
            break;
        }
    }
    return(crossT);
}

bool Maillage::crossOtherTriangle(int idTriangle, const Point & P, const Point & Q)
{
    bool crossT(false);

    for (int i=0; i<gNbTri(); ++i) {
        if (vTriangle_[i].isIntersected(P, Q) && i != idTriangle && (vTriangle_[i].vectNormal() | vTriangle_[idTriangle].vectNormal()) < 0) {
            crossT = true;
            break;
        }
    }
    return(crossT);
}

Triangle Maillage::closestIntersectedTriangle(const Point & P, const Point & Q)
{
    Point P_intersection;
    double norm_intersection;
    Triangle T_min;
    double norm_min(std::numeric_limits<double>::infinity());

    for (int i=0; i<gNbTri(); ++i) {
        if (vTriangle_[i].isIntersected(P, Q)) {
            P_intersection = vTriangle_[i].ptIntersection(P, Q);
            norm_intersection = norm_euc(P_intersection-P);
            if (norm_intersection < norm_min) {
                norm_min = norm_intersection;
                T_min = vTriangle_[i];
            }
        }
    }
    return(T_min);
}

int Maillage::closestIntersectedTriangleId(const Point & P, const Point & Q)
{
    Point P_intersection;
    double norm_intersection;
    int id_T_min(-1);
    double norm_min(std::numeric_limits<double>::infinity());

    for (int i=0; i<gNbTri(); ++i) {
        if (vTriangle_[i].isIntersected(P, Q)) {
            P_intersection = vTriangle_[i].ptIntersection(P, Q);
            norm_intersection = norm_euc(P_intersection-P);
            if (norm_intersection < norm_min) {
                norm_min = norm_intersection;
                id_T_min = i;
            }
        }
    }
    return(id_T_min);
}

int Maillage::closestIntersectedTriangleIdObs(const Point & P_obs, const Point & P_pixel)
{
    Point P_intersection;
    double norm_intersection;
    int id_T_min(-1);
    double norm_min(std::numeric_limits<double>::infinity());

    for (int i=0; i<gNbTri(); ++i) {
        if (vTriangle_[i].isIntersectedObs(P_obs, P_pixel)) {
            P_intersection = vTriangle_[i].ptIntersectionObs(P_obs, P_pixel);
            norm_intersection = norm_euc(P_intersection-P_obs);
            if (norm_intersection < norm_min) {
                norm_min = norm_intersection;
                id_T_min = i;
            }
        }
    }
    return(id_T_min);
}

std::vector<int> Maillage::getPointAroundTriangle(int i)
{
    return(vPointAroundTriangle_[i]);
}

std::vector<int> Maillage::getTriangleAroundPoint(int i)
{
    return(vTriangleAroundPoint_[i]);
}

Point Maillage::vectNormPoint(int idxPoint) // Utilisation de vectNormBis pour les Points
{
    Point normPoint;
    std::cout << idxPoint << " | " << << std::endl;
    for (unsigned int i=0; i<vTriangleAroundPoint_[idxPoint].size(); ++i) {
        normPoint += vTriangle_[vTriangleAroundPoint_[idxPoint][i]].vectNormalBis();
        std::cout << vTriangleAroundPoint_[idxPoint][i] << std::endl;
    }
    std::cout << "Total : " << normPoint << std::endl << std::endl;
    if (norm_euc(normPoint) == 0.0) {
        std::cout << "Triangles plats -> vecteur normal nul" << std::endl;
        exit(1);
    }
    normPoint /= norm_euc(normPoint);
    return(normPoint);
}

std::vector<double> Maillage::angleOmbrage(int id_P, const Point & P_lum, const Point & P_pixel)
{
    std::vector<double> vAng(2);
    Point vectNorm = vectNormPoint(id_P);
    vAng[0] = acos((vectNorm | (P_lum - vPoint_[id_P])) / norm_euc(P_lum - vPoint_[id_P])); //theta
    vAng[1] = acos((vectNorm | (P_pixel - vPoint_[id_P])) / norm_euc(P_pixel - vPoint_[id_P])) - vAng[0]; //alpha
    return(vAng);
}

std::vector<double> Maillage::angleOmbrageAvecNorm(const Point & P_intersection, const Point & vectNorm, const Point & P_lum, const Point & P_pixel)
{
    std::vector<double> vAng(2);
    vAng[0] = acos((vectNorm | (P_lum - P_intersection)) / norm_euc(P_lum - P_intersection)); //theta
    vAng[1] = acos((vectNorm | (P_pixel - P_intersection)) / norm_euc(P_pixel - P_intersection)) - vAng[0]; //alpha
    return(vAng);
}
