#include <unittest++/UnitTest++.h>
#include <iostream>
#include <cstring>
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
using namespace std;
using namespace m3g;


TEST (VertexBuffer_default_variables)
{
  VertexBuffer* vbuf = new VertexBuffer;

  CHECK_EQUAL (OBJTYPE_VERTEX_BUFFER, vbuf->getObjectType());
  CHECK_EQUAL (0xffffffff,      vbuf->getDefaultColor());
  CHECK_EQUAL ((VertexArray*)0, vbuf->getColors());
  CHECK_EQUAL ((VertexArray*)0, vbuf->getNormals());
  CHECK_EQUAL ((VertexArray*)0, vbuf->getPositions(0));
  CHECK_EQUAL ((VertexArray*)0, vbuf->getTexCoords(0, 0));
  CHECK_EQUAL ((VertexArray*)0, vbuf->getTexCoords(1, 0));
  
  delete vbuf;
}

TEST (VertexBuffer_set_variables)
{
    VertexBuffer* vbuf = new VertexBuffer;
    VertexArray* colors = new VertexArray (1, 3, 1);
    VertexArray* normals = new VertexArray (1, 3, 2);
    VertexArray* positions = new VertexArray (1, 3, 2);
    VertexArray* texcoords = new VertexArray (1, 2, 2);
    float bias[4] = {32768/65535.f, 2*32768/65535.f, 3*32768/65535.f};

    vbuf->setDefaultColor (0x12345678);
    vbuf->setColors (colors);
    vbuf->setNormals (normals);
    vbuf->setPositions (positions, 1/65535.f, bias);
    vbuf->setTexCoords (1, texcoords, 1/65535.f, bias);

    CHECK_EQUAL (0x12345678, vbuf->getDefaultColor());
    CHECK_EQUAL (colors, vbuf->getColors());
    CHECK_EQUAL (normals, vbuf->getNormals());
    CHECK_EQUAL (positions, vbuf->getPositions(0));
    CHECK_EQUAL (texcoords, vbuf->getTexCoords(1,0));
    CHECK_EQUAL ((VertexArray*)0, vbuf->getTexCoords(0,0));


    delete texcoords;
    delete positions;
    delete normals;
    delete colors;
    delete vbuf;
}
