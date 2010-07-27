#include "m3g.hpp"
#include <GL/glut.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cmath>
using namespace std;
using namespace m3g;

std::vector<Object3D*> objs;
World* wld = 0;

void display(void)
{
    Graphics3D* g3d = Graphics3D::getInstance();
    g3d->render (wld);
    glutSwapBuffers();
}

void resize(int w, int h)
{
    Graphics3D* g3d = Graphics3D::getInstance();
    g3d->setViewport (0,0,w,h);
    Camera* cam = wld->getActiveCamera();
    cam->setPerspective (45, w/(float)h, 0.1, 100);
}

void quit ()
{
    for (int i = 0; i < (int)objs.size(); i++) {
        delete objs[i];
    }
    Graphics3D* g3d = Graphics3D::getInstance();
    delete g3d;

    exit (0);
}


int world_time = 0;

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 'q':
        quit();
        break;
    case ' ':
        cout << "main: time = " << world_time << "\n";
        wld->animate (world_time);
        world_time += 5;
    default:
        break;
    }
    glutPostRedisplay();
}


int main (int argc, char** argv)
{
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow (argv[0]);

    AnimationController* controller = new AnimationController;

    KeyframeSequence* keyframe_sequence1 = new KeyframeSequence (3, 3, KeyframeSequence::LINEAR);
    float keyframes1[3][3] = {{1,0,0}, {0,0,0}, {1,0,0}};
    keyframe_sequence1->setKeyframe (0, 0,   keyframes1[0]);
    keyframe_sequence1->setKeyframe (1, 100, keyframes1[1]);
    keyframe_sequence1->setKeyframe (2, 200, keyframes1[2]);
    keyframe_sequence1->setRepeatMode (KeyframeSequence::LOOP);
    keyframe_sequence1->setValidRange (0, 2);
    keyframe_sequence1->setDuration (200);

    KeyframeSequence* keyframe_sequence2 = new KeyframeSequence (3, 3, KeyframeSequence::LINEAR);
    float keyframes2[3][3] = {{-1,0,0}, {0,0,0}, {-1,0,0}};
    keyframe_sequence2->setKeyframe (0, 0,   keyframes2[0]);
    keyframe_sequence2->setKeyframe (1, 100, keyframes2[1]);
    keyframe_sequence2->setKeyframe (2, 200, keyframes2[2]);
    keyframe_sequence2->setRepeatMode (KeyframeSequence::LOOP);
    keyframe_sequence2->setValidRange (0, 2);
    keyframe_sequence2->setDuration (200);

    AnimationTrack* animation_track1 = new AnimationTrack (keyframe_sequence1, AnimationTrack::TRANSLATION);
    animation_track1->setController (controller);

    AnimationTrack* animation_track2 = new AnimationTrack (keyframe_sequence2, AnimationTrack::TRANSLATION);
    animation_track2->setController (controller);

    float scale = 1;
    float bias[3] = {0,0,0};

    short        pos1[] = {1,-1,0, 1,1,0, -1,-1,0, -1,1,0};
    VertexArray* positions1         = new VertexArray (4, 3, 2);
    positions1->set (0, 4, pos1);

    VertexBuffer* vertices1 = new VertexBuffer;
    vertices1->setPositions (positions1, scale, bias);

    short        pos2[] = {1,-1,-3, 1,1,-3, -1,-1,-3, -1,1,-3};
    VertexArray* positions2         = new VertexArray (4, 3, 2);
    positions2->set (0, 4, pos2);

    VertexBuffer* vertices2 = new VertexBuffer;
    vertices2->setPositions (positions2, scale, bias);

    int strips[1] = {4};
    TriangleStripArray* tris = new TriangleStripArray (0, 1, strips);

    Material* mat1 = new Material;
    mat1->setColor (Material::DIFFUSE, 0xcf4c4cb2);

    CompositingMode* cmode1 = new CompositingMode;
    cmode1->setBlending (CompositingMode::ALPHA);
    cmode1->setDepthTestEnable (false);
  
    Material* mat2 = new Material;
    mat2->setColor (Material::DIFFUSE, 0xcf4cb2b2);

    CompositingMode* cmode2 = new CompositingMode;
    cmode2->setBlending (CompositingMode::ALPHA);
    cmode2->setDepthTestEnable (false);

    Appearance* app1 = new Appearance;
    app1->setMaterial (mat1);
    app1->setCompositingMode (cmode1);

    Appearance* app2 = new Appearance;
    app2->setMaterial (mat2);
    app2->setCompositingMode (cmode2);

    Mesh* mesh1 = new Mesh (vertices1, tris, app1);
    mesh1->addAnimationTrack (animation_track1);
    mesh1->translate (1,0,0);

    Mesh* mesh2 = new Mesh (vertices2, tris, app2);
    mesh2->addAnimationTrack (animation_track2);
    mesh2->translate (-1,0,0);
  
    Group* grp = new Group;
    grp->translate (0,0,-3);
    grp->addChild(mesh1);
    grp->addChild(mesh2);


    Camera* cam = new Camera;
    cam->translate (0,0,5);

    wld = new World;
    wld->addChild (cam);
    wld->setActiveCamera (cam);
    wld->addChild (grp);

    cout << *wld << "\n";

    objs.push_back (controller);
    objs.push_back (keyframe_sequence1);
    objs.push_back (keyframe_sequence2);
    objs.push_back (animation_track1);
    objs.push_back (animation_track2);
    objs.push_back (positions1);
    objs.push_back (positions2);
    objs.push_back (vertices1);
    objs.push_back (vertices2);
    objs.push_back (tris);
    objs.push_back (mat1);
    objs.push_back (mat2);
    objs.push_back (cmode1);
    objs.push_back (cmode2);
    objs.push_back (app1);
    objs.push_back (app2);
    objs.push_back (mesh1);
    objs.push_back (mesh2);
    objs.push_back (grp);
    objs.push_back (cam);
    objs.push_back (wld);

    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutMainLoop ();

    return 0;
}
