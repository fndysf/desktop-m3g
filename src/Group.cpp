#include "Group.hpp"
#include <iostream>
#include <algorithm>
#include "RayIntersection.hpp"
//#include "Sprite3D.hpp"
#include "Exception.hpp"
#include "World.hpp"
using namespace m3g;
using namespace std;


Group:: Group ()
{
}

Group:: ~Group ()
{
}

Group* Group:: duplicate () const
{
  Group* grp  = new Group (*this);
  Node*  node = Node::duplicate();
  *(Node*)grp = *node;
  delete node;

  for (int i = 0; i < (int)children.size(); i++) {
    grp->children[i] = this->children[i]->duplicate();
    grp->children[i]->setParent (grp);
  }
  return grp;
}


void Group:: addChild (Node* child)
{
  if (child == NULL) {
    throw NullPointerException (__FILE__, __func__, "Child is NULL.");
  }
  if (child == this) {
    throw IllegalArgumentException (__FILE__, __func__, "Can't add myself.");
  }
  if (dynamic_cast<World*>(child)) {
    throw IllegalArgumentException (__FILE__, __func__, "Child is invalid, World.");
  }
  if (child->getParent() != NULL && child->getParent() != this) {
    throw IllegalArgumentException (__FILE__, __func__, "Added child is already a member of other group.");
  }

  child->setParent (this);
  children.push_back (child);
}

int Group:: animate (int world_time)
{
  //cout << "Group: animate, time=" << world_time << "\n";

  Node:: animate (world_time);

  for (int i = 0; i < (int)children.size(); i++) {
    children[i]->animate (world_time);
  }

  return 0;
}

Node* Group:: getChild (int index) const
{
  if (index < 0 || index >= (int)children.size()) {
    throw IllegalArgumentException (__FILE__, __func__, "Chldren index is invalid, index=%d.", index);
  }

  return children[index];
}

int Group:: getChildCount () const
{
  return children.size();
}

bool Group:: pick (int scope, float ox, float oy, float oz, float dx, float dy, float dz, RayIntersection* ri) const
{
  throw NotImplementedException (__FILE__, __func__, "Sorry, pick is not implemented.");
  return false;
}

void Group:: removeChild (Node* child)
{
  std::vector<Node*>::iterator it;
  for (it = children.begin(); it < children.end(); it++) {
    if (*it == child) {
      children.erase (it);
      child->setParent (NULL);
      break;
    }
  }

  // SkinnedMeshの場合特殊な処理が必要。
  // 仕様書を見よ。
}

/**
 * Note: Group should be rendered via all rendering pass.
 */
void Group:: render (RenderState& state) const
{
  //cout << "Group: render\n";
  Node::render (state);

  for (int i = 0; i < (int)children.size(); i++) {
    glPushMatrix ();
    children[i]->render (state);
    glPopMatrix ();
  }


}
  


std::ostream& Group:: print (std::ostream& out) const
{
  out << "Group: " << children.size() << " nodes";
  //for (int i = 0; i < (int)children.size(); i++) {
  //  out << "  [" << i << "] : ";
  //  children[i]->print(out);
  //}
  return out;
}

std::ostream& operator<< (std::ostream& out, const Group& grp)
{
  return grp.print (out);
}


