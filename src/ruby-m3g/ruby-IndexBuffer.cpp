#include <iostream>
#include "ruby.h"
#include "m3g.hpp"
#include "ruby-m3g.hpp"
using namespace m3g;
using namespace std;

VALUE ruby_IndexBuffer_free (IndexBuffer* ptr)
{
    ptr->~IndexBuffer ();
    ruby_xfree (ptr);
}

VALUE ruby_IndexBuffer_allocate (VALUE self)
{
    void* p = ruby_xmalloc (sizeof(IndexBuffer));
    return Data_Wrap_Struct (self, 0, ruby_IndexBuffer_free, p);
}

VALUE ruby_IndexBuffer_initialize (VALUE self)
{
    IndexBuffer* p;
    Data_Get_Struct (self, IndexBuffer, p);
    new (p) IndexBuffer;
    return self;
}

VALUE ruby_IndexBuffer_get_index_count (VALUE self)
{
  IndexBuffer* p;
  int count;
  Data_Get_Struct (self, IndexBuffer, p);
  count = p->getIndexCount ();
  return INT2FIX(count);
}


VALUE ruby_IndexBuffer_get_indices (VALUE self)
{
  IndexBuffer* p;
  int count;
  Data_Get_Struct (self, IndexBuffer, p);
  count = p->getIndexCount ();

  int* indices = (int*)ruby_xmalloc (sizeof(int)*count);
  p->getIndices (indices);
  
  VALUE val_indices = rb_ary_new ();
  for (int i = 0; i < count; i++) {
    VALUE val_index = INT2FIX(indices[i]);
    rb_ary_push (val_indices, val_index);
  }

  return val_indices;
}


void register_IndexBuffer ()
{
     // IndexBuffer
     rb_define_alloc_func (rb_cIndexBuffer, ruby_IndexBuffer_allocate);
     rb_define_private_method (rb_cIndexBuffer, "initialize", (VALUE(*)(...))ruby_IndexBuffer_initialize, -1);

     rb_define_method (rb_cIndexBuffer, "index_count", (VALUE(*)(...))ruby_IndexBuffer_get_index_count, 0);
     rb_define_method (rb_cIndexBuffer, "indices",     (VALUE(*)(...))ruby_IndexBuffer_get_indices,    0);
}
