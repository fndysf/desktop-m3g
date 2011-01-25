#include "m3g/IndexBuffer.hpp"
#include "m3g/Exception.hpp"
#include <iostream>
#include <cstring>
using namespace m3g;
using namespace std;


IndexBuffer:: IndexBuffer ()
{
}

IndexBuffer:: ~IndexBuffer ()
{
}

IndexBuffer* IndexBuffer:: duplicate () const
{
    return duplicate_xxx (NULL);
}

IndexBuffer* IndexBuffer:: duplicate_xxx (Object3D* obj) const
{
    IndexBuffer* ibuf = dynamic_cast<IndexBuffer*>(obj);
    if (ibuf == NULL) {
        ibuf = new IndexBuffer;
    }
    Object3D:: duplicate_xxx (ibuf);

    return ibuf;
}


int IndexBuffer:: getFaceCount () const
{
    return 0;
}

/**
 *
 */
int IndexBuffer:: getFaceVertexCount () const
{
    return 0;
}

void IndexBuffer:: getFaceVertexIndex (int face, int* vertex_indices) const
{
    if (face < 0 || face > getFaceCount()) {
        throw IndexOutOfBoundsException (__FILE__, __func__, "Face index is invalid. face=%d.", face);
    }
    return;
}


int IndexBuffer:: getIndexCount () const
{
    return 0;
}

void IndexBuffer:: getIndices (int*) const
{
}

/**
 * Note: Background should be rendered only at second rendering pass(pass=2).
 * In other cases, do nothing.
 */
void IndexBuffer:: render_xxx (RenderState& state) const
{
    // nothing to do.
}

std::ostream& IndexBuffer:: print (std::ostream& out) const
{
    out << "IndexBuffer: ";
    return out;
}

std::ostream& IndexBuffer:: print_raw_data (std::ostream& out) const
{
    print (out) << "\n";
    return out;
}


std::ostream& operator<< (std::ostream& out, const IndexBuffer& ibuf)
{
    return ibuf.print (out);
}


