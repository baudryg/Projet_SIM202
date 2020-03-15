#include "Ray_TracingPhong.hpp"
#include <cmath>
#include "Triangle.hpp"
#include <iostream>

double distanceDeuxPts1(Point P1,Point P2){
    return(sqrt( (P1(1)-P2(1))*(P1(1)-P2(1)) + (P1(2)-P2(2))*(P1(2)-P2(2)) + (P1(3)-P2(3))*(P1(3)-P2(3)) ));
}

double aireTriangle1(Point P1, Point P2, Point P3){
    double a,b,c,s;
    a=distanceDeuxPts1(P1,P2);
    b=distanceDeuxPts1(P1,P3);
    c=distanceDeuxPts1(P2,P3);
    s=0.5*(a+b+c);
    return(sqrt(s*(s-a)*(s-b)*(s-c)));
}


double RayTracingPhong::intensiteFromAngle()
{
    return(Ia[0]*ka);
}

double RayTracingPhong::intensiteFromAngle(double theta, double alpha, int i)
{
    return(Ia[i]*ka + Is[i] * (kd * cos(theta) + kr * pow(cos(alpha), n)));
}

void RayTracingPhong::colorImage()
{
    int nbx=imgRT_.gnbx();
    int nby=imgRT_.gnby();
    for (int i=0; i<nbx; ++i) {
        cout << i << "/" << nbx <<endl;
        for (int j=0; j<nby; ++j) {

            Point Pij(imgRT_.glx()/double(nbx)*(i+0.5), imgRT_.gly()/double(nby)*(nby-j-0.5), 0.0);

            if (maillageRT_.crossTriangle(Pij)) {
                int id_Tij = maillageRT_.closestIntersectedTriangleId(Pij);
                Triangle Tij = maillageRT_(id_Tij);
                Point P_intersection = Tij.ptIntersection(Pij);
                if (false && (maillageRT_.crossOtherTriangle(id_Tij, P_intersection, srcLumRT_))) {
                    //Il faut revoir la méthode crossOtherTriangle(id_Tij, P_intersection, srcLumRT_)
                    for (int k=0; k<3; ++k) {
                        (imgRT_.vPixel_[i][j])[k] = uint8_t(intensiteFromAngle()/1.5*255);
                    }
                    /*cout << "i = " << i << " j = " << j << endl;
                    Tij.print();
                    cout << "intensite : " << intensiteFromAngle() << " ";
                    Pij.print();*/
                }
                else {
                    //cout<<"i,j= "<<i<<", "<<j<<endl;
                    //Calculer les coordeonnées du point d'intersection dans le triangle
                    vector<Point> NormauxSommets;
                    NormauxSommets.resize(3);
                    for(int i=0;i<3;i++){
                        NormauxSommets[i]=Point(0,0,0);
                    }

                    double aireTriangleDeBase=aireTriangle1(Tij(1),Tij(2),Tij(3));
                    double u=aireTriangle1(Tij(1),Tij(3),P_intersection)/aireTriangleDeBase;
                    double v=aireTriangle1(Tij(1),Tij(2),P_intersection)/aireTriangleDeBase;
                    //if(i==j) cout<<"u,v= "<<u<<","<<v<<endl;
                    for(int z=1;z<4;++z){
                        int cpt=0;
                        for(int t=0;t<maillageRT_.vTriangle_.size();++t){
                            if(maillageRT_.vTriangle_[t](1)==Tij(z) || maillageRT_.vTriangle_[t](2)==Tij(z) || maillageRT_.vTriangle_[t](3)==Tij(z)) {
                                cpt++;
                                NormauxSommets[z-1]+=maillageRT_.vTriangle_[t].vectNormal();
                                if(i==101 && j==190){
                                    cout<<"cpt=" <<cpt<<endl;
                                    cout<<"maillageRT_.vTriangle_[t].vectNormal()=" ;
                                    maillageRT_.vTriangle_[t].vectNormal().print();
                                    cout<<"NormauxSommets["<<z-1<<"] vaut mtn: ";
                                    NormauxSommets[z-1].print();
                                }
/*                                if(i==50 && j==98){
                                        cout<<"Contribution du triangle N "<<cpt<<" = "<<int(intensiteFromAngle(Angij[0], Angij[1])/1.5*255)+1<<endl;
                                        cout<<"LumSommets["<<z-1<<"] vaut mtn: "<<LumSommets[z-1][0]+1<<endl;
                                }*/
                            }
                        }
                        if (cpt==0) {cout<<"cpt=0 pour z="<<z<<endl;break; }
                        else {
                            if(i==101 && j==190){cout<<"Now let's calculate the final values, "<<"NormauxSommets["<<z-1<<"] vaut mtn: ";
                            NormauxSommets[z-1].print();
                            }
                            if (cpt<3) cout<<"cpt ="<<cpt<<" for i="<<i<<" & j="<<j<<endl;
                            NormauxSommets[z-1]=NormauxSommets[z-1]*(1/norm_euc(NormauxSommets[z-1]));
                            if(i==101 && j==190){
                                cout<<"NormauxSommets["<<z-1<<"] vaut mtn: ";
                                NormauxSommets[z-1].print();
                            }
                        }
                        //cout<<"for i,j="<<i<<","<<j<<" and z="<<z<<", on a LumSommets[z-1][2]="<<LumSommets[z-1][2]+1<<endl;

                    }

                    Point normalAuPtInter=NormauxSommets[1]*u+ NormauxSommets[2]*v + NormauxSommets[0]*(1-u-v);
                    normalAuPtInter=normalAuPtInter*(1/norm_euc(normalAuPtInter));


                    Point projBary_ = P_intersection;
                    projBary_[2] = 0.0;

                    vector<double> vAng(2);
                    vAng[0] = acos((normalAuPtInter | (srcLumRT_ - P_intersection)) / norm_euc(srcLumRT_ - P_intersection)); //theta
                    vAng[1] = acos((normalAuPtInter | (projBary_-P_intersection))/norm_euc(projBary_-P_intersection)) - vAng[0]; //alpha


                    for (int k=0; k<3; ++k) {
                        (imgRT_.vPixel_[i][j])[k] = uint8_t(intensiteFromAngle(vAng[0], vAng[1],k)/1.5*255);
                    }

                    /*cout << "i = " << i << " j = " << j << endl;
                    Tij.print();
                    cout << "theta = " << Angij[0] << "alpha = " << Angij[1] << endl;
                    cout << "intensite : " << intensiteFromAngle(Angij[0], Angij[1]) << " ";
                    Pij.print();*/
                }
            }
            else {
                for (int k=0; k<3; ++k) {
                    (imgRT_.vPixel_[i][j])[k] = uint8_t(0);
                }
                //cout <<"pas de croisement : ";
                //Pij.print();
            }
        }
    }
}

void RayTracingPhong::afficherImage()
{
    colorImage();
    imgRT_.displayImageBMP();
}
