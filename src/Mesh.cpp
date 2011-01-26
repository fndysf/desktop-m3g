#include "m3g/Mesh.hpp"
#include "m3g/VertexBuffer.hpp"
#include "m3g/VertexArray.hpp"
#include "m3g/IndexBuffer.hpp"
#include "m3g/Appearance.hpp"
#include "m3g/Exception.hpp"
#include "m3g/RayIntersection.hpp"
#include "m3g/RenderState.hpp"
#include "m3g/Intersect.hpp"
#include "m3g/Vector.hpp"
#include <iostream>
#include <cassert>
#include <limits>
#include <cstring>
using namespace std;
using namespace m3g;

Mesh:: Mesh (VertexBuffer* vertices_, int num_submesh,
             IndexBuffer** submeshes, Appearance** appearances_) : vertices(0)
{
    if (vertices_ == NULL) {
        throw NullPointerException (__FILE__, __func__, "VertexBuffer is NULL.");
    }
    if (num_submesh == 0) {
        throw IllegalArgumentException (__FILE__, __func__, "Number of submesh is invalid, num_submesh=%d.", num_submesh);
    }
    if (submeshes == NULL) {
        throw NullPointerException (__FILE__, __func__, "IndexBuffer is NULL.");
    }
    if (appearances_ == NULL) {
        throw NullPointerException (__FILE__, __func__, "Appearances is NULL.");
    }

    initialize (vertices_, num_submesh, submeshes, appearances_);
}

Mesh:: Mesh (VertexBuffer* vertices_, IndexBuffer* submesh, Appearance* appearance_)
{
    if (vertices_ == NULL) {
        throw NullPointerException (__FILE__, __func__, "VertexBuffer is NULL.");
    }
    if (submesh == NULL) {
        throw NullPointerException (__FILE__, __func__, "IndexBuffer is NULL.");
    }
    if (appearance_ == NULL) {
        throw NullPointerException (__FILE__, __func__, "Appearance is NULL.");
    }

    initialize (vertices_, 1, &submesh, &appearance_);
}


void Mesh:: initialize (VertexBuffer* vertices_, int num_submesh, IndexBuffer** submeshes, Appearance** appearances_)
{
    vertices = vertices_;

    indices.assign (submeshes, submeshes+num_submesh);
    appearances.assign (appearances_, appearances_+num_submesh);
}

Mesh:: ~Mesh ()
{
}

Mesh* Mesh:: duplicate () const
{
    return duplicate_xxx (NULL);
}

Mesh* Mesh:: duplicate_xxx (Object3D* obj) const
{
    Mesh* mesh = dynamic_cast<Mesh*>(obj);
    if (mesh == NULL) {
        mesh = new Mesh (vertices,
                         indices.size(),
                         (IndexBuffer**)&indices[0],
                         (Appearance**)&appearances[0]);        
    }
    Node:: duplicate_xxx (mesh);

    return mesh;
}




int Mesh:: getReferences_xxx (Object3D** references) const
{
    int n = Node:: getReferences_xxx (references);
    if (vertices)
        references ? references[n] = vertices, n++ : n++;
    for (int i = 0; i < (int)indices.size(); i++) {
        if (indices[i])
            references ? references[n] = indices[i], n++ : n++;
    }
    for (int i = 0; i < (int)appearances.size(); i++) {
        if (appearances[i])
            references ? references[n] = appearances[i], n++ : n++;
    }

    return n;  
}


int Mesh:: animate_xxx (int world_time)
{
    Node::animate_xxx (world_time);

    if (vertices) {
        vertices->animate (world_time);
    }

    for (int i = 0; i < (int)indices.size(); i++) {
        indices[i]->animate (world_time);
    }
    for (int i = 0; i < (int)appearances.size(); i++) {
        appearances[i]->animate (world_time);
    }

    return 0;
}


Appearance* Mesh:: getAppearance (int index) const
{
    if (index < 0 || index >= (int)appearances.size()) {
        throw IndexOutOfBoundsException (__FILE__, __func__, "Index is invalid, index=%d, max=%d.", index, appearances.size());
    }
    return appearances[index];
}

IndexBuffer* Mesh:: getIndexBuffer (int index) const
{
    if (index < 0 || index >= (int)indices.size()) {
        throw IndexOutOfBoundsException (__FILE__, __func__, "Index is invalid, index=%d, max=%d.", index, indices.size());
    }
    return indices[index];
}

int Mesh:: getSubmeshCount () const
{
    return indices.size();
}

VertexBuffer* Mesh:: getVertexBuffer () const
{
    return vertices;
}

void Mesh:: setAppearance (int index, Appearance* appearance)
{
    if (index < 0 || index >= (int)appearances.size()) {
        throw IndexOutOfBoundsException (__FILE__, __func__, "Index is invalid, index=%d, max=%d.", index, appearances.size());
    }
  
    appearances[index] = appearance;
}

bool Mesh:: intersect (const Vector& org, const Vector& dir, RayIntersection* ri_out) const
{
    if (org.w != 1 || dir.w != 1) {
        throw IllegalArgumentException (__FILE__, __func__, "W must be 1. org.w=%f, dir.w=%f.", org.w, dir.w);
    }
    if (ri_out == NULL) {
        throw NullPointerException (__FILE__, __func__, "RayIntersection is NULL.");
    }
    if (!isGlobalPickingEnabled()) {
        return false;
    }

    float  scale_bias[4];
    float& scale = scale_bias[0];
    float* bias  = &scale_bias[1];

    VertexArray* positions = vertices->getPositions(scale_bias);
    bool         ri_hit    = false;
    float        ri_u      = 0;
    float        ri_v      = 0;
    float        ri_t      = numeric_limits<float>::max();
    int          ri_index_values[3];
    int          ri_submesh_index = 0;

    for (int i = 0; i < (int)indices.size(); i++) {

        int type = indices[i]->getPrimitiveType ();
        if (type != IndexBuffer::TRIANGLES) {
            throw IllegalStateException (__FILE__, __func__, "Primitive type is invalid, type=%d\n", type);
        }

        int  num_vertices = indices[i]->getIndexCount();
        int* vertices     = new int [num_vertices];
        indices[i]->getIndices (vertices);

        float position_values[3][3];

        for (int i = 0; i < num_vertices; i+=3) {
            positions->get (vertices[i+0], 1, scale, bias, &position_values[0][0]);
            positions->get (vertices[i+1], 1, scale, bias, &position_values[1][0]);
            positions->get (vertices[i+2], 1, scale, bias, &position_values[2][0]);
            Vector v0 = Vector(position_values[0]);
            Vector v1 = Vector(position_values[1]);
            Vector v2 = Vector(position_values[2]);
            float u, v, t;
            bool hit = triangle_intersect (org, dir, v0, v1, v2, &u, &v, &t);
            if (hit && t < ri_t) {
                ri_hit = true;
                ri_u = u;
                ri_v = v;
                ri_t = t;
                ri_submesh_index = i;
                memcpy (ri_index_values, &vertices[i], sizeof(int)*3);
            }
        }

        delete [] vertices;

    }

    if (ri_hit) {
        //cout << "ri_hit = " << ri_hit << "\n";
        if (ri_out) {
            *ri_out = RayIntersection (const_cast<Mesh*>(this),
                                       org, dir, ri_t,
                                       ri_u, ri_v,
                                       3, ri_index_values,
                                       ri_submesh_index);
        }
        return true;
    }
        
    return false;
}


/**
 * Note: if 1st rendering pass (pass=1), add this mesh's rendering layer number.
 */
/*
 * Note: Mesh should be rendered only at second rendering pass(pass=2).
 *       In other cases, do nothing.
 */
void Mesh:: render_xxx (RenderState& state) const
{
    if (!isGlobalRenderingEnabled()) {
        return;
    }

    if (state.pass == -1) {
        for (int i = 0; i < (int)appearances.size(); i++) {
            state.valid_layers.push_back (appearances[i]->getLayer2());
        }
    }
    if (state.pass != 2) {
        return;
    }

    //cout << "Mesh: render " << indices.size() << "メッシュ\n";

    // 注意：下の３つは厳密にこの順番でなければならない。変更不可

    // ノードαの設定
    Node::render_xxx (state);

    // 頂点データの指定
    vertices->render (state);


    // マテリアルとインデックスの指定
    for (int i = 0; i < (int)appearances.size(); i++) {
        if (appearances[i] && appearances[i]->getLayer2() == state.layer) {
            appearances[i]->render (state);
            indices[i]->render (state);
        }
    }

}

std::ostream& Mesh:: print (std::ostream& out) const
{
    out << "Mesh: ";
    out << "  " << indices.size() << " submesh";
    VertexArray* vary = vertices->getPositions(0);
    if (vary)
        out << ",  " << vary->getVertexCount() << " vertices";
    else
        out << ",  0 vertices";
    out << ", node_alpha=" << getAlphaFactor();
    //for (int i = 0; i < (int)appearances.size(); i++) {
    //  out << *appearances[i];
    //}

    return out;
}


std::ostream& operator<< (std::ostream& out, const Mesh& mesh)
{
    return mesh.print (out);
}

