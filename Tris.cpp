#include "Tris.h"
#include <Eigen/Core>
#include <iostream>
#include <Eigen/Geometry>
#include<stdlib.h>
#include <vector>

bool Tris::trianglesIntersect(const Tris& t1, const Tris& t2)
{
    // Pobranie wierzcho³ków rzutowanych
    Eigen::Vector2f t1Vertices[] = { t1.getV1Projection(), t1.getV2Projection(), t1.getV3Projection() };
    Eigen::Vector2f t2Vertices[] = { t2.getV1Projection(), t2.getV2Projection(), t2.getV3Projection() };
    std::vector<Eigen::Vector2f> axes;

    // Wygeneruj osie prostopad³e do krawêdzi trójk¹ta t1
    for (int i = 0; i < 3; ++i) {
        Eigen::Vector2f p1 = t1Vertices[i];
        Eigen::Vector2f p2 = t1Vertices[(i + 1) % 3]; // Nastêpny wierzcho³ek (zawijanie)
        Eigen::Vector2f edge = p2 - p1;
        // Oœ prostopad³a (normalna): obrót wektora krawêdzi o 90 stopni
        // (x, y) -> (-y, x) lub (y, -x)
        Eigen::Vector2f normal(-edge.y(), edge.x());
        // Nie musimy normalizowaæ wektora osi dla SAT, ale jeœli byœmy chcieli:
        // normal.normalize();
        axes.push_back(normal);
    }

    // Wygeneruj osie prostopad³e do krawêdzi trójk¹ta t2
    for (int i = 0; i < 3; ++i) {
        Eigen::Vector2f p1 = t2Vertices[i];
        Eigen::Vector2f p2 = t2Vertices[(i + 1) % 3];
        Eigen::Vector2f edge = p2 - p1;
        Eigen::Vector2f normal(-edge.y(), edge.x());
        axes.push_back(normal);
    }

    // SprawdŸ wszystkie 6 osi
    for (const auto& axis : axes) {
        // SprawdŸ, czy oœ nie jest wektorem zerowym (mo¿e siê zdarzyæ dla zdegenerowanych trójk¹tów)
        if (axis.squaredNorm() < 1e-8) { // Ma³y próg, aby unikn¹æ problemów z precyzj¹
            continue;
        }

        Tris::Projection proj1 = projectTriangle(axis, t1);
        Tris::Projection proj2 = projectTriangle(axis, t2);

        // Jeœli na którejkolwiek osi projekcje siê NIE pokrywaj¹, trójk¹ty siê nie przecinaj¹
        if (!projectionsOverlap(proj1, proj2)) {
            return false; // Znaleziono oœ separuj¹c¹!
        }
    }

    // Jeœli na wszystkich osiach projekcje siê pokrywa³y, trójk¹ty siê przecinaj¹
    return true;
}
// Sprawdza, czy projekcje dwóch trójk¹tów na danej osi siê pokrywaj¹
bool Tris::projectionsOverlap(const Projection& proj1, const Projection& proj2)
{
    // Sprawdzamy, czy przedzia³y S¥ roz³¹czne. Jeœli tak, zwracamy false.
    // Jeœli nie s¹ roz³¹czne, to znaczy, ¿e siê pokrywaj¹.
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
