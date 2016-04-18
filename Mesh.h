#ifndef MESH_H
#define MESH_H

#include "Types.h"
#include "Vertex.h"
#include "Edge.h"
#include "Face.h"
#include "HalfEdge.h"
#include "Bvh.h"

class Mesh {
public:
    // default constructor
    Mesh();
            
    // read mesh from file
    bool read(const std::string& fileName);
    
    // write mesh to file
    bool write(const std::string& fileName) const;
    
    // center mesh about origin and rescale to unit radius
    void normalize(const double& yshift, const double& scale);
    
    // member variables
    std::vector<HalfEdge> halfEdges;
    std::vector<Vertex> vertices;
    std::vector<Eigen::Vector3d> uvs;
    std::vector<Eigen::Vector3d> normals;
    std::vector<Edge> edges;
    std::vector<Face> faces;
    std::vector<HalfEdgeIter> boundaries;
    Bvh bvh;
    double thickness;
    bool isRigid;
};

#endif