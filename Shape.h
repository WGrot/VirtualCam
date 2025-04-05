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
	
};

