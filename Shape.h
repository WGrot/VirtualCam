#pragma once
#include "Edge.h"
#include "Tris.h"
#include <vector>

class Shape
{
private:
	std::vector<Edge> edges;
    std::vector<Tris> tris_list;
public:
	Shape(std::vector<Edge> edges, std::vector<Tris> tris) : edges(edges), tris_list(tris) {}
	std::vector<Edge> getEdges() const { return edges; }
	std::vector<Tris> getTris_list() const { return tris_list; }
	void setEdges(std::vector<Edge> newEdges) { edges = newEdges; }
	void addEdge(Edge edge) { edges.push_back(edge); }
	void removeEdge(int index) { edges.erase(edges.begin() + index); }
	void setTris(std::vector<Tris> newTris) { tris_list = newTris; }
	void addTris(Tris tris) { tris_list.push_back(tris); }
	void removeTris(int index) { tris_list.erase(tris_list.begin() + index); }
};

