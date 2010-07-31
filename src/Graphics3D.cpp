#include <iostream>
#include <cmath>
#include "m3g-gl.hpp"
#include "Graphics3D.hpp"
#include "World.hpp"
#include "Image2D.hpp"
#include "Loader.hpp"
#include "Camera.hpp"
#include "RenderState.hpp"
#include "Exception.hpp"
using namespace m3g;
using namespace std;

const int Graphics3D:: ANTIALIAS;
const int Graphics3D:: DITHER;
const int Graphics3D:: OVERWRITE;
const int Graphics3D:: TRUE_COLOR;


Graphics3D:: Graphics3D () : 
    viewport(0,0,0,0), depth_buffer_enable(true), hints(0)
{
    // プロパティは決めうち
    properties.insert (map<const char*, int>::value_type("supportAntialiasing"         , 1));
    properties.insert (map<const char*, int>::value_type("supportTrueColor"            , 1));
    properties.insert (map<const char*, int>::value_type("supportDithering"            , 1));
    properties.insert (map<const char*, int>::value_type("supportMipmapping"           , 1));
    properties.insert (map<const char*, int>::value_type("supportPerspectiveCorrection", 1));
    properties.insert (map<const char*, int>::value_type("supportLocalCameraLighting"  , 1));
    properties.insert (map<const char*, int>::value_type("maxLights"                   , 8));
    properties.insert (map<const char*, int>::value_type("maxViewportWith"             , 2048));
    properties.insert (map<const char*, int>::value_type("maxViewportHeight"           , 2048));
    properties.insert (map<const char*, int>::value_type("maxViewportDimension"        , 2048));
    properties.insert (map<const char*, int>::value_type("maxTextureDimension"         , 2048));
    properties.insert (map<const char*, int>::value_type("maxSpriteCropDimension"      , 2048));
    properties.insert (map<const char*, int>::value_type("maxTransformsPerVertex"      , 65535)); // unlimited.
    properties.insert (map<const char*, int>::value_type("numTextureUnits"             , 4));
}

Graphics3D:: ~Graphics3D ()
{
}  

int Graphics3D:: addLight (Light* light, const Transform& transform)
{
    throw NotImplementedException (__FILE__, __func__, "Immediate mode is obsoleted, Please use retained mode.");
    return 0;
}

void Graphics3D:: initOpenGL ()
{
}

void Graphics3D:: bindTarget (void* target, bool depth_buffer_enable, int hint)
{
    throw NotImplementedException (__FILE__, __func__, "Sorry, bindTarget() is not implemented.");
}

void Graphics3D:: clear (Background* bg)
{
    throw NotImplementedException (__FILE__, __func__, "Immediate mode is obsoleted, Please use retained mode.");
}

Camera* Graphics3D:: getCamera (Transform* transform)
{
    throw NotImplementedException (__FILE__, __func__, "Immediate mode is obsoleted, Please use retained mode.");
    return 0;
}

float Graphics3D:: getDepthRangeFar () const
{
    throw NotImplementedException (__FILE__, __func__, "Immediate mode is obsoleted, Please use retained mode.");
    return 0;
}

float Graphics3D:: getDepthRangeNear () const
{
    throw NotImplementedException (__FILE__, __func__, "Immediate mode is obsoleted, Please use retained mode.");
    return 0;
}

int Graphics3D:: getHints () const
{
    return hints;
}

Graphics3D* Graphics3D:: getInstance ()
{
    static Graphics3D* g3d = new Graphics3D;
    return g3d;
}

Light* Graphics3D:: getLight (int index, Transform* transform) const
{
    throw NotImplementedException (__FILE__, __func__, "Immediate mode is obsoleted, Please use retained mode.");
    return 0;
}

int Graphics3D:: getLightCount () const
{
    throw NotImplementedException (__FILE__, __func__, "Immediate mode is obsoleted, Please use retained mode.");
    return 0;
}

std::map<const char*, int> Graphics3D:: getProperties () const
{
    return properties;
}

void* Graphics3D:: getTarget () const
{
    throw NotImplementedException (__FILE__, __func__, "Sorry, getTarget() is not implemented.");
}

int Graphics3D:: getViewportHeight () const
{
    return viewport.height;
}

int Graphics3D:: getViewportWidth () const
{
    return viewport.width;
}

int Graphics3D:: getViewportX () const
{
    return viewport.x;
}

int Graphics3D:: getViewportY () const
{
    return viewport.y;
}

bool Graphics3D:: isDepthBufferEnabled () const
{
    return depth_buffer_enable;
}

void Graphics3D:: releaseTarget ()
{
    throw NotImplementedException (__FILE__, __func__, "Sorry, releaseTarget() is not implemented.");
}

void Graphics3D:: render (Node* node, const Transform& transform) const
{
    throw NotImplementedException (__FILE__, __func__, "Immediate mode is obsoleted, Please use retained mode.");
}

void Graphics3D:: render (VertexBuffer* vertices, IndexBuffer* submesh, Appearance* apperance, Transform& transform, int scope) const
{
    throw NotImplementedException (__FILE__, __func__, "Immediate mode is obsoleted, Please use retained mode.");
}

void Graphics3D:: render (World* wld) const
{
    if (wld == NULL) {
        throw NullPointerException (__FILE__, __func__, "World is NULL.");
    }
    Camera* cam = wld->getActiveCamera();
    if (cam == NULL) {
        throw IllegalStateException (__FILE__, __func__, "Active camera is NULL.");
    }
    Node* parent = cam->getGlobalParent();
    if (parent != wld) {
        throw IllegalStateException (__FILE__, __func__, "Activated camera must be under the World.");
    }

    //cout << "Graphics3D: レンダー " << wld->getChildCount() << " ノード\n";

    switch (depth_buffer_enable) {
    case true : glEnable  (GL_DEPTH_TEST); break;
    case false: glDisable (GL_DEPTH_TEST); break;
    }

    // M3Gの規格では何も言及がないが
    // リスケールしないとscaleした時に
    // 法線の長さが1でなくなるので。
    glEnable (GL_NORMALIZE);

    glMatrixMode (GL_TEXTURE);
    glLoadIdentity ();
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();


    RenderState state;

    // pass -1 for set up valid layers.
    // pass 0 for Background, Camera.
    // pass 1 for Lights.
    // pass 2 for Scene nodes.
    for (int i = -1; i < 3; i++) {
        state.pass = i;
        wld->render (state);
    }

}

void Graphics3D:: resetLights ()
{
    throw NotImplementedException (__FILE__, __func__, "Immediate mode is obsoleted, Please use retained mode.");
}

void Graphics3D:: setCamera (Camera* camera_, const Transform& transform)
{
    throw NotImplementedException (__FILE__, __func__, "Immediate mode is obsoleted, Please use retained mode.");
}

void Graphics3D:: setDepthRange (float near, float far)
{
    throw NotImplementedException (__FILE__, __func__, "Immediate mode is obsoleted, Please use retained mode.");
}

void Graphics3D:: setLight (int index, Light* light, const Transform& transform)
{
    throw NotImplementedException (__FILE__, __func__, "Immediate mode is obsoleted, Please use retained mode.");
}

void Graphics3D:: setViewport (int x, int y, int width, int height)
{
    viewport.x      = x;
    viewport.y      = y;
    viewport.width  = width;
    viewport.height = height;

    glViewport (viewport.x, viewport.y, viewport.width, viewport.height);
}


std::ostream& Graphics3D:: print (std::ostream& out) const
{
    out << "Grapphics3D: ";
    out << "  depth_buffer_enable=" << depth_buffer_enable;
    out << ", hints="  << hints;
    return out;
}

std::ostream& operator<< (std::ostream& out, const m3g::Graphics3D& g3d)
{
    return g3d.print (out);
}
