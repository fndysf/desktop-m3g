#include <iostream>
#include <fstream>
#include "m3g.hpp"
#include "Loader.hpp"
using namespace std;
using namespace m3g;
#include <stdlib.h>

std::ifstream Loader::ifs;
std::vector<Object3D*> Loader::objs;


Loader:: Loader ()
{
}

Loader:: ~Loader ()
{
}


std::vector<Object3D*> Loader:: load (void* data, int offset)
{
  throw NotImplementedException (__FILE__, __func__, "Load from memory is not implemented.");
  return objs;
}

const char m3g_ident[12] = {0xAB,0x4A,0x53,0x52,0x31,0x38,0x34,0xBB,0x0D,0x0A,0x1A,0x0A};

std::vector<Object3D*> Loader:: load (const char* name)
{
  ifs.open (name);
  if (!ifs) {
    throw IOException (__FILE__, __func__, "Can't load %s.", name);
  }
  if (!getM3GIdent()) {
    throw IOException (__FILE__, __func__, "This is not M3G format file=%s.", name);
  }

  while (1) {
    // section
    //cout << "----------start section--------------\n";

    char         compression_scheme   = getByte();
    unsigned int total_section_length = getUInt32 ();
    unsigned int uncompressed_length  = getUInt32 ();
    //cout << "compression_scheme = "   << (int)compression_scheme << "\n";
    //cout << "total_section_length = " << total_section_length << "\n";
    //cout << "uncompressed_length = "  << uncompressed_length << "\n";

    if (ifs.eof())
      break;
    if (compression_scheme != 0) {
      throw NotImplementedException (__FILE__, __func__, "Compressed file is not implemtened.");
    }

    int end_of_objects = (int)ifs.tellg() + total_section_length - 13;

    // 注意：オブジェクトインデックスは１から始まる
    int i = 1;

    while (1) {
      // object
      //cout << "object -------- " << i++ << " --------\n";
      char         object_type = getByte ();
      unsigned int length      = getUInt32 ();
      //cout << "object type = " << objtype_to_string(object_type) << "(" << (int)object_type << ")\n";
      //cout << "length = "      << length << "\n";

      switch (object_type) {
      case OBJTYPE_HEADER_OBJECT       : parseHeader()             ; break;
      case OBJTYPE_ANIMATION_CONTROLLER: parseAnimationController(); break;
      case OBJTYPE_ANIMATION_TRACK     : parseAnimationTrack ()    ; break;
      case OBJTYPE_APPEARANCE          : parseAppearance ()        ; break;
      case OBJTYPE_BACKGROUND          : parseBackground ()        ; break;
      case OBJTYPE_CAMERA              : parseCamera ()            ; break;
      case OBJTYPE_COMPOSITING_MODE    : parseCompositingMode ()   ; break;
      case OBJTYPE_FOG                 : parseFog ()               ; break;
      case OBJTYPE_GROUP               : parseGroup ()             ; break;
      case OBJTYPE_IMAGE2D             : parseImage2D ()           ; break;
      case OBJTYPE_KEYFRAME_SEQUENCE   : parseKeyframeSequence ()  ; break;
      case OBJTYPE_LIGHT               : parseLight ()             ; break;
      case OBJTYPE_MATERIAL            : parseMaterial ()          ; break;
      case OBJTYPE_MESH                : parseMesh ()              ; break;
      case OBJTYPE_POLYGON_MODE        : parsePolygonMode ()       ; break;
      case OBJTYPE_SKINNED_MESH        : parseSkinnedMesh ()       ; break;
      case OBJTYPE_SPRITE3D            : parseSprite3D ()          ; break;
      case OBJTYPE_TEXTURE2D           : parseTexture2D ()         ; break;
      case OBJTYPE_TRIANGLE_STRIP_ARRAY: parseTriangleStripArray (); break;
      case OBJTYPE_VERTEX_ARRAY        : parseVertexArray ()       ; break;
      case OBJTYPE_VERTEX_BUFFER       : parseVertexBuffer ()      ; break;
      case OBJTYPE_WORLD               : parseWorld ()             ; break;
      default: 
	cout << "Unknown obj type = " << object_type << " is ignored.\n";
	ifs.seekg (length, ios_base::cur);
      }

      if (ifs.tellg() >= end_of_objects)
	break;
    }

    // checksum, 今だけノーチェック
    //unsigned int checksum = getUInt32 ();
    unsigned int checksum = getUInt32();
    //cout << "  checksum = " << checksum << "\n";

  }

  return objs;
}

bool Loader:: getBoolean ()
{
  bool buf;
  ifs.read ((char*)&buf, 1);
  return buf;
}

char Loader:: getByte ()
{
  char buf;
  ifs.read ((char*)&buf, 1);
  return buf;
}

short Loader:: getInt16 ()
{
  short buf;
  ifs.read ((char*)&buf, 2);
  return buf;
}

unsigned short Loader:: getUInt16 ()
{
  unsigned short buf;
  ifs.read ((char*)&buf, 2);
  return buf;
}

int Loader:: getInt32 ()
{
  int buf;
  ifs.read ((char*)&buf, 4);
  return buf;
}

unsigned int Loader:: getUInt32 ()
{
  unsigned int buf;
  ifs.read ((char*)&buf, 4);
  return buf;
}

float        Loader:: getFloat32 ()
{
  float buf;
  ifs.read ((char*)&buf, 4);
  return buf;
}

const char* Loader:: getString ()
{
  static char buf[128];
  ifs.getline (buf, 128, '\0');
  return buf;
}

unsigned int Loader:: getObjectIndex ()
{
  unsigned int buf;
  ifs.read ((char*)&buf, 4);
  return buf;
}

int          Loader:: getColorRGBA ()
{
  unsigned char r,g,b,a;
  ifs.read ((char*)&r, 1);
  ifs.read ((char*)&g, 1);
  ifs.read ((char*)&b, 1);
  ifs.read ((char*)&a, 1);
  return (a << 24) | (r << 16) | (g << 8) | (b << 0);
}

int          Loader:: getColorRGB ()
{
  unsigned char r,g,b;
  ifs.read ((char*)&r, 1);
  ifs.read ((char*)&g, 1);
  ifs.read ((char*)&b, 1);
  return (r << 16) | (g << 8) | (b << 0);
}

bool         Loader:: getM3GIdent ()
{
  char ident[12];
  ifs.read ((char*)ident, 12);
  //cout << "ident = ";
  //for (int i = 0; i < 12; i++) {
  //  cout << ident[i];
  //}
  //cout << "\n";
  return true;
}

char* Loader:: getByteArray (int n)
{
  char* p = new char[n];
  ifs.read ((char*)p, n);
  return p;
}


short* Loader:: getInt16Array (int n)
{
  short* p = new short[n];
  ifs.read ((char*)p, n*2);
  return p;
}

unsigned short* Loader:: getUInt16Array (int n)
{
  unsigned short* p = new unsigned short[n];
  ifs.read ((char*)p, n*2);
  return p;
}


unsigned int* Loader:: getUInt32Array (int n)
{
  unsigned int* p = new unsigned int[n];
  ifs.read ((char*)p, n*4);
  return p;
}

float* Loader:: getFloat32Array (int n)
{
  float* p = new float[n];
  ifs.read ((char*)p, n*4);
  return p;
}

void Loader:: parseHeader ()
{
  char version_major                    = getByte();
  char version_minor                    = getByte();
  bool has_external_refference          = getBoolean();
  unsigned int total_file_size          = getUInt32 ();
  unsigned int approximate_content_size = getUInt32 ();
  const char* authoring_field           = getString ();

  //cout << "M3G Header ---\n";
  //cout << "  version_major = "            << (int)version_major       << "\n";
  //cout << "  version_minor = "            << (int)version_minor       << "\n";
  //cout << "  has_external_refference  = " << has_external_refference  << "\n";
  //cout << "  total_file_size          = " << total_file_size          << "\n";
  //cout << "  approximate_content_size = " << approximate_content_size << "\n";
  ////cout << "  authoring_field          = " << authoring_field          << "\n";

  if (has_external_refference) {
    throw NotImplementedException (__FILE__, __func__, "Has_external_reference is not implemented.");
  }

  // delete authoring_field;

  // 注意1：index 0はダミー
  // 注意2：Header objectもオブジェクトに含む
  objs.clear ();
  objs.reserve (16);
  objs.push_back ((Object3D*)0);  // for dummy
  objs.push_back ((Object3D*)0);  // for header
}

void Loader:: parseExref ()
{
  const char* uri = getString();
  //cout << "uri = " << uri << "\n";
}

void Loader:: parseAnimationController ()
{
  AnimationController* ctr = new AnimationController;
  parseObject3D (ctr);

  float speed                   = getFloat32 ();
  float weight                  = getFloat32 (); 
  int   active_interval_start   = getInt32 ();
  int   active_interval_end     = getInt32 ();
  float reference_sequence_time = getFloat32 ();
  int   reference_world_time    = getInt32 ();

  ctr->setSpeed (speed, reference_world_time);
  ctr->setWeight (weight);
  ctr->setActiveInterval (active_interval_start, active_interval_end);
  ctr->setPosition (reference_sequence_time, reference_world_time);

  //ctr->AnimationController:: print (cout);

  objs.push_back (ctr);
}

void Loader:: parseAnimationTrack ()
{
  Object3D* obj = new Object3D;
  parseObject3D (obj);

  unsigned int keyframe_sequence_index    = getUInt32();
  unsigned int animation_controller_index = getUInt32();
  unsigned int property_id = getInt32();
 
   
  KeyframeSequence* kseq = dynamic_cast<KeyframeSequence*>(objs.at(keyframe_sequence_index));
  AnimationTrack*   track = new AnimationTrack (kseq, property_id);

  if (animation_controller_index > 0) {
    AnimationController* ctr = dynamic_cast<AnimationController*>(objs.at(animation_controller_index));
    track->setController (ctr);
  }

  //track->AnimationTrack:: print (cout);

  objs.push_back (track);
}

void Loader:: parseAppearance ()
{
  Appearance* app = new Appearance;
  parseObject3D (app);

  char         layer                  = getByte ();
  unsigned int compositing_mode_index = getUInt32 ();
  unsigned int fog_index              = getUInt32 ();
  unsigned int polygon_mode_index     = getUInt32 ();
  unsigned int material_index         = getUInt32 ();

  app->setLayer (layer);

  if (compositing_mode_index > 0) {
    CompositingMode* cmode = dynamic_cast<CompositingMode*>(objs.at(compositing_mode_index));
    app->setCompositingMode (cmode);
  }
  if (fog_index > 0) {
    Fog* fog = dynamic_cast<Fog*>(objs.at(fog_index));
    app->setFog (fog);
  }
  if (polygon_mode_index > 0) {
    PolygonMode* pmode = dynamic_cast<PolygonMode*>(objs.at(polygon_mode_index));
    app->setPolygonMode (pmode);
  }
  if (material_index > 0) {
    Material* mat = dynamic_cast<Material*>(objs.at(material_index));
    app->setMaterial (mat);
  }

  unsigned int textures_count = getUInt32 ();
  for (int i = 0; i < (int)textures_count; i++) {
    unsigned int texture_index = getUInt32 ();
    Texture2D* tex = dynamic_cast<Texture2D*>(objs.at(texture_index));
    app->setTexture (i, tex);
  }

  //app->Appearance:: print (cout);

  objs.push_back (app);
}

void Loader:: parseBackground ()
{
  Background* bg = new Background;
  parseObject3D (bg);

  int          background_color        = getColorRGBA ();
  unsigned int background_image_index  = getUInt32 ();
  char         background_image_mode_x = getByte ();
  char         background_image_mode_y = getByte ();
  int          crop_x                  = getInt32 ();
  int          crop_y                  = getInt32 ();
  int          crop_width              = getInt32 ();
  int          crop_height             = getInt32 ();
  bool         depth_clear_enabled     = getBoolean ();
  bool         color_clear_enabled     = getBoolean ();

  bg->setColor (background_color);
  if (background_image_index > 0) {
    Image2D* img = dynamic_cast<Image2D*>(objs.at(background_image_index));
    bg->setImage (img);
  }
  bg->setImageMode (background_image_mode_x, background_image_mode_y);
  bg->setCrop (crop_x, crop_y, crop_width, crop_height);
  bg->setDepthClearEnable (depth_clear_enabled);
  bg->setColorClearEnable (color_clear_enabled);
   
  //bg->Background:: print (cout);

  objs.push_back (bg);
}

void Loader:: parseCamera ()
{
  Camera* cam = new Camera;
  parseNode (cam);
   
  char projection_type = getByte();
  if (projection_type == Camera::GENERIC) {
    float matrix[16];
    for (int i = 0; i < 16; i++)
      matrix[i] = getFloat32 ();
    Transform trns;
    trns.set (matrix);
    cam->setGeneric (trns);
  } else {
    float fovy         = getFloat32 ();
    float aspect_ratio = getFloat32 ();
    float near         = getFloat32 ();
    float far          = getFloat32 ();
    cam->setPerspective (fovy, aspect_ratio, near, far);
  }

  //cam->print (cout);

  objs.push_back (cam);
}

void Loader:: parseCompositingMode ()
{
  CompositingMode* cmode = new CompositingMode;
  parseObject3D (cmode);

  bool  depth_test_enabled  = getBoolean ();
  bool  depth_write_enabled = getBoolean ();
  bool  color_write_enabled = getBoolean ();
  bool  alpha_write_enabled = getBoolean ();
  char  blending            = getByte ();
  char  alpha_threshold     = getByte ();
  float depth_offset_factor = getFloat32 ();
  float depth_offset_units  = getFloat32 ();

  cmode->setDepthTestEnable (depth_test_enabled);
  cmode->setDepthWriteEnable (depth_write_enabled);
  cmode->setColorWriteEnable (color_write_enabled);
  cmode->setAlphaWriteEnable (alpha_write_enabled);
  cmode->setBlending (blending);
  cmode->setAlphaThreshold (alpha_threshold);
  cmode->setDepthOffset (depth_offset_factor, depth_offset_units);

  objs.push_back (cmode);
}

void Loader:: parseFog ()
{
  Fog* fog = new Fog;
  parseObject3D (fog);

  int  color = getColorRGB ();
  char mode  = getByte ();

  fog->setColor (color);

  if (mode == Fog::EXPONENTIAL) {
    float density = getFloat32 ();
    fog->setDensity (density);
  } else {
    float near = getFloat32 ();
    float far  = getFloat32 ();
    fog->setLinear (near, far);
  }

  objs.push_back (fog);
}

void Loader:: parseGroup ()
{
  Group* grp = new Group;
  parseGroup (grp);

  objs.push_back (grp);
}

void Loader:: parseGroup (Group* grp)
{
  parseNode (grp);

  unsigned int children_count = getUInt32 ();
  for (int i = 0; i < (int)children_count; i++) {
    unsigned int children_index = getUInt32 ();
    if (children_index > 0) {
      Node* node = dynamic_cast<Node*>(objs.at(children_index));
      grp->addChild (node);
    }

  }

  //grp->Group:: print (cout);
}

void Loader:: parseImage2D ()
{
  Object3D* obj = new Object3D;
  parseObject3D (obj);

  Image2D* img = 0;

  char format         = getByte ();
  bool is_mutable     = getBoolean ();
  unsigned int width  = getUInt32 ();
  unsigned int height = getUInt32 ();
  if (!is_mutable) {
    unsigned int palette_count = getUInt32 ();
    if (palette_count > 0) {
      throw NotImplementedException (__FILE__, __func__, "Paletted Image is not supported.");
    }
    unsigned int pixel_count = getUInt32 ();
    char*        pixels      = getByteArray(pixel_count);
    img = new Image2D (format, width, height, pixels);
    delete [] pixels;
  }

  // img->print (cout);

  objs.push_back (img);
}

void Loader:: parseIndexBuffer (IndexBuffer* ibuf)
{
  parseObject3D (ibuf);
  // nothing to do.
}

void Loader:: parseKeyframeSequence ()
{
  Object3D* obj = new Object3D;
  parseObject3D (obj);

  KeyframeSequence* kseq = 0;

  unsigned char interpolation     = getByte ();
  unsigned char repeat_mode       = getByte ();
  unsigned char encoding          = getByte ();
  unsigned int  duration          = getUInt32 ();
  unsigned int  valid_range_first = getUInt32 ();
  unsigned int  valid_range_last  = getUInt32 ();
  unsigned int  component_count   = getUInt32 ();
  unsigned int  keyframe_count    = getUInt32 ();

  kseq = new KeyframeSequence (keyframe_count, component_count, interpolation);

  switch (encoding) {
  case 0: {
    for (int i = 0; i < (int)keyframe_count; i++) {
      unsigned int time  = getUInt32 ();
      float*       value = getFloat32Array (component_count);
      kseq->setKeyframe (i, time, value);
      delete [] value;
    }
    break;
  }
  case 1: {
    float* bias  = getFloat32Array (component_count);
    float* scale = getFloat32Array (component_count);
    for (int i = 0; i < (int)keyframe_count; i++) {
      unsigned int time = getUInt32 ();
      char* encoded_values = getByteArray (component_count);
      float* values = new float[component_count];
      for (int j = 0; j < (int)component_count; j++) {
	values[j] = (encoded_values[j] - bias[j]) / scale[j] * 255;
      }
      kseq->setKeyframe (i, time, values);
      delete [] encoded_values;
      delete [] values;
    }
    delete [] bias;
    delete [] scale;
    break;
  }
  case 2: {
    float* bias  = getFloat32Array (component_count);
    float* scale = getFloat32Array (component_count);
    for (int i = 0; i < (int)keyframe_count; i++) {
      unsigned int time    = getUInt32 ();
      short* encoded_values = getInt16Array (component_count);
      float* values = new float[component_count];
      for (int j = 0; j < (int)component_count; j++) {
	values[j] = (encoded_values[j] - bias[j]) / scale[j] * 255;
      }
      kseq->setKeyframe (i, time, values);
      delete bias;
      delete scale;
      delete encoded_values;
      delete values;
    }
    break;
  }
  default: {
    throw IOException (__FILE__, __func__, "Encoding is invalid, enc=%d.", encoding);
  }
  }

  kseq->setRepeatMode (repeat_mode);
  kseq->setDuration (duration);
  kseq->setValidRange (valid_range_first, valid_range_last);

  //kseq->KeyframeSequence:: print (cout);

  objs.push_back (kseq);
}

void Loader:: parseLight ()
{
  Light* lig = new Light;
  parseNode (lig);

  float attenuation_constant  = getFloat32 ();
  float attenuation_linear    = getFloat32 ();
  float attenuation_quadratic = getFloat32 ();
  int   color                 = getColorRGB ();
  unsigned char mode          = getByte ();
  float intensity             = getFloat32 ();
  float spot_angle            = getFloat32 ();
  float spot_exponent         = getFloat32 ();

  lig->setAttenuation (attenuation_constant, attenuation_linear, attenuation_quadratic);
  lig->setColor (color);
  lig->setMode (mode);
  lig->setIntensity (intensity);
  lig->setSpotAngle (spot_angle);
  lig->setSpotExponent (spot_exponent);

  //lig->Light:: print (cout);

  objs.push_back (lig);
}

void Loader:: parseMaterial ()
{
  Material* mat = new Material;
  parseObject3D (mat);

  int   ambient_color  = getColorRGB();
  int   diffuse_color  = getColorRGBA();
  int   emissive_color = getColorRGB();
  int   specular_color = getColorRGB();
  float shininess      = getFloat32();
  bool  vertex_color_tracking_enabled = getBoolean();

  mat->setColor (Material::AMBIENT, ambient_color);
  mat->setColor (Material::DIFFUSE, diffuse_color);
  mat->setColor (Material::EMISSIVE, emissive_color);
  mat->setColor (Material::SPECULAR, specular_color);
  mat->setShininess (shininess);
  mat->setVertexColorTrackingEnable (vertex_color_tracking_enabled);

  //mat->Material:: print (cout);

  objs.push_back (mat);
}

void Loader:: parseMesh ()
{
  Node* node = new Node;
  parseNode (node);

  unsigned int vertex_buffer_index = getUInt32 ();
  unsigned int submesh_count       = getUInt32 ();

  IndexBuffer** indices = new IndexBuffer* [submesh_count];
  Appearance**  appears = new Appearance* [submesh_count];
   
  VertexBuffer*  vbuf = dynamic_cast<VertexBuffer*>(objs.at(vertex_buffer_index));

  for (int i = 0; i < (int)submesh_count; i++) {
    unsigned int index_buffer_index = getUInt32();
    unsigned int appearance_index   = getUInt32();
     
    indices[i] = dynamic_cast<IndexBuffer*>(objs.at(index_buffer_index));
    appears[i] = dynamic_cast<Appearance*>(objs.at(appearance_index));
  }

  Mesh* mesh = new Mesh (vbuf, submesh_count, indices, submesh_count, appears);

  //mesh->Mesh:: print (cout);

  delete [] indices;
  delete [] appears;

  objs.push_back (mesh);
}


 
void Loader:: parseNode (Node* node)
{
  parseTransformable (node);

  bool         enable_rendering = getBoolean();
  bool         enable_picking   = getBoolean();
  char         alpha_factor     = getByte ();
  unsigned int scope            = getUInt32();
  bool         has_alignment    = getBoolean();
  if (has_alignment) {
    char z_target                  = getByte();
    char y_target                  = getByte();
    unsigned int z_reference_index = getUInt32();
    unsigned int y_reference_index = getUInt32();

    Node* z_ref = dynamic_cast<Node*>(objs.at(z_reference_index));
    Node* y_ref = dynamic_cast<Node*>(objs.at(y_reference_index));
    node->setAlignment (z_ref, z_target, y_ref, y_target);
  }
  node->setAlphaFactor ((alpha_factor+128)/255.f);
  node->setPickingEnable (enable_picking);
  node->setRenderingEnable (enable_rendering);
  node->setScope (scope);

  //node->Node::print (cout);
}

void Loader:: parseObject3D (Object3D* obj)
{
  unsigned int user_id                = getUInt32(); 

  obj->setUserID (user_id);

  unsigned int animation_tracks_count = getUInt32();
  for (int i = 0; i < (int)animation_tracks_count; i++) {
    unsigned int animation_track_index = getUInt32();
    if (animation_track_index > 0) {
      AnimationTrack* track = dynamic_cast<AnimationTrack*>(objs.at(animation_track_index));
      obj->addAnimationTrack (track);
    }
  }
  unsigned int user_parameter_count = getUInt32();
  for (int i = 0; i < (int)user_parameter_count; i++) {
    unsigned int parameter_id          = getUInt32();
    unsigned int parameter_value_count = getUInt32();
    //cout << "dummy" << parameter_id << parameter_value_count;
    for (int i = 0; i < (int)parameter_value_count; i++) {
      char parameter_value = getByte ();
      //cout << "duumy" << parameter_value;
    }
    throw NotImplementedException (__FILE__, __func__, "User object is not implemted.");
  }
   
  //obj->Object3D::print (cout);
}

void Loader:: parsePolygonMode ()
{
  PolygonMode* pmode = new PolygonMode;
  parseObject3D (pmode);

  unsigned char  culling  = getByte();
  unsigned char  shading  = getByte();
  unsigned char  winding  = getByte();
  bool two_sided_lighting_enabled     = getBoolean();
  bool local_camera_lighting_enabled  = getBoolean();
  bool perspective_correction_enabled = getBoolean();

  pmode->setCulling (culling);
  pmode->setShading (shading);
  pmode->setWinding (winding);
  pmode->setTwoSidedLightingEnable (two_sided_lighting_enabled);
  pmode->setLocalCameraLightingEnable (local_camera_lighting_enabled);
  pmode->setPerspectiveCorrectionEnable (perspective_correction_enabled);

  //pmode->PolygonMode:: print (cout);

  objs.push_back (pmode);
}

void Loader:: parseSkinnedMesh ()
{
  // 注意：Mesh部分はparseMesh()と完全に同じ。
  // ---- ここからparseMesh()と同じ ----

  Node* node = new Node;
  parseNode (node);

  unsigned int vertex_buffer_index = getUInt32 ();
  unsigned int submesh_count       = getUInt32 ();

  IndexBuffer** indices = new IndexBuffer* [submesh_count];
  Appearance**  appears = new Appearance*  [submesh_count];
   
  VertexBuffer*  vbuf = dynamic_cast<VertexBuffer*>(objs.at(vertex_buffer_index));

  for (int i = 0; i < (int)submesh_count; i++) {
    unsigned int index_buffer_index = getUInt32();
    unsigned int appearance_index   = getUInt32();
     
    indices[i] = dynamic_cast<IndexBuffer*>(objs.at(index_buffer_index));
    appears[i] = dynamic_cast<Appearance*>(objs.at(appearance_index));
  }

  // ---- ここまでparseMesh()と同じ ----

  unsigned int skeleton_index = getUInt32();
  Group*       skeleton       = dynamic_cast<Group*>(objs.at(skeleton_index));

  SkinnedMesh* mesh = new SkinnedMesh (vbuf, submesh_count, indices, submesh_count, appears, skeleton);
  *(Node*)mesh = *node;

  //mesh->Mesh:: print (cout);

  delete [] indices;
  delete [] appears;


  unsigned int transform_reference_count = getUInt32();
  for (int i = 0; i < (int)transform_reference_count; i++) {
    unsigned int transfrom_node_index = getUInt32();
    unsigned int first_vertex         = getUInt32();
    unsigned int vertex_count         = getUInt32();
    int          weight               = getInt32();
    Node* node = dynamic_cast<Node*>(objs.at(transfrom_node_index));
    mesh->addTransform (node, weight, first_vertex, vertex_count);
  }

  //mesh->SkinnedMesh:: print (cout);

  objs.push_back (mesh);
}

void Loader:: parseSprite3D ()
{
  Node* node = new Node;
  parseNode (node);

  unsigned int image_index      = getUInt32();
  unsigned int appearance_index = getUInt32();
  bool         is_scaled        = getBoolean();
  int          crop_x           = getInt32();
  int          crop_y           = getInt32();
  int          crop_width       = getInt32();
  int          crop_height      = getInt32();

  Image2D*    img = dynamic_cast<Image2D*>(objs.at(image_index));
  Appearance* app = dynamic_cast<Appearance*>(objs.at(appearance_index));
  Sprite3D*   spr = new Sprite3D (is_scaled, img, app);

  *(Node*)spr = *node;
  spr->setCrop (crop_x, crop_y, crop_width, crop_height);

  //spr->Sprite3D:: print (cout);

  objs.push_back (spr);
}

void Loader:: parseTexture2D ()
{
  Transformable* trns = new Transformable;
  parseTransformable (trns);

  unsigned int image_index   = getUInt32();
  int  blend_color           = getColorRGB();
  unsigned char blending     = getByte ();
  unsigned char wrapping_s   = getByte ();
  unsigned char wrapping_t   = getByte ();
  unsigned char level_filter = getByte ();
  unsigned char image_filter = getByte ();

  Image2D*   img = dynamic_cast<Image2D*>(objs.at(image_index));
  Texture2D* tex = new Texture2D (img);

  *(Transformable*)tex = *trns;
  tex->setBlendColor (blend_color);
  tex->setBlending (blending);
  tex->setWrapping (wrapping_s, wrapping_t);
  tex->setFiltering (level_filter, image_filter);
   
  //tex->Texture2D:: print (cout);
   
  objs.push_back (tex);
}

void Loader:: parseTransformable (Transformable* trans)
{
  parseObject3D (trans);

  bool has_component_transform = getBoolean();
  if (has_component_transform) {
    float tx    = getFloat32();
    float ty    = getFloat32();
    float tz    = getFloat32();
    float sx    = getFloat32();
    float sy    = getFloat32();
    float sz    = getFloat32();
    float angle = getFloat32();
    float ax    = getFloat32();
    float ay    = getFloat32();
    float az    = getFloat32();
    trans->setTranslation (tx, ty, tz);
    trans->setScale (sx, sy, sz);
    trans->setOrientation (angle, ax, ay, az);
  }
  bool has_general_transform = getBoolean();
  if (has_general_transform) {
    float matrix[16];
    for (int i = 0; i < 16; i++)
      matrix[i] = getFloat32();
    Transform tra;
    tra.set (matrix);
    trans->setTransform (tra);
  }

  trans->Transformable::print (cout);
}


void Loader:: parseTriangleStripArray ()
{
  IndexBuffer* ibuf = new IndexBuffer;
  parseIndexBuffer (ibuf);

  unsigned int        start_index;
  unsigned int        indices_count;
  int*                indices;
  TriangleStripArray* tris;

  unsigned char encoding = getByte();
  switch (encoding) {
  case 0:
    start_index = getUInt32();
    break;
  case 1:
    start_index = getByte();
    break;
  case 2:
    start_index = getUInt16();
    break;
  case 128:
    indices_count = getUInt32();
    indices       = new int [indices_count];
    for (int i = 0; i < (int)indices_count; i++) 
      indices[i] = getUInt32();
    break;
  case 129:
    indices_count = getUInt32();
    indices       = new int [indices_count];
    for (int i = 0; i < (int)indices_count; i++)
      indices[i] = getByte();
    break;
  case 130:
    indices_count = getUInt32();
    indices       = new int [indices_count];
    for (int i = 0; i < (int)indices_count; i++)
      indices[i] = getUInt16();
    break;
  default:
    throw IOException (__FILE__, __func__, "Encoding is invalid, enc=%d.", encoding);
  }

  unsigned int strips_len = getUInt32();
  int*         strips     = (int*)getUInt32Array(strips_len);
  //cout << "encoding = " << (int)encoding << "\n";
  //cout << "1<<7 = " << 1<<7 << "\n";

  if (encoding & (1<<7)) {
    tris = new TriangleStripArray (indices, strips_len, strips);
    //cout << "A\n";
  }
  else {
    tris = new TriangleStripArray (start_index, strips_len, strips);
    //cout << "B\n";
  }
   
  *(IndexBuffer*)tris = *ibuf;

  //tris->TriangleStripArray:: print(cout);

  objs.push_back (tris);
}

void Loader:: parseVertexArray ()
{
  Object3D* obj = new Object3D;
  parseObject3D (obj);

  char component_size         = getByte();
  char component_count        = getByte();
  char encoding               = getByte();
  unsigned short vertex_count = getUInt16();

  VertexArray* varr = new VertexArray (vertex_count, component_count, component_size);
  *(Object3D*)varr = *obj;
   
  if (component_size == 1) {
    char* vertex_values = getByteArray(vertex_count*component_count);
    if (encoding) {
      for (int i = component_count; i < vertex_count*component_count; i++)
	vertex_values[i] += vertex_values[i-component_count];
    }
    varr->set (0, vertex_count, vertex_values);
    delete [] vertex_values;
  } else if (component_size == 2) {
    short* vertex_values = getInt16Array (vertex_count*component_count);
    if (encoding) {
      for (int i = component_count; i < vertex_count*component_count; i++)
	vertex_values[i] += vertex_values[i-component_count];
    }
    varr->set (0, vertex_count, vertex_values);
    delete [] vertex_values;
  } else {
    throw IOException (__FILE__, __func__, "Component size is invalid, size=%d.", component_size);
  }

  //varr->VertexArray:: print (cout);

  objs.push_back (varr);
}

void Loader:: parseVertexBuffer ()
{
  VertexBuffer* vbuf = new VertexBuffer;
  parseObject3D(vbuf);

  int          default_color   = getColorRGBA();
  unsigned int positions_index = getUInt32();
  float        bias[3]         = {getFloat32(), getFloat32(), getFloat32()};
  float        scale           = getFloat32();
  unsigned int normals_index   = getUInt32();
  unsigned int colors_index    = getUInt32();
   
  //cout << "positions_index = " << positions_index << "\n";
  //cout << "normals_index = " << normals_index << "\n";
  //cout << "colors_index = " << colors_index << "\n";

  vbuf->setDefaultColor (default_color);
  if (positions_index > 0) {
    VertexArray* positions = dynamic_cast<VertexArray*>(objs.at(positions_index));
    vbuf->setPositions (positions, scale, bias);
  }
  if (normals_index > 0) {
    VertexArray* normals = dynamic_cast<VertexArray*>(objs.at(normals_index));
    vbuf->setNormals (normals);
  }
  if (colors_index > 0) {
    VertexArray* colors = dynamic_cast<VertexArray*>(objs.at(colors_index));
    vbuf->setColors (colors);
  }

  unsigned int tex_coord_array_count = getUInt32();
  for (int i = 0; i < (int)tex_coord_array_count; i++) {
    unsigned int tex_coords_index = getUInt32();
    float        bias[3]          = {getFloat32(), getFloat32(), getFloat32()};
    float        scale            = getFloat32();
    if (tex_coords_index > 0) {
      VertexArray* tex_coords = dynamic_cast<VertexArray*>(objs.at(tex_coords_index));
      vbuf->setTexCoords (i, tex_coords, scale, bias);
    }
  }
   
  //vbuf->VertexBuffer:: print (cout);

  objs.push_back (vbuf);
}

void Loader:: parseWorld ()
{
  World* wld = new World;
  parseGroup (wld);

  unsigned int active_camera_index = getUInt32();
  unsigned int background_index    = getUInt32();

  //cout << "camera_index = " << active_camera_index << "\n";
  //cout << "background_index = " << background_index << "\n";

  if (active_camera_index > 0) {
    Camera*     cam = dynamic_cast<Camera*>(objs.at(active_camera_index));
    wld->setActiveCamera (cam);
  }

  if (background_index > 0) {
    Background* bg  = dynamic_cast<Background*>(objs.at(background_index));
    wld->setBackground (bg);
  }

  wld->World:: print (cout);

  objs.push_back (wld);
}

std::ostream& operator<< (std::ostream& out, const Loader& ldr)
{
  out << "Loader: ";
  return out << "\n";
}
