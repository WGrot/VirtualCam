#pragma once
#include <Eigen/Core>

struct Edge
{
private:
    Eigen::Vector4f start;
    Eigen::Vector4f end;
public:
    Edge(Eigen::Vector4f start, Eigen::Vector4f end) : start(start), end(end) {}
    Eigen::Vector4f getStart() const { return start; }
    Eigen::Vector4f getEnd() const { return end; }
    void setStart(Eigen::Vector4f newStart) { start = newStart; }
    void setEnd(Eigen::Vector4f newEnd) { end = newEnd; }
};
