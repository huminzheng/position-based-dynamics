#include "Constraint.h"
#include "Mesh.h"

Constraint::Constraint(const int& n0, const double& k0):
n(n0),
k(k0)
{
    
}

Constraint::~Constraint()
{
    
}

StretchingConstraint::StretchingConstraint(VertexIter v1, VertexIter v2,
                                           const double& k0):
Constraint(2, k0)
{
    vs = { v1, v2 };
    l = (v1->position-v2->position).norm();
}

StretchingConstraint::~StretchingConstraint()
{
    
}

void StretchingConstraint::solve()
{
    VertexIter& v1(vs[0]);
    VertexIter& v2(vs[1]);
    
    // compute and add correction
    Eigen::Vector3d u = v1->nPosition - v2->nPosition;
    double d = u.norm();
    u.normalize();
    
    double sum = v1->invMass + v2->invMass;
    if (sum < EPSILON) return;
        
    double lambda = k*(d-l) / sum;
    v1->nPosition -= v1->invMass*lambda*u;
    v2->nPosition += v2->invMass*lambda*u;
}

void StretchingConstraint::updateVelocity()
{
    // nothing to do
}

BendingConstraint::BendingConstraint(VertexIter v1, VertexIter v2,
                                     VertexIter v3, VertexIter v4,
                                     const double& k0):
Constraint(4, k0)
{
    vs = { v1, v2, v3, v4 };
    
    // compute dihedral angle
    Eigen::Vector3d n1 = (v1->position-v3->position).cross(v2->position-v3->position).normalized();
    Eigen::Vector3d n2 = (v2->position-v4->position).cross(v1->position-v4->position).normalized();
    double d = n1.dot(n2);
    if (d < -1.0) d = -1.0;
    else if (d >  1.0) d = 1.0;
    theta = acos(d);
}

BendingConstraint::~BendingConstraint()
{
    
}

void BendingConstraint::solve()
{
    VertexIter& v1(vs[0]);
    VertexIter& v2(vs[1]);
    VertexIter& v3(vs[2]);
    VertexIter& v4(vs[3]);
    
    // compute and add correction
    Eigen::Vector3d e = v2->nPosition-v1->nPosition;
    Eigen::Vector3d n1 = (v1->nPosition-v3->nPosition).cross(v2->nPosition-v3->nPosition);
    Eigen::Vector3d n2 = (v2->nPosition-v4->nPosition).cross(v1->nPosition-v4->nPosition);
    
    double de = e.norm();
    double dn1 = n1.norm(); n1 /= dn1;
    double dn2 = n2.norm(); n2 /= dn2;
    
    Eigen::Vector3d q1 = ((v3->nPosition - v2->nPosition).dot(e)*n1 / dn1 +
                          (v4->nPosition - v2->nPosition).dot(e)*n2 / dn2) / de;
    Eigen::Vector3d q2 = ((v1->nPosition - v3->nPosition).dot(e)*n1 / dn1 +
                          (v1->nPosition - v4->nPosition).dot(e)*n2 / dn2) / de;
    Eigen::Vector3d q3 = de * n1 / dn1;
    Eigen::Vector3d q4 = de * n2 / dn2;
    
    double sum = v1->invMass*q1.squaredNorm() +
                 v2->invMass*q2.squaredNorm() +
                 v3->invMass*q3.squaredNorm() +
                 v4->invMass*q4.squaredNorm();
    if (sum < EPSILON) return;
    
    double d = n1.dot(n2);
    if (d < -1.0) d = -1.0;
    else if (d >  1.0) d = 1.0;
    
    double lambda = k*(acos(d)-theta) / sum;
    if (n1.cross(n2).dot(e) > 0.0) lambda = -lambda;
    
    v1->nPosition -= v1->invMass*lambda*q1;
    v2->nPosition -= v2->invMass*lambda*q2;
    v3->nPosition -= v3->invMass*lambda*q3;
    v4->nPosition -= v4->invMass*lambda*q4;
}

void BendingConstraint::updateVelocity()
{
    // nothing to do
}

CollisionConstraint::CollisionConstraint(VertexIter v1, const Eigen::Vector3d& q0,
                                         const Eigen::Vector3d& normal0):
Constraint(1, 1.0),
normal(normal0),
q(q0)
{
    vs = { v1 };
}

CollisionConstraint::~CollisionConstraint()
{
    
}

void CollisionConstraint::solve()
{
    VertexIter& v1(vs[0]);
    
    // TODO: this is not an equality constraint
    
    // compute and add correction
    double lambda = normal.dot(v1->nPosition-q);
    v1->nPosition -= lambda*normal;
}

void CollisionConstraint::updateVelocity()
{
    // TODO: Do this only if constrained correction is performed
    VertexIter& v1(vs[0]);
    v1->velocity -= v1->velocity.dot(normal)*normal;
}