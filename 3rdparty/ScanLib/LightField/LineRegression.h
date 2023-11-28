#ifndef _LINE_REGRESSION_H_
#define _LINE_REGRESSION_H_

#include <iostream>
#include <vector>
#include "Geometry/Point.h"
#include "Geometry/Line.h"
#include "Geometry/Plane.h"
#include <Eigen/Dense>
#include <Eigen/Eigenvalues> 


class CLineRegression {

public:
    CLineRegression() {};
    ~CLineRegression() {};

protected:

    //fit a list of points by a 3D line

    /*!
    *   Given a list of points, compute the mean of them
    */
    MeshLib::CPoint  _mean(std::vector<MeshLib::CPoint>& points)
    {
        size_t n = points.size();
        MeshLib::CPoint mean(0, 0, 0);
        for (auto p : points) {
            mean += p;
        }
        mean /= n;

        return mean;
    }

    /*!
    *   Given a list point, {p_i}
    *   q_i = p_i - mean
    *   covariance matrix = \sum_i q_i q_i^T
    */
    Eigen::Matrix3d _covariance_matrix(std::vector<MeshLib::CPoint>& points, MeshLib::CPoint & mean )
    {
        Eigen::Matrix3d cov = Eigen::Matrix3d::Zero();
        for (auto p : points)
        {
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++) {
                    cov(i, j) += (p[i] - mean[i]) * (p[j] - mean[j]);
                }
        }
        return cov;
    }

    /*!
     *  Compute the eigen decomposition of the covariance matrix
     *  return the eigen vector corresponding to the biggest eigenvalue
     */
    MeshLib::CPoint _pca_direction(Eigen::Matrix3d& A )
    {
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> es;
        es.compute(A);
/*
        std::cout << "The eigenvalues of A are:" << std::endl << es.eigenvalues() << std::endl;
        std::cout << "The matrix of eigenvectors, V, is:" << std::endl << es.eigenvectors() << std::endl ;
*/
        MeshLib::CPoint d;
        for (int i = 0; i < 3; i++) {
            d[i] = es.eigenvectors().col(2)(i);
        }
        return d;
    }

public:

    /*!
     *  Fit a 3D line to a list 3D points
     *
     */
    MeshLib::CLine fit(std::vector<MeshLib::CPoint>& points)
    {
        MeshLib::CPoint mean = _mean(points);
        Eigen::Matrix3d cov = _covariance_matrix(points, mean);
        MeshLib::CPoint dir = _pca_direction(cov);

        MeshLib::CLine l(mean, dir);
        return l;
    }

    /*!
    *   Compute the common point of a bundle of lines (a_i,n_i)
    *
    *   d_i^2 = (p-a_i,p_a_i) - (p-a_i,n_i)^2
    *   
    *   https://math.stackexchange.com/questions/61719/finding-the-intersection-point-of-many-lines-in-3d-point-closest-to-all-lines
    * 
    *   [\sum_i (n_i * n_i^T - I )] * p = \sum_i [ n_i * n_i^T-I] * a_i
    */
    MeshLib::CPoint intersect(std::vector<MeshLib::CLine>& lines)
    {
        Eigen::Matrix3d I = Eigen::Matrix3d::Identity();
        Eigen::Vector3d b = Eigen::Vector3d::Zero();
        Eigen::Matrix3d S = Eigen::Matrix3d::Zero();

        for (auto l : lines)
        {
            MeshLib::CPoint _a = l.base();
            MeshLib::CPoint _n = l.direction();

            Eigen::Vector3d a(_a[0], _a[1], _a[2]);
            Eigen::Vector3d n(_n[0], _n[1], _n[2]);

            b += (n * n.transpose() - I) * a;
            S += (n * n.transpose() - I);
        }
        //QR method
        Eigen::Vector3d p = S.colPivHouseholderQr().solve(b);

        //use least squre method to solve the linear system : Sp = b
        //Eigen::Vector3d p = S.bdcSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(b);
        //Eigen::Vector3d p = (S.transpose() * S).ldlt().solve(S.transpose() * b);
        MeshLib::CPoint intersection(p(0), p(1), p(2));
        return intersection;
    };

    /*
    *   Given two 3D lines, find the closest point between them
    *   return the middle point of the two closest points and the direction along the two closest points
    */
    void closest_point(MeshLib::CLine L1, MeshLib::CLine& L2, MeshLib::CPoint& mid_point, MeshLib::CPoint& normal)
    {
        MeshLib::CPoint p1 = L1.base();
        MeshLib::CPoint d1 = L1.direction();
        d1 /= d1.norm();

        MeshLib::CPoint p2 = L2.base();
        MeshLib::CPoint d2 = L2.direction();
        d2 /= d2.norm();

        normal = d1^d2;
        normal /= normal.norm();

        double c1 = (p1 - p2)* d1;
        double c2 = (p2 - p1)* d2;
        double c = d1 * d2;

        double t1 = 1 / (1 - c * c) * (-c1 - c * c2);
        double t2 = 1 / (1 - c * c) * (-c2 - c * c1);

        MeshLib::CPoint q1 = p1 + d1 * t1;
        MeshLib::CPoint q2 = p2 + d2 * t2;

        mid_point = (q1 + q2) / 2.0;

    }

    //compute the intersection point between a plane and a pline in R3

    bool intersect(MeshLib::CLine& L, MeshLib::CPlane& p, MeshLib::CPoint& pt)
    {
        MeshLib::CPoint n = p.normal();
        MeshLib::CPoint b = p.base();

        MeshLib::CPoint D = L.direction();
        MeshLib::CPoint B = L.base();

        double dn = D * n;
        if (fabs(dn) < 1e-10) {
            std::cerr << "Plane and line are parallel" << std::endl;
            return false;
        }
        double t = (b - B)* n / dn;

        pt = B + (D * t);
        if (fabs((pt - b) * n) > 1e-12) {
            std::cerr << "Error in line and plane intersection" << std::endl;
            return false;
        }
        return true;
    }

};

inline void test_lineregression_fit()
{
    CLineRegression LR;
    std::vector<MeshLib::CPoint> points;
    MeshLib::CPoint d(1, -1, 0.5);
    points.push_back(MeshLib::CPoint(0, 0, 0));
    points.push_back(d * 0.5);
    points.push_back(d * 2.0);
    MeshLib::CLine l = LR.fit(points);

    std::cout << "(" << l.base()[0] << "," << l.base()[1] << "," << l.base()[2] << ")";
    std::cout << "(" << l.direction()[0] << "," << l.direction()[1] << "," << l.direction()[2] << ")" << std::endl;
}

inline void test_lineregression_intersection()
{

    std::vector<MeshLib::CLine> lines;
    for (int i = 0; i < 50; i++)
    {
        MeshLib::CLine l;
        l.base() = MeshLib::CPoint(0.1, 0.3, -0.5);
        Eigen::Vector3d _d = Eigen::Vector3d::Random();
        MeshLib::CPoint d(_d(0), _d(1), _d(2));
        l.direction() = d / d.norm();
        lines.push_back(l);
    }
    CLineRegression LR;
    MeshLib::CPoint p = LR.intersect(lines);
    std::cout << "(" << p[0] << "," << p[1] << "," << p[2] << ")";
}



#endif

