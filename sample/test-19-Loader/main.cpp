#include "m3g.hpp"
#include <GL/glut.h>
#include <iostream>
#include <cstring>
#include <cassert>
#include <cmath>
#include <cstdlib>
using namespace std;
using namespace m3g;
#include <typeinfo>

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
  Camera* cam = wld->getActiveCamera ();
  cam->setPerspective (45, w/(float)h, 0.1, 10000);
}


int animation_time = 0;

static void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case 'q':
    exit(0);
  case ' ':
    wld->animate (animation_time);
    animation_time += 33;
    cout << "animation time = " << animation_time << "\n";
  default:
    break;
  }
  glutPostRedisplay();
}

int main (int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutCreateWindow(argv[0]);
  glewInit ();

  //std::vector<Object3D*> objs = Loader::load ("blocklow.m3g");
  //std::vector<Object3D*> objs = Loader::load ("simple.m3g");
  //std::vector<Object3D*> objs = Loader::load ("test.m3g");
  std::vector<Object3D*> objs = Loader::load ("test.m3g");
  for (int i = 0; i < (int)objs.size(); i++) {
    Node* node = dynamic_cast<Node*>(objs[i]);
    if (node) {
      node->setAlphaFactor(1);
    }
    //if (objs[i])
    //  objs[i]->print (cout);
    Light* lig = dynamic_cast<Light*>(objs[i]);
    if (lig) {
      lig->print (cout);
      lig->setAttenuation (1,0,0);
      lig->setIntensity (1);
    }
  }

  for (int i = 0; i < (int)objs.size(); i++) {
    wld = dynamic_cast<World*>(objs[i]);
    if (wld) {
      cout << *wld << "\n";
      break;
    }
  }
  assert (wld != 0);

  Camera* cam = wld->getActiveCamera ();
  if (cam == 0) {
    cam = new Camera;
    cam->translate (0,20,80);
    wld->addChild (cam);
    wld->setActiveCamera (cam);
  }

  Background* bg = wld->getBackground ();
  if (bg == 0) {
    bg = new Background;
    wld->setBackground (bg);
  }
  bg->setColor (0xff3f7fff);

  glutKeyboardFunc(keyboard);
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutMainLoop ();

  return 0;
}
