#include "m3g/World.hpp"
#include "m3g/Camera.hpp"
#include "m3g/Background.hpp"
#include "m3g/Light.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>
#include "m3g/Exception.hpp"
#include "m3g/RenderState.hpp"
using namespace m3g;
using namespace std;
#include <iterator>


World:: World () :
    background(0), active_camera(0)
{
}

World:: ~World ()
{
}

World* World:: duplicate () const
{
    return duplicate_xxx (NULL);
}

World* World:: duplicate_xxx (Object3D* obj) const
{
    World* wld   = dynamic_cast<World*>(obj);
    if (wld == NULL) {
        wld = new World;
    }
    Group:: duplicate_xxx (wld);


    wld->background = background;
    if (active_camera) {
        wld->active_camera = dynamic_cast<Camera*>(active_camera->getDuplicatedNode());
    }

    return wld;
}


int World:: animate_xxx (int world_time)
{
    Group:: animate_xxx (world_time);

    if (background) {
        background->animate (world_time);
    }
    // shoud not call active_camera->animate ();

    return 0;
}


int World:: getReferences_xxx (Object3D** references) const
{
    int n = Group:: getReferences_xxx (references);
    if (background) {
        references ? references[n] = background, n++ : n++;
    }
    if (active_camera) {
        references ? references[n] = active_camera, n++ : n++;
    }


    return n;
}


Camera* World:: getActiveCamera () const
{
    return active_camera;
}

Background* World:: getBackground () const
{
    return background;
}

void World:: setActiveCamera (Camera* cam)
{
    if (cam == NULL) {
        throw NullPointerException (__FILE__, __func__, "Camera is NULL.");
    }
    active_camera = cam;
}

void World:: setBackground (Background* bg)
{
    if (bg == NULL) {
        throw NullPointerException (__FILE__, __func__, "Background is NULL.");
    }

    background = bg;
}

/**
 * Note: World should be rendered via all rendering pass.
 *   pass=-1: setup valid layers.
 *   pass=0: render background and camera.
 *   pass=1: render lights.
 *   pass=2: render objets.
 */
void World:: render_xxx (RenderState& state) const
{
    if (active_camera == NULL) {
        throw IllegalStateException (__FILE__, __func__, "Active camera is NULL.");
    }
    
    //cout << "World render\n";
    
    state.camera = active_camera;

    vector<int>& v = state.valid_layers;

    switch (state.pass) {
    case -1:
        // 使用するレイヤー番号の収集
        Group::render_xxx (state);
        sort (v.begin(), v.end());
        v.erase (unique(v.begin(), v.end()), v.end());
        break;
    case 0: {
        // バックグラウンドとカメラ
        if (background) {
            background->render (state);
        } else {
            Background:: renderX ();
        }
        active_camera->render (state);
        break;
    }
    case 1:
        // ライト
        state.light_index = 0;
        Group::render_xxx (state);
        break;
    case 2:
        // プリミティブ
        for (int i = 0; i < (int)state.valid_layers.size(); i++) {
            state.layer = state.valid_layers[i];
            Group::render_xxx (state);
        }
        break;
    default:
        throw IllegalStateException (__FILE__, __func__, "Render pass is invalid, pass=%d.", state.pass);
    }

}



std::ostream& World::print (std::ostream& out) const
{
    out << "World: \n";
    int index = -1;
    for (int i = 0; i < (int)children.size(); i++) {
        if (children[i] == active_camera) {
            index = i;
            break;
        }
    }
    if (index >= 0)
        out << "  active camera = [" << index << "]\n";
    else 
        out << "  active camera = NOT FOUND\n";
    if (background)
        out << "  background    = " << *background << "\n";
    else 
        out << "  background    = NOT FOUND\n";
    for (int i = 0; i < (int)children.size(); i++) {
        out << "  [" << i << "] : ";
        children[i]->print(out) << "\n";
    }

    return out;
}

std::ostream& operator<< (std::ostream& out, const m3g::World& wld)
{
    return wld.print(out);
}
