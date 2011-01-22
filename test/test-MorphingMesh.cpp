#include <unittest++/UnitTest++.h>
#include <iostream>
#include "m3g/MorphingMesh.hpp"
#include "m3g/VertexArray.hpp"
#include "m3g/VertexBuffer.hpp"
#include "m3g/TriangleStripArray.hpp"
#include "m3g/Appearance.hpp"
#include "m3g/Group.hpp"
using namespace std;
using namespace m3g;

TEST (MorphingMesh_default_value)
{
    VertexArray*  varry       = new VertexArray (16, 3, 2);
    int           indices[]   = {0,1,2};
    int           strips[]    = {3};
    TriangleStripArray* tris  = new TriangleStripArray (indices, 1, strips);
    Appearance*   app         = new Appearance;

    float scale  = 1;
    float bias[] = {0,0,0};

    VertexBuffer* base_vertices = new VertexBuffer;
    base_vertices->setPositions (varry, scale, bias);
    VertexBuffer* target_vertices[2] = {base_vertices->duplicate (),
                                        base_vertices->duplicate ()};
    MorphingMesh*  mesh        = new MorphingMesh (base_vertices, 2, target_vertices, tris, app);

    CHECK_EQUAL (2, mesh->getMorphTargetCount());

    delete base_vertices;
    delete target_vertices[0];
    delete target_vertices[1];
    delete tris;
    delete app;
    delete mesh;
}


TEST (MorphingMesh_set_variable)
{
    VertexArray*  varry         = new VertexArray (16, 3, 2);
    int           indices[]   = {0,1,2};
    int           strips[]    = {3};
    TriangleStripArray* tris  = new TriangleStripArray (indices, 1, strips);
    Appearance*   app         = new Appearance;

    float scale  = 1;
    float bias[] = {0,0,0};

    VertexBuffer* base_vertices = new VertexBuffer;
    base_vertices->setPositions (varry, scale, bias);
    VertexBuffer* target_vertices[2] = {base_vertices->duplicate (),
                                        base_vertices->duplicate ()};
    MorphingMesh*  mesh        = new MorphingMesh (base_vertices, 2, target_vertices, tris, app);

    CHECK_EQUAL (2, mesh->getMorphTargetCount());
  
    float weights[3] = {1,2,998};
    mesh->setWeights(2, weights);

    float values[3] = {999,999,999};
    mesh->getWeights(values);
  
    CHECK_EQUAL (1.f,   values[0]);
    CHECK_EQUAL (2.f,   values[1]);
    CHECK_EQUAL (999.f, values[2]);

    delete base_vertices;
    delete target_vertices[0];
    delete target_vertices[1];
    delete tris;
    delete app;
    delete mesh;
}

TEST (MorphingMesh_duplicate)
{
    VertexArray*  varry         = new VertexArray (16, 3, 2);
    int           indices[]   = {0,1,2};
    int           strips[]    = {3};
    TriangleStripArray* tris  = new TriangleStripArray (indices, 1, strips);
    Appearance*   app         = new Appearance;

    float scale  = 1;
    float bias[] = {0,0,0};

    VertexBuffer* base_vertices = new VertexBuffer;
    base_vertices->setPositions (varry, scale, bias);
    VertexBuffer* target_vertices[2] = {base_vertices->duplicate (),
                                        base_vertices->duplicate ()};
    MorphingMesh*  mesh1        = new MorphingMesh (base_vertices, 2, target_vertices, tris, app);

    float weights[2] = {1,2};
    mesh1->setWeights(2, weights);

    MorphingMesh* mesh2 = mesh1->duplicate();

    CHECK_EQUAL (2, mesh2->getMorphTargetCount());

    float values[3] = {999,999};
    mesh2->getWeights(values);

    CHECK_EQUAL (1.f,   values[0]);
    CHECK_EQUAL (2.f,   values[1]);


    delete base_vertices;
    delete target_vertices[0];
    delete target_vertices[1];
    delete tris;
    delete app;
    delete mesh1;
    delete mesh2;
}

TEST (MorphingMesh_getReferences)
{
    VertexArray*  varry       = new VertexArray (16, 3, 2);
    int           indices[]   = {0,1,2};
    int           strips[]    = {3};
    TriangleStripArray* tris  = new TriangleStripArray (indices, 1, strips);
    Appearance*   app         = new Appearance;

    float scale  = 1;
    float bias[] = {0,0,0};

    VertexBuffer* base_vertices = new VertexBuffer;
    base_vertices->setPositions (varry, scale, bias);
    VertexBuffer* target_vertices[2] = {base_vertices->duplicate (),
                                        base_vertices->duplicate ()};
    MorphingMesh*  mesh        = new MorphingMesh (base_vertices, 2, target_vertices, tris, app);

    int n;
    Object3D* objs[5];

    n = mesh->getReferences (objs);

    CHECK_EQUAL (5, n);
    CHECK_EQUAL (base_vertices     , objs[0]);
    CHECK_EQUAL (tris              , objs[1]);
    CHECK_EQUAL (app               , objs[2]);
    CHECK_EQUAL (target_vertices[0], objs[3]);
    CHECK_EQUAL (target_vertices[1], objs[4]);

    delete varry;
    delete base_vertices;
    delete target_vertices[0];
    delete target_vertices[1];
    delete tris;
    delete app;
    delete mesh;
}

TEST (MorphingMesh_find)
{
    VertexArray*  varry       = new VertexArray (16, 3, 2);
    int           indices[]   = {0,1,2};
    int           strips[]    = {3};
    TriangleStripArray* tris  = new TriangleStripArray (indices, 1, strips);
    Appearance*   app         = new Appearance;

    float scale  = 1;
    float bias[] = {0,0,0};

    VertexBuffer* base_vertices = new VertexBuffer;
    base_vertices->setPositions (varry, scale, bias);
    VertexBuffer* target_vertices[2] = {base_vertices->duplicate (),
                                        base_vertices->duplicate ()};
    MorphingMesh*  mesh        = new MorphingMesh (base_vertices, 2, target_vertices, tris, app);


    varry->setUserID (100);
    tris->setUserID  (101);
    app->setUserID   (102);
    base_vertices->setUserID      (103);
    target_vertices[0]->setUserID (104);
    target_vertices[1]->setUserID (105);
    mesh->setUserID (106);

    CHECK_EQUAL (varry             , mesh->find(100));
    CHECK_EQUAL (tris              , mesh->find(101));
    CHECK_EQUAL (app               , mesh->find(102));
    CHECK_EQUAL (base_vertices     , mesh->find(103));
    CHECK_EQUAL (target_vertices[0], mesh->find(104));
    CHECK_EQUAL (target_vertices[1], mesh->find(105));
    CHECK_EQUAL (mesh              , mesh->find(106));

    delete varry;
    delete base_vertices;
    delete target_vertices[0];
    delete target_vertices[1];
    delete tris;
    delete app;
    delete mesh;
}

