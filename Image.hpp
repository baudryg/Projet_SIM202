#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <string>
#include "Pixel.hpp"


class Image
{
public: // ATTENTION : A METTRE EN PROTECTED
    double lx_, ly_;
    int nbx_, nby_;
    std::vector<std::vector<Pixel> > vPixel_;

public:
    Image(double, double, int, int);

    int gnbx() {return(nbx_);}
    int gnby() {return(nby_);}
    double glx() {return(lx_);}
    double gly() {return(ly_);}

    void displayImageBMP(const std::string &);
};

#endif // IMAGE_H
