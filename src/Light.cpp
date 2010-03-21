#include "Light.hpp"
#include <iostream>
#include "Exception.hpp"
#include "AnimationTrack.hpp"
#include "AnimationController.hpp"
#include "KeyframeSequence.hpp"
using namespace std;
using namespace m3g;

int Light:: gl_light_index = 0;

Light:: Light () :
  mode(DIRECTIONAL), color(0x00ffffff), intensity(1),
  attenuation(1,0,0), spot(45,0)
{
  setObjectType (OBJTYPE_LIGHT);
}

Light:: ~Light ()
{
}

void Light:: addAnimationTrack (AnimationTrack* animation_track)
{
  if (animation_track == NULL) {
    throw NullPointException (__FILE__, __func__, "Animation track is NULL.");
  }
  int property = animation_track->getTargetProperty();
  if (property != AnimationTrack::COLOR         &&
      property != AnimationTrack::INTENSITY     &&
      property != AnimationTrack::SPOT_ANGLE    &&
      property != AnimationTrack::SPOT_EXPONENT &&
      property != AnimationTrack::ALPHA         &&
      property != AnimationTrack::PICKABILITY   &&
      property != AnimationTrack::VISIBILITY    &&
      property != AnimationTrack::ORIENTATION   &&
      property != AnimationTrack::SCALE         &&
      property != AnimationTrack::TRANSLATION) {
    throw IllegalArgumentException (__FILE__, __func__, "Animation target is invalid for Light, property=%d.", property);
  }
 
  Object3D:: addAnimationTrack (animation_track);
}

int Light:: animate (int world_time)
{
  Object3D:: animate (world_time);

  bool  is_color_modefied = false;
  bool  is_intensity_modefied = false;
  bool  is_spot_angle_modefied = false;
  bool  is_spot_exponent_modefied = false;
  float rgb[] = {0,0,0};
  float new_intensity;
  Spot new_spot = Spot(0,0);
  
  for (int i = 0; i < getAnimationTrackCount(); i++) {
    AnimationTrack*      track      = getAnimationTrack (i);
    KeyframeSequence*    keyframe   = track->getKeyframeSequence();
    AnimationController* controller = track->getController();
    if (controller == NULL) {
      //cout << "Node: missing controller, this animation track is ignored.\n";
      continue;
    }
    if (!controller->isActiveInterval(world_time)) {
      continue;
    }
    float weight     = controller->getWeight ();
    float local_time = controller->getPosition (world_time);
    
    switch (track->getTargetProperty()) {
    case AnimationTrack::COLOR: {
      float value[3] = {1,1,1};
      keyframe->getFrame (local_time, value);
      rgb[0] += value[0] * weight;
      rgb[1] += value[1] * weight;
      rgb[2] += value[2] * weight;
      is_color_modefied = true;
      //cout << "VertexBuffer: default color --> " << rgb[0] << ", " << rgb[1] << ", " << rgb[2] << "\n";
      break;
    }
    case AnimationTrack::INTENSITY: {
      float value[1] = {1};
      keyframe->getFrame (local_time, value);
      new_intensity += value[0] * weight;
      is_intensity_modefied = true;
      //cout << "Light: intensity --> " << new_intensity << "\n";
      break;
    }
    case AnimationTrack::SPOT_ANGLE: {
      float value[1] = {1};
      keyframe->getFrame (local_time, value);
      new_spot.angle += value[0] * weight;
      is_spot_angle_modefied = true;
      //cout << "Light: spot_angle --> " << new_spot.angle << "\n";
      break;
    }
    case AnimationTrack::SPOT_EXPONENT: {
      float value[1] = {1};
      keyframe->getFrame (local_time, value);
      new_spot.exponent += value[0] * weight;
      is_spot_exponent_modefied = true;
      //cout << "Light: spot_angle --> " << new_spot.exponent << "\n";
      break;
    }
    default: {
      // Unknwon target should be ignored.
      // animate() of Base class (of Derived class) retrieve it.
    }
    }
  }

  if (is_color_modefied) {
      unsigned char r = (rgb[0] <= 0) ? 0 : 
	(rgb[0] >= 1) ? 255 :
	(unsigned char)(rgb[0]*255);
      unsigned char g = (rgb[1] <= 0) ? 0 : 
	(rgb[1] >= 1) ? 255 : 
	(unsigned char)(rgb[1]*255);
      unsigned char b = (rgb[2] <= 0) ? 0 : 
	(rgb[2] >= 1) ? 255 : 
	(unsigned char)(rgb[2]*255);
      //cout << "Light: r,g,b = " << (int)r << ", " << (int)g << ", " << (int)b << "\n";
      color &= 0xff000000;
      color |= (r << 16) | (g << 8) | (b << 0);
  }
  if (is_intensity_modefied) {
    intensity = new_intensity;
  }
  if (is_spot_angle_modefied) {
    spot.angle = new_spot.angle;
  }
  if (is_spot_exponent_modefied) {
    spot.exponent = new_spot.exponent;
  }

  //cout << *this << "\n";

  return 0;

}


int Light:: getColor () const
{
    return color;
}

float Light:: getConstantAttenuation () const
{
    return attenuation.constant;
}

float Light:: getIntensity () const
{
    return intensity;
}

float Light:: getLinearAttenuation () const
{
    return attenuation.linear;
}

int Light:: getMode () const
{
    return mode;
}

float Light:: getQuadraticAttenuation () const
{
    return attenuation.quadratic;
}

float Light:: getSpotAngle () const
{
    return spot.angle;
}

float Light:: getSpotExponent () const
{
    return spot.exponent;
}

void Light:: setAttenuation (float constant, float linear, float quadratic)
{
  if (constant < 0 || linear < 0 || quadratic < 0) {
    throw IllegalArgumentException (__FILE__, __func__, "Attenuation parameters is invalid, constant=%f, linear=%f, quadratic=%f.", constant, linear, quadratic);
  }
  if (constant == 0 && linear == 0 && quadratic == 0) {
    throw IllegalArgumentException (__FILE__, __func__, "Attenuation parameters is invalid, constant=%f, linear=%f, quadratic=%f.", constant, linear, quadratic);
  }
  attenuation.constant  = constant;
  attenuation.linear    = linear;
  attenuation.quadratic = quadratic;
}

void Light:: setColor (int rgb)
{
  color = rgb & 0x00ffffff;
}

void Light:: setIntensity (float i)
{
  intensity = i;
}

void Light:: setMode (int mod)
{
  if (mod != AMBIENT && mod != DIRECTIONAL && mod != OMNI && mod != SPOT) {
    throw IllegalArgumentException (__FILE__, __func__, "Mode is invalid, mode=%d.", mod);
  }
  mode = mod;
}

void Light:: setSpotAngle (float angle)
{
  if (angle < 0 || angle > 90) {
    throw IllegalArgumentException (__FILE__, __func__, "Spot angle is invalid, angle=%f.", angle);
  }
  spot.angle = angle;
}

void Light:: setSpotExponent (float exponent)
{
  if (exponent < 0 || exponent > 128) {
    throw IllegalArgumentException (__FILE__, __func__, "Spot exponent is invalid, exponent=%f.", exponent);
  }
  spot.exponent = exponent;
}


void Light:: resetGLIndex ()
{
  gl_light_index = 0;
}

int Light:: getGLIndex ()
{
  return gl_light_index++;
}

/**
 * Note: Light should be rendered only at first rendering pass(pass=1).
 * In other cases, do nothing.
 */
void Light:: render (int pass, int index_do_not_use) const
{
  if (pass != 1) {
    return;
  }

  //cout << "Light: render ================\n";
  
int index = getGLIndex ();

  GLfloat black[4] = {0,0,0,1};
  GLfloat argb[4];
  argb[3] = intensity * ((color & 0xff000000) >> 24) / 255.f;
  argb[0] = intensity * ((color & 0x00ff0000) >> 16) / 255.f;
  argb[1] = intensity * ((color & 0x0000ff00) >>  8) / 255.f;
  argb[2] = intensity * ((color & 0x000000ff) >>  0) / 255.f;
  
  Transform trans;
  getCompositeTransform (&trans);
  float m[16];
  trans.get (m);

  //cout << "Light is enabled\n";
  glEnable  (GL_LIGHTING);
  glEnable  (GL_LIGHT0+index);


  if (mode == AMBIENT) {
    //cout << "AMBIENT light\n";
    glLightfv (GL_LIGHT0+index, GL_AMBIENT,  argb);
    glLightfv (GL_LIGHT0+index, GL_DIFFUSE,  black);
    glLightfv (GL_LIGHT0+index, GL_SPECULAR, black);
    glLightf  (GL_LIGHT0+index, GL_SPOT_CUTOFF, 180.0f);
  }
  else if (mode == DIRECTIONAL) {
    //cout << "DIRECTIOANL light\n";
    GLfloat xyzw[4] = {m[2], m[6], m[10], 0};
    glLightfv (GL_LIGHT0+index, GL_POSITION, xyzw);
    glLightfv (GL_LIGHT0+index, GL_AMBIENT,  black);
    glLightfv (GL_LIGHT0+index, GL_DIFFUSE,  argb);
    glLightfv (GL_LIGHT0+index, GL_SPECULAR, argb);
    glLightf  (GL_LIGHT0+index, GL_SPOT_CUTOFF, 180.0f);
  }
  else if (mode == OMNI) {
    GLfloat xyzw[4] = {m[3], m[7], m[11], 1.0f};
    //cout << "OMNI light: xyzw = " << xyzw[0] << ", "<< xyzw[1] << ", "<< xyzw[2] << ", "<< xyzw[3] << "\n";
    //cout << "OMNI light: color = " << argb[0] << ", "<< argb[1] << ", "<< argb[2] << ", "<< argb[3] << "\n";
    //cout << "index = " << index << "\n";
    glLightfv (GL_LIGHT0+index, GL_POSITION, xyzw);
    glLightfv (GL_LIGHT0+index, GL_AMBIENT,  black);
    glLightfv (GL_LIGHT0+index, GL_DIFFUSE,  argb);
    glLightfv (GL_LIGHT0+index, GL_SPECULAR, argb);
    glLightf  (GL_LIGHT0+index, GL_SPOT_CUTOFF, 180.f);
  }
  else if (mode == SPOT) {
    //cout << "-------------------------SPOT light----------\n";
    GLfloat pos[4] = {m[3], m[7], m[11], 1};
    GLfloat dir[4] = {-m[2], -m[6], -m[10], 0};
    //cout << "dir = " << -m[2] << ", " << -m[6] << ", " << -m[10] << "\n";
    glLightfv (GL_LIGHT0+index, GL_POSITION, pos);
    glLightfv (GL_LIGHT0+index, GL_AMBIENT,  black);
    glLightfv (GL_LIGHT0+index, GL_DIFFUSE,  argb);
    glLightfv (GL_LIGHT0+index, GL_SPECULAR, argb);
    glLightfv (GL_LIGHT0+index, GL_SPOT_DIRECTION, dir);
    glLightf  (GL_LIGHT0+index, GL_SPOT_EXPONENT, spot.exponent);
    glLightf  (GL_LIGHT0+index, GL_SPOT_CUTOFF,   spot.angle);
  }
  else {
    throw IllegalArgumentException (__FILE__, __func__, "Invalid light mode, mode=%d.", mode);
  }

  glLightf (GL_LIGHT0+index, GL_CONSTANT_ATTENUATION,  attenuation.constant);
  glLightf (GL_LIGHT0+index, GL_LINEAR_ATTENUATION,    attenuation.linear);
  glLightf (GL_LIGHT0+index, GL_QUADRATIC_ATTENUATION, attenuation.quadratic);

  glEnable  (GL_LIGHTING);
  glEnable  (GL_LIGHT0+index);

}
static
const char* mode_to_string (int mode)
{
  switch (mode) {
  case Light::AMBIENT: return "AMBIENT";
  case Light::DIRECTIONAL: return "DIRECTIONAL";
  case Light::OMNI: return "OMNI";
  case Light::SPOT: return "SPOT";
  default: return "Unknown";
  }
}

std::ostream& Light:: print (std::ostream& out) const
{
  out << "Light: mode=" << mode_to_string(mode) << ", color=0x" << hex << color << dec;
  out << ", intensity=" << intensity << ", attenuation=" << attenuation.constant;
  out << "," << attenuation.linear << "," << attenuation.quadratic << ", spot=";
  out << spot.angle << "," << spot.exponent << "\n";
  return out;
}

std::ostream& operator<< (std::ostream& out, const Light& lig)
{
  return lig.print(out);
}
