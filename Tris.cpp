#include "Tris.h"
#include <Eigen/Core>
#include <iostream>
#include <Eigen/Geometry>
#include<stdlib.h>
#include <vector>

bool Tris::trianglesIntersect(const Tris& t1, const Tris& t2)
{
    // Pobranie wierzcho�k�w rzutowanych
    Eigen::Vector2f t1Vertices[] = { t1.getV1Projection(), t1.getV2Projection(), t1.getV3Projection() };
    Eigen::Vector2f t2Vertices[] = { t2.getV1Projection(), t2.getV2Projection(), t2.getV3Projection() };
    std::vector<Eigen::Vector2f> axes;

    // Wygeneruj osie prostopad�e do kraw�dzi tr�jk�ta t1
    for (int i = 0; i < 3; ++i) {
        Eigen::Vector2f p1 = t1Vertices[i];
        Eigen::Vector2f p2 = t1Vertices[(i + 1) % 3]; // Nast�pny wierzcho�ek (zawijanie)
        Eigen::Vector2f edge = p2 - p1;
        // O� prostopad�a (normalna): obr�t wektora kraw�dzi o 90 stopni
        // (x, y) -> (-y, x) lub (y, -x)
        Eigen::Vector2f normal(-edge.y(), edge.x());
        // Nie musimy normalizowa� wektora osi dla SAT, ale je�li by�my chcieli:
        // normal.normalize();
        axes.push_back(normal);
    }

    // Wygeneruj osie prostopad�e do kraw�dzi tr�jk�ta t2
    for (int i = 0; i < 3; ++i) {
        Eigen::Vector2f p1 = t2Vertices[i];
        Eigen::Vector2f p2 = t2Vertices[(i + 1) % 3];
        Eigen::Vector2f edge = p2 - p1;
        Eigen::Vector2f normal(-edge.y(), edge.x());
        axes.push_back(normal);
    }

    // Sprawd� wszystkie 6 osi
    for (const auto& axis : axes) {
        // Sprawd�, czy o� nie jest wektorem zerowym (mo�e si� zdarzy� dla zdegenerowanych tr�jk�t�w)
        if (axis.squaredNorm() < 1e-8) { // Ma�y pr�g, aby unikn�� problem�w z precyzj�
            continue;
        }

        Tris::Projection proj1 = projectTriangle(axis, t1);
        Tris::Projection proj2 = projectTriangle(axis, t2);

        // Je�li na kt�rejkolwiek osi projekcje si� NIE pokrywaj�, tr�jk�ty si� nie przecinaj�
        if (!projectionsOverlap(proj1, proj2)) {
            return false; // Znaleziono o� separuj�c�!
        }
    }

    // Je�li na wszystkich osiach projekcje si� pokrywa�y, tr�jk�ty si� przecinaj�
    return true;
}
// Sprawdza, czy projekcje dw�ch tr�jk�t�w na danej osi si� pokrywaj�
bool Tris::projectionsOverlap(const Projection& proj1, const Projection& proj2)
{
    // Sprawdzamy, czy przedzia�y S� roz��czne. Je�li tak, zwracamy false.
    // Je�li nie s� roz��czne, to znaczy, �e si� pokrywaj�.
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
