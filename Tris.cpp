#include "Tris.h"
#include <Eigen/Core>
#include <iostream>
#include <Eigen/Geometry>
#include<stdlib.h>
#include <vector>

bool Tris::trianglesIntersect(const Tris& t1, const Tris& t2)
{

    Eigen::Vector2f t1Vertices[] = { t1.getV1Projection(), t1.getV2Projection(), t1.getV3Projection() };
    Eigen::Vector2f t2Vertices[] = { t2.getV1Projection(), t2.getV2Projection(), t2.getV3Projection() };
    std::vector<Eigen::Vector2f> axes;

    for (int i = 0; i < 3; ++i) {
        Eigen::Vector2f p1 = t1Vertices[i];
        Eigen::Vector2f p2 = t1Vertices[(i + 1) % 3]; 
        Eigen::Vector2f edge = p2 - p1;

        Eigen::Vector2f normal(-edge.y(), edge.x());

        axes.push_back(normal);
    }

    for (int i = 0; i < 3; ++i) {
        Eigen::Vector2f p1 = t2Vertices[i];
        Eigen::Vector2f p2 = t2Vertices[(i + 1) % 3];
        Eigen::Vector2f edge = p2 - p1;
        Eigen::Vector2f normal(-edge.y(), edge.x());
        axes.push_back(normal);
    }


    for (const auto& axis : axes) {

        if (axis.squaredNorm() < 1e-8) { 
            continue;
        }

        Tris::Projection proj1 = projectTriangle(axis, t1);
        Tris::Projection proj2 = projectTriangle(axis, t2);


        if (!projectionsOverlap(proj1, proj2)) {
            return false; 
        }
    }

    return true;
}

bool Tris::projectionsOverlap(const Projection& proj1, const Projection& proj2)
{

    return !(proj1.max < proj2.min || proj2.max < proj1.min);
}

Tris::Projection Tris::projectTriangle(const Eigen::Vector2f& axis, const Tris& triangle)
{
    Tris::Projection proj;
    Eigen::Vector2f vertices[] = {
        triangle.getV1Projection(),
        triangle.getV2Projection(),
        triangle.getV3Projection()
    };

    for (const auto& vertex : vertices) {
        float dotProduct = axis.dot(vertex);
        proj.min = std::min(proj.min, dotProduct);
        proj.max = std::max(proj.max, dotProduct);
    }
    return proj;
}
