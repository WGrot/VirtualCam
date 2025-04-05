#pragma once
#include "Edge.h"
#include <vector>

class Shape
{
private:
	std::vector<Edge> edges;
public:
	Shape(std::vector<Edge> edges) : edges(edges) {}
	std::vector<Edge> getEdges() const { return edges; }
	void setEdges(std::vector<Edge> newEdges) { edges = newEdges; }
	void addEdge(Edge edge) { edges.push_back(edge); }
	void removeEdge(int index) { edges.erase(edges.begin() + index); }
	static Shape createCubeEdges() {
        std::vector<Edge> edges;
        // Definiowanie wierzcho³ków szeœcianu
        Eigen::Vector4f v0(-1, -1, -1, 1);
        Eigen::Vector4f v1(1, -1, -1, 1);
        Eigen::Vector4f v2(1, 1, -1, 1);
        Eigen::Vector4f v3(-1, 1, -1, 1);
        Eigen::Vector4f v4(-1, -1, 1, 1);
        Eigen::Vector4f v5(1, -1, 1, 1);
        Eigen::Vector4f v6(1, 1, 1, 1);
        Eigen::Vector4f v7(-1, 1, 1, 1);

        // Definiowanie krawêdzi szeœcianu
        edges.push_back(Edge(v0, v1));
        edges.push_back(Edge(v1, v2));
        edges.push_back(Edge(v2, v3));
        edges.push_back(Edge(v3, v0));
        edges.push_back(Edge(v4, v5));
        edges.push_back(Edge(v5, v6));
        edges.push_back(Edge(v6, v7));
        edges.push_back(Edge(v7, v4));
        edges.push_back(Edge(v0, v4));
        edges.push_back(Edge(v1, v5));
        edges.push_back(Edge(v2, v6));
        edges.push_back(Edge(v3, v7));

        return Shape(createCubeEdges());
    }
};

