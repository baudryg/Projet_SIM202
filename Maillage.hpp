#ifndef MAILLAGE_H
#define MAILLAGE_H

#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include "Point.hpp"
#include "Triangle.hpp"


class Maillage {
protected:
    std::vector<Point> vPoint_;
    std::vector<Triangle> vTriangle_;

    std::vector<std::vector<int> > vPointAroundTriangle_;
    std::vector<std::vector<int> > vTriangleAroundPoint_;

public:
    Maillage() {vPoint_.resize(0); vTriangle_.resize(0);}
    Maillage(std::vector<Point> vPoint, std::vector<Triangle> vTriangle) : vPoint_(vPoint), vTriangle_(vTriangle) {}
    void loadGMSH(const std::string &);
    Maillage(const std::string & filename) {loadGMSH(filename);}

    Point operator[](unsigned int) const;
    Triangle operator()(unsigned int) const;

    int gNbPt() const {return vPoint_.size();}
    int gNbTri() const {return vTriangle_.size();}
    std::vector<int> getPointAroundTriangle(int);
    std::vector<int> getTriangleAroundPoint(int);

    bool crossTriangle(const Point &, const Point &);
    bool crossTriangleObs(const Point &, const Point &); // isIntersected pour P_obs et P_pixel
    bool crossOtherTriangle(int, const Point &, const Point &);

    Triangle closestIntersectedTriangle(const Point &, const Point &);
    int closestIntersectedTriangleId(const Point &, const Point &);
    int closestIntersectedTriangleIdObs(const Point &, const Point &); // isIntersected pour P_obs et P_pixel

    Point vectNormPoint(int);
    std::vector<double> angleOmbrage(int, const Point &, const Point &);
    std::vector<double> angleOmbrageAvecNorm(const Point &, const Point &, const Point &, const Point &);
};

template <typename Out>
static inline void split(const std::string & s, char delim, Out result)
{
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        *result++ = item;
    }
}

static inline std::vector<std::string> split(const std::string & s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

#endif // MAILLAGE_H
