#include <unittest++/UnitTest++.h>
#include <iostream>
#include "Mesh.hpp"
#include "VertexBuffer.hpp"
#include "TriangleStripArray.hpp"
#include "Appearance.hpp"
using namespace std;
using namespace m3g;


TEST (Mesh_default_variables_1)
{
  VertexBuffer* vbuf = new VertexBuffer;
  int indices[] = {0,1,2};
  int strips[] = {3};
  TriangleStripArray* tris[2] = {new TriangleStripArray (indices, 1, strips),
                                  new TriangleStripArray (indices, 1, strips)};
  Appearance*  app[2]  = {new Appearance, new Appearance};
  Mesh* mesh = new Mesh (vbuf, 2, (IndexBuffer**)tris, 2, app);

  CHECK_EQUAL (OBJTYPE_MESH, mesh->getObjectType());
  CHECK_EQUAL (vbuf,    mesh->getVertexBuffer());
  CHECK_EQUAL (2,       mesh->getSubmeshCount());
  CHECK_EQUAL (tris[0], mesh->getIndexBuffer(0));
  CHECK_EQUAL (tris[1], mesh->getIndexBuffer(1));
  CHECK_EQUAL (app[0],  mesh->getAppearance(0));
  CHECK_EQUAL (app[1],  mesh->getAppearance(1));

  Appearance* app2 = new Appearance;
  mesh->setAppearance (0, app2);
  CHECK_EQUAL (app2, mesh->getAppearance(0));

  delete vbuf;
  delete tris[0];
  delete tris[1];
  delete app[0];
  delete app[1];
  delete mesh;
}


TEST (Mesh_default_variables_2)
{
  VertexBuffer* vbuf = new VertexBuffer;
  int indices[] = {0,1,2};
  int strips[] = {3};
  TriangleStripArray* tris = new TriangleStripArray (indices, 1, strips);
  Appearance* app = new Appearance;
  Mesh* mesh = new Mesh (vbuf, tris, app);

  CHECK_EQUAL (vbuf, mesh->getVertexBuffer());
  CHECK_EQUAL (1,    mesh->getSubmeshCount());
  CHECK_EQUAL (tris, mesh->getIndexBuffer(0));
  CHECK_EQUAL (app,  mesh->getAppearance(0));


  Appearance* app2 = new Appearance;
  mesh->setAppearance (0, app2);
  CHECK_EQUAL (app2, mesh->getAppearance(0));

  delete vbuf;
  delete tris;
  delete app;
  delete mesh;
}

