#include "moment.h"

// Fill the vector m such that m[i][j] =  M_{ij} = \sum_x \sum_y x^i y^j I(x,y)
// Pre-condition: m[i][j] = 0 for all i
void rawMoment(const Image &image, Point lowerBound, Point upperBound, vector<vector<double>> &m) {
    for(int x = lowerBound[0] ; x < upperBound[0] ; x++) {
        for(int y = lowerBound[1] ; y < upperBound[1] ; y++) {
            if(image(Point(x, y))) {// I(x, y) is in {0, 1}
                for(unsigned i = 0 ; i < m.size() ; i++) {
                    for(unsigned j = 0 ; j < m[i].size() ; j++) {
                        m[i][j] += pow(x, i)*pow(y, j) ;
                    }
                }
            }
        }
    }
}
#include <iostream>

// Compute moments of the image, using http://en.wikipedia.org/wiki/Image_moment
// These moments are translation, scale and rotation invariants.
vector<double> moment(Image image)  {

    // Raw moment
    vector<vector<double>> M(4, vector<double>(4, 0)) ;
    Point lowerBound = image.domain().lowerBound(), upperBound = image.domain().upperBound() ;
    rawMoment(image, lowerBound, upperBound, M) ;

    // Central moments
    vector<vector<double>> mu(4, vector<double>(4, 0)) ;
    double xbar = M[1][0]/M[0][0], ybar = M[0][1]/M[0][0] ;
    mu[0][0] = M[0][0] ;
    mu[1][1] = M[1][1] - xbar*M[0][1] ;
    mu[2][0] = M[2][0] - xbar*M[1][0] ;
    mu[0][2] = M[0][2] - ybar*M[0][1] ;
    mu[2][1] = M[2][1] - 2*xbar*M[1][1] - ybar*M[2][0] + 2*pow(xbar, 2)*M[0][1] ;
    mu[1][2] = M[1][2] - 2*ybar*M[1][1] - xbar*M[0][2] + 2*pow(ybar, 2)*M[1][0] ;
    mu[3][0] = M[3][0] - 3*xbar*M[2][0] + 2*pow(xbar, 2)*M[1][0] ;
    mu[0][3] = M[0][3] - 3*ybar*M[0][2] + 2*pow(ybar, 2)*M[0][1] ;

    // Scale invariant moments
    vector<vector<double>> eta(4, vector<double>(4, 0)) ;
    for(unsigned i = 0 ; i < eta.size() ; i++) {
        for(unsigned j = 0 ; j < eta.size() ; j++) {
            eta[i][j] = mu[i][j]/pow(mu[0][0], 1+((double)(i+j))/2) ;
        }
    }

    // Rotation invariant moments
    vector<double> result ;
    result.push_back(eta[2][0] + eta[0][2]) ; // I1
    result.push_back(pow(eta[2][0] - eta[0][2], 2) + 4*pow(eta[1][1], 2)) ; // I2
    result.push_back(pow(eta[3][0] - 3*eta[1][2], 2) + pow(3*eta[2][1] - eta[0][3], 2)) ; // I3
    result.push_back(pow(eta[3][0] + eta[1][2], 2) + pow(eta[2][1] + eta[0][3], 2)) ; // I4
    result.push_back((eta[3][0]-3*eta[1][2])*(eta[3][0]+eta[1][2])*(pow(eta[3][0]+eta[1][2], 2) -3*pow(eta[2][1]+eta[0][3], 2)) +\
        (3*eta[2][1] - eta[0][3])*(eta[2][1]+eta[0][3])*(3*pow(eta[3][0]+eta[1][2], 2) -pow(eta[2][1]+eta[0][3], 2))) ; // I5
    result.push_back((eta[2][0]-eta[0][2])*(pow(eta[3][0]+eta[1][2], 2)-pow(eta[2][1]+eta[0][3], 2)) +\
        4*eta[1][1]*(eta[3][0]+eta[1][2])*(eta[2][1]+eta[0][3])) ; // I6
    result.push_back((3*eta[2][1]-eta[0][3])*(eta[3][0]+eta[1][2])*(pow(eta[3][0]+eta[1][2], 2) -3*pow(eta[2][1]+eta[0][3], 2)) -\
        (eta[3][0]-3*eta[1][2])*(eta[2][1]+eta[0][3])*(3*pow(eta[3][0]+eta[1][2], 2) -pow(eta[2][1]+eta[0][3], 2))) ; // I7
    result.push_back(eta[1][1]*(pow(eta[3][0]+eta[1][2], 2) -pow(eta[0][3]+eta[2][1], 2)) -\
        (eta[2][0]-eta[0][2])*(eta[3][0]+eta[1][2])*(eta[0][3]+eta[2][1])) ; // I8
    return result ;
}
