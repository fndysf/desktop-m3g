#include "VertexArray.hpp"
#include "Exception.hpp"
#include <iostream>
#include <cstring>
using namespace std;
using namespace m3g;



VertexArray:: VertexArray (int num_vertices, int num_components, int component_size_) :
  component_count(num_components),
  component_size(component_size_),
  vertex_count(num_vertices),
  char_values(0), vbo(0)
{
  setObjectType (OBJTYPE_VERTEX_ARRAY);

  if (vertex_count < 1 || vertex_count > 65535) {
    throw IllegalArgumentException (__FILE__, __func__, "Vertex count is invalid, vertex_count=%d.", vertex_count);
  }
  if (component_count < 2 || component_count > 4) {
    throw IllegalArgumentException (__FILE__, __func__, "Component count is invalid, component_count=%d.", component_count);
  }
  if (component_size != 1 && component_size != 2 && component_size != 4) {
    throw IllegalArgumentException (__FILE__, __func__, "Component size is invalid, component_size=%d.", component_size);
  }

  int size = vertex_count * component_count * component_size;

  char_values = new char[size];
  memset (char_values, 0, size);

  glGenBuffers (1, &vbo);
  glBindBuffer (GL_ARRAY_BUFFER, vbo);
  glBufferData (GL_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW);
}

VertexArray:: ~VertexArray ()
{
  delete [] char_values;
  if (glIsBuffer(vbo)) {
    glDeleteBuffers (1, &vbo);
  }
}

VertexArray* VertexArray:: duplicate () const
{
  VertexArray* varry = new VertexArray (*this);
  Object3D* obj      = Object3D::duplicate();
  *(Object3D*)varry  = *obj;

  int size           = vertex_count * component_count * component_size;
  varry->char_values = new char[size];
  memcpy (varry->char_values, this->char_values, size);

  glGenBuffers (1, &varry->vbo);
  glBindBuffer (GL_ARRAY_BUFFER, varry->vbo);
  glBufferData (GL_ARRAY_BUFFER, size, varry->char_values, GL_STATIC_DRAW);
  return varry;
}

void VertexArray:: get (int first_vertex, int num_vertices, char* values) const
{
  if (first_vertex < 0 || first_vertex >= vertex_count) {
    throw IllegalArgumentException (__FILE__, __func__, "First vertex is invalid, first_vertex=%d", first_vertex);
  }
  if (num_vertices < 0 || first_vertex + num_vertices > vertex_count) {
    throw IllegalArgumentException(__FILE__, __func__, "Number of vertices is Invalid, first_vertex=%d, num_vertices=%d.", first_vertex, num_vertices);
  }
  if (values == NULL) {
    throw NullPointException (__FILE__, __func__, "Values is NULL.");
  }
  if (component_size != 1) {
    throw IllegalStateException (__FILE__, __func__, "Component size of this vertex array is not 1 byte, component_size=%d.", component_size);
  }

  memcpy ((char*)values, 
          (char*)&char_values[first_vertex*component_count],
          num_vertices*component_count*sizeof(char));
}

void VertexArray:: get (int first_vertex, int num_vertices, short* values) const
{
  if (first_vertex < 0 || first_vertex >= vertex_count) {
    throw IllegalStateException (__FILE__, __func__, "First vertex is invalid, first_vertex=%d.", first_vertex);
  }
  if (num_vertices < 0 || first_vertex + num_vertices > vertex_count) {
    throw IllegalStateException (__FILE__, __func__, "Number of vertices is invalid, first_vertex=%d, num_vertices=%d.", first_vertex, num_vertices);
  }
  if (values == NULL) {
    throw NullPointException (__FILE__, __func__, "Values is NULL.");
  }
  if (component_size != 2) {
    throw IllegalStateException (__FILE__, __func__, "Component size of this vertex array is not 2 byte, component_size=%d.", component_size);
  }

  memcpy ((char*)values,
          (char*)&short_values[first_vertex*component_count],
          num_vertices*component_count*sizeof(short));
}

void VertexArray:: get (int first_vertex, int num_vertices, float* values) const
{
  if (first_vertex < 0 || first_vertex >= vertex_count) {
    throw IllegalStateException (__FILE__, __func__, "First vertex is invalid, first_vertex=%d.", first_vertex);
  }
  if (num_vertices < 0 || first_vertex + num_vertices > vertex_count) {
    throw IllegalStateException (__FILE__, __func__, "Number of vertices is invalid, first_vertex=%d, num_vertices=%d.", first_vertex, num_vertices);
  }
  if (values == NULL) {
    throw NullPointException (__FILE__, __func__, "Values is NULL.");
  }
  if (component_size != 4) {
    throw IllegalStateException (__FILE__, __func__, "Component size of this vertex array is not 4 byte, component_size=%d.", component_size);
  }

  memcpy ((char*)values,
          (char*)&float_values[first_vertex*component_count],
          num_vertices*component_count*sizeof(float));
}

int VertexArray:: getComponentCount () const
{
    return component_count;
}

int VertexArray:: getComponentType () const
{
  return component_size;
}

int VertexArray:: getVertexCount () const
{
    return vertex_count;
}

void VertexArray:: set (int first_vertex, int num_vertices, const char* values)
{
  if (values == NULL) {
    throw NullPointException (__FILE__, __func__, "Values is NULL.");
  }
  if (num_vertices < 0) {
    throw IllegalArgumentException (__FILE__, __func__, "Number of vertices is invalid, num_vertices=%d.", num_vertices);
  }
  if (first_vertex < 0 || first_vertex + num_vertices > vertex_count) {
    throw IndexOutOfBoundsException (__FILE__, __func__, "Vertex is out of bounds, [%d,%d) in [0,%d).", first_vertex, first_vertex+num_vertices, vertex_count);
  }
  if (component_size != 1) {
    throw IllegalStateException (__FILE__, __func__, "Component size of this vertex array is not 1 byte, component_size=%d.", component_size);
  }

  int offset      = first_vertex*component_count;
  int offset_size = first_vertex*component_count*sizeof(char);
  int size        = num_vertices*component_count*sizeof(char);

  memcpy ((char*)&char_values[offset],
          (char*)values,
          size);

  glBindBuffer (GL_ARRAY_BUFFER, vbo);
  glBufferSubData (GL_ARRAY_BUFFER,
                   offset_size,
                   size, 
                   &char_values[offset]);
}

void VertexArray:: set (int first_vertex, int num_vertices, const short* values)
{
  if (values == NULL) {
    throw NullPointException (__FILE__, __func__, "Values is NULL.");
  }
  if (num_vertices < 0) {
    throw IllegalArgumentException (__FILE__, __func__, "Number of vertices is invalid, num_vertices=%d.", num_vertices);
  }
  if (first_vertex < 0 || first_vertex + num_vertices > vertex_count) {
    throw IndexOutOfBoundsException (__FILE__, __func__, "Vertex is out of bounds, [%d,%d) in [0,%d).", first_vertex, first_vertex+num_vertices, vertex_count);
  }
  if (component_size != 2) {
    throw IllegalStateException (__FILE__, __func__, "Component size of this vertex array is not 2 byte, component_size=%d.", component_size);
  }

  int offset      = first_vertex*component_count;
  int offset_size = first_vertex*component_count*sizeof(short);
  int size        = num_vertices*component_count*sizeof(short);

  //cout << "vbo         = " << vbo << "\n";
  //cout << "offset      = " << offset      << "\n";
  //cout << "offset_size = " << offset_size << "\n";
  //cout << "size        = " << size        << "\n";

  memcpy ((char*)&short_values[offset],
          (char*)values,
          size);

  glBindBuffer (GL_ARRAY_BUFFER, vbo);
  glBufferSubData (GL_ARRAY_BUFFER,
                   offset_size,
                   size,
                   &short_values[offset]);
}

void VertexArray:: set (int first_vertex, int num_vertices, const float* values)
{
  if (values == NULL) {
    throw NullPointException (__FILE__, __func__, "Values is NULL.");
  }
  if (num_vertices < 0) {
    throw IllegalArgumentException (__FILE__, __func__, "Number of vertices is invalid, num_vertices=%d.", num_vertices);
  }
  if (first_vertex < 0 || first_vertex + num_vertices > vertex_count) {
    throw IndexOutOfBoundsException (__FILE__, __func__, "Vertex is out of bounds, [%d,%d) in [0,%d).", first_vertex, first_vertex+num_vertices, vertex_count);
  }
  if (component_size != 4) {
    throw IllegalStateException (__FILE__, __func__, "Component size of this vertex array is not 4 byte, component_size=%d.", component_size);
  }

  int offset      = first_vertex*component_count;
  int offset_size = first_vertex*component_count*sizeof(float);
  int size        = num_vertices*component_count*sizeof(float);

  memcpy ((char*)&float_values[offset],
          (char*)values,
          size);

  glBindBuffer (GL_ARRAY_BUFFER, vbo);
  glBufferSubData (GL_ARRAY_BUFFER,
                   offset_size,
                   size,
                   &float_values[offset]);
}


void VertexArray:: setMorphing (const VertexArray* base,
                   const std::vector<const VertexArray*>& targets,
                   const std::vector<float>& weights)
{
  int size = vertex_count * component_count * component_size;
  memcpy (char_values, base->char_values, size);

  //cout << "targets.size() = " << targets.size() << "\n";
  //for (int t = 0; t < (int)targets.size(); t++) {
  //  cout << t << " : target = " << targets[t] << ", weights = " << weights[t] << "\n";
  //}

  for (int t = 0; t < (int)targets.size(); t++) {
    if (targets[t]) {
      for (int v = 0; v < vertex_count; v++) {
	switch (component_size) {
	case 1:
	  char_values[v*3  ] += weights[t] * (targets[t]->char_values[v*3  ] - base->char_values[v*3  ]);
	  char_values[v*3+1] += weights[t] * (targets[t]->char_values[v*3+1] - base->char_values[v*3+1]);
	  char_values[v*3+2] += weights[t] * (targets[t]->char_values[v*3+2] - base->char_values[v*3+2]);
	  break;
	case 2:
	  short_values[v*3  ] += weights[t] * (targets[t]->short_values[v*3  ] - base->short_values[v*3  ]);
	  short_values[v*3+1] += weights[t] * (targets[t]->short_values[v*3+1] - base->short_values[v*3+1]);
	  short_values[v*3+2] += weights[t] * (targets[t]->short_values[v*3+2] - base->short_values[v*3+2]);
	  break;
	case 4:
	  float_values[v*3  ] += weights[t] * (targets[t]->float_values[v*3  ] - base->float_values[v*3  ]);
	  float_values[v*3+1] += weights[t] * (targets[t]->float_values[v*3+1] - base->float_values[v*3+1]);
	  float_values[v*3+2] += weights[t] * (targets[t]->float_values[v*3+2] - base->float_values[v*3+2]);
	  break;
	}
      }  // バーテックスループ
    }
  }  // ターゲットループ

  glBindBuffer    (GL_ARRAY_BUFFER, vbo);
  glBufferSubData (GL_ARRAY_BUFFER,
                   0,
                   vertex_count * component_count * component_size,
                   char_values);
}


unsigned int VertexArray:: getOpenGLVBO () const
{
  return vbo;
}

unsigned int VertexArray:: getOpenGLFormat () const
{
   switch (component_size) {
   case 1: return GL_BYTE;
   case 2: return GL_SHORT;
   case 4: return GL_FLOAT;
   default: IllegalStateException (__FILE__, __func__, "Component type is invalid, type=%d.", component_size);
   }
   return 0;
}


std::ostream& VertexArray:: print (std::ostream& out) const
{
  out << "VertexArray: ";
  out << vertex_count << " vertices (";
  out << component_size << "byte x " << component_count << "comps / vertex)";
  out << ", vbo=" << vbo;
  return out;
}

std::ostream& VertexArray:: print_raw_data (std::ostream& out) const
{
  print (out) << "\n";
  for (int i = 0; i < vertex_count*component_count; i+=component_count) {
    out << i/component_count << " : (";
    for (int j = 0; j < component_count; j++) {
      switch (component_size) {
      case 1: out << (int)char_values[i+j]  << ", "; break;
      case 2: out << (int)short_values[i+j] << ", "; break;
      case 4: out <<      float_values[i+j] << ", "; break;
      default: throw IllegalStateException (__FILE__, __func__, "Component size is invalid, size=%d.", component_size);
      }
    }
    out << ")\n";
  }

  return out;
}

std::ostream& operator<< (std::ostream& out, const VertexArray& vary)
{
  return vary.print(out);
}
