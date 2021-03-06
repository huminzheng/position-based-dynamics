#ifndef VERTEX_H
#define VERTEX_H

#include "Types.h"

class Vertex {
public:
    // constructor
    Vertex();
    
    // outgoing halfedge
    HalfEdgeIter he;
    
    // location in 3d
    Eigen::Vector3d nPosition;
    Eigen::Vector3d position;
    
    // velocity
    Eigen::Vector3d velocity;
    
    // mass
    double mass;
    
    // inverse mass
    double invMass;
    
    // id between 0 and |V|-1
    int index;
    
    // checks if vertex is contained in any edge or face
    bool isIsolated() const;
    
    // returns angle weighted vertex normal
    Eigen::Vector3d normal() const;
};

#endif
