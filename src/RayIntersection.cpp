#include "RayIntersection.hpp"
#include "Exception.hpp"
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "Mesh.hpp"
#include "Node.hpp"
#include <iostream>
#include <typeinfo>
#include "Node.hpp"
using namespace std;
using namespace m3g;

RayIntersection:: RayIntersection () :
    ray(Vector(0,0,0),Vector(0,0,0),0), node(0), u(0), v(0), submesh_index(0)
{
    //throw NotImplementedException (__FILE__, __func__, "Sorry, not implemented yet.");
}

RayIntersection:: RayIntersection (Node* node_,
                                   const Vector& org, const Vector& dir, float t,
                                   float u, float v,
                                   int vertex_num, int* vertex_indices,
                                   int submesh_index_) :
    ray(Vector(0,0,0),Vector(0,0,0),0), node(0), u(0), v(0), submesh_index(0)
{
    ray.org = org;
    ray.dir = dir;
    ray.t   = t;
    node    = node_;
    submesh_index = submesh_index_;

    vertices.reserve (vertex_num);
    for (int i = 0; i < vertex_num; i++) {
        vertices.push_back (vertex_indices[i]);
    }
}


RayIntersection:: ~RayIntersection ()
{
}

float RayIntersection:: getDistance () const
{
    return ray.t;
}

Node* RayIntersection:: getIntersected () const
{
    return node;
}

float RayIntersection:: getNormalX () const
{
    Vector normal;
    Mesh* mesh = dynamic_cast<Mesh*>(node);
    if (mesh) {
        float  scale_bias[4];
        float& scale = scale_bias[0];
        float* bias  = &scale_bias[1];
        VertexArray* normals = mesh->getVertexBuffer()->getNormals(scale_bias);
        if (normals) {
            float normal_values[3][3];
            normals->get (vertices[0], 1, scale, bias, normal_values[0]);
            normals->get (vertices[1], 1, scale, bias, normal_values[1]);
            normals->get (vertices[2], 1, scale, bias, normal_values[2]);
            Vector n0 = Vector(normal_values[0]);
            Vector n1 = Vector(normal_values[1]);
            Vector n2 = Vector(normal_values[2]);
            //cout << "n0 = " << n0 << "\n";
            //cout << "n1 = " << n1 << "\n";
            //cout << "n2 = " << n2 << "\n";
            normal = lerp (u, v, n0, n1, n2);
            if (normal.length() > 0)
                normal.normalize();
            //cout << "normal = " << normal << "\n";
        }
    }
    return normal.x;
}

float RayIntersection:: getNormalY () const
{
    Vector normal;
    throw NotImplementedException (__FILE__, __func__, "getNormalY is not implemtened, yet\n");
    return normal.y;
}

float RayIntersection:: getNormalZ () const
{
    Vector normal;
    throw NotImplementedException (__FILE__, __func__, "getNormalY is not implemtened, yet\n");
    return normal.z;
}

void RayIntersection:: getRay (float* ray_) const
{
    if (ray_ == NULL) {
        throw NullPointerException (__FILE__, __func__, "Ray is NULL.");
    }
    ray_[0] = ray.org.x;
    ray_[1] = ray.org.y;
    ray_[2] = ray.org.z;
    ray_[3] = ray.dir.x;
    ray_[4] = ray.dir.y;
    ray_[5] = ray.dir.z;
}

int RayIntersection:: getSubmeshIndex () const
{
    return submesh_index;
}

float RayIntersection:: getTextureS (int index) const
{
    Vector tex_coord;
    Mesh* mesh = dynamic_cast<Mesh*>(node);
    if (mesh) {
        float scale_bias[4];
        VertexArray* tex_coords = mesh->getVertexBuffer()->getTexCoords (index, scale_bias);
        if (tex_coords) {
            float tex_coord_values[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
            tex_coords->get (vertices[0], 1, tex_coord_values[0]);
            tex_coords->get (vertices[1], 1, tex_coord_values[1]);
            tex_coords->get (vertices[2], 1, tex_coord_values[2]);
            Vector tex_coord0 = Vector(tex_coord_values[0]);
            Vector tex_coord1 = Vector(tex_coord_values[1]);
            Vector tex_coord2 = Vector(tex_coord_values[2]);
            tex_coord = lerp (u, v, tex_coord0, tex_coord1, tex_coord2);
        }
    }
    return tex_coord.x;
}

float RayIntersection:: getTextureT (int index) const
{
    return 0;
}

void RayIntersection:: transformRay (const Transform& trans)
{
    ray.org = trans.transform (ray.org);
    ray.dir.w = 0;
    ray.dir = trans.transform (ray.dir);
    ray.dir.w = 1;
}

std::ostream& RayIntersection:: print (std::ostream& out) const
{
    out << "RayIntersection: " << (node ? "hit" : "not hit");
    out << ", ray.org=" << ray.org;
    out << ", ray.dir=" << ray.dir;
    out << ", ray.t=" << ray.t;
    out << ", node=" << (node ? typeid(*node).name() : "none");
    out << ", u=" << u << ",v=" << v;
    out << ", vertices=[";
    for (int i = 0; i < (int)vertices.size(); i++) {
        out << vertices[i] << ",";
    }
    out << "]";
    out << ", submesh_index=" << submesh_index;
    return out;
}

std::ostream& operator<< (std::ostream& out, const RayIntersection& ri)
{
    return ri.print (out);
}


