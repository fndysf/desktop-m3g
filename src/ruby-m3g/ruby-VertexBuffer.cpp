#include <iostream>
#include "ruby.h"
#include "m3g.hpp"
#include "ruby-m3g.hpp"
using namespace m3g;
using namespace std;

namespace {
    struct TextureAccessor {
        VertexBuffer* vertex_buffer;
    };
    VALUE rb_cVertexBuffer_TextureAccessor;
}

VALUE ruby_VertexBuffer_free (VertexBuffer* ptr)
{
    __TRY__;
    ptr->~VertexBuffer ();
    __CATCH__;
    ruby_xfree (ptr);
}

VALUE ruby_VertexBuffer_allocate (VALUE self)
{
    void* p = ruby_xmalloc (sizeof(VertexBuffer));
    return Data_Wrap_Struct (self, 0, ruby_VertexBuffer_free, p);
}

VALUE ruby_VertexBuffer_initialize (VALUE self)
{
    VertexBuffer* p;
    Data_Get_Struct (self, VertexBuffer, p);
    __TRY__;
    new (p) VertexBuffer;
    __CATCH__;
    p->setExportedEntity ((void*)self);
    return self;
}

VALUE ruby_VertexBuffer_get_colors (VALUE self)
{
    VertexBuffer* p;
    Data_Get_Struct (self, VertexBuffer, p);
    VertexArray*  colors;
    __TRY__;
    colors = p->getColors ();
    __CATCH__;
    return colors ? (VALUE)colors->getExportedEntity() : Qnil;
}

VALUE ruby_VertexBuffer_get_default_color (VALUE self)
{
    VertexBuffer* p;
    Data_Get_Struct (self, VertexBuffer, p);
    int argb;
    __TRY__;
    argb = p->getDefaultColor ();
    __CATCH__;
    return INT2NUM(argb);
}

VALUE ruby_VertexBuffer_get_normals (VALUE self)
{
    VertexBuffer* p;
    Data_Get_Struct (self, VertexBuffer, p);
    VertexArray*  normals;
    __TRY__;
    normals = p->getNormals ();
    __CATCH__;
    return normals ? (VALUE)normals->getExportedEntity() : Qnil;
}

VALUE ruby_VertexBuffer_get_positions (VALUE self)
{
    VertexBuffer* p;
    Data_Get_Struct (self, VertexBuffer, p);
    VertexArray*  positions;
    float         scale_bias[4];
    __TRY__;
    positions = p->getPositions (scale_bias);
    __CATCH__;

    VALUE val_ret = rb_ary_new2 (3);
    if (positions) {
        rb_ary_push (val_ret, (VALUE)positions->getExportedEntity());
        rb_ary_push (val_ret, rb_float_new(scale_bias[0]));
        VALUE val_bias = rb_ary_new2 (3);
        rb_ary_push (val_bias, rb_float_new(scale_bias[1]));
        rb_ary_push (val_bias, rb_float_new(scale_bias[2]));
        rb_ary_push (val_bias, rb_float_new(scale_bias[3]));
        rb_ary_push (val_ret, val_bias);
    }
    return val_ret;
}

VALUE ruby_VertexBuffer_get_tex_coords (VALUE self)
{
    VertexBuffer* p;
    Data_Get_Struct (self, VertexBuffer, p);
    TextureAccessor* accessor;
    VALUE        val_accessor = Data_Make_Struct (rb_cVertexBuffer_TextureAccessor, TextureAccessor, 0, -1, accessor);
    accessor->vertex_buffer = p;
    return val_accessor;
}

VALUE ruby_VertexBuffer_set_colors (VALUE self, VALUE val_colors)
{
    VertexBuffer* p;
    Data_Get_Struct (self, VertexBuffer, p);
    VertexArray* colors;
    Data_Get_Struct (val_colors, VertexArray, colors);
    __TRY__;
    p->setColors (colors);
    __CATCH__;
    return Qnil;
}

VALUE ruby_VertexBuffer_set_default_color (VALUE self, VALUE val_argb)
{
    VertexBuffer* p;
    Data_Get_Struct (self, VertexBuffer, p);
    int argb = NUM2INT (val_argb);
    __TRY__;
    p->setDefaultColor (argb);
    __CATCH__;
    return Qnil;
}

VALUE ruby_VertexBuffer_set_normals (VALUE self, VALUE val_normals)
{
    VertexBuffer* p;
    Data_Get_Struct (self, VertexBuffer, p);
    VertexArray* normals;
    Data_Get_Struct (val_normals, VertexArray, normals);
    __TRY__;
    p->setNormals (normals);
    __CATCH__;
    return Qnil;
}

VALUE ruby_VertexBuffer_set_positions (VALUE self, VALUE val_args)
{
    VALUE val_positions = rb_ary_entry (val_args, 0);
    VALUE val_scale     = rb_ary_entry (val_args, 1);
    VALUE val_bias      = rb_ary_entry (val_args, 2);
    VertexBuffer* p;
    Data_Get_Struct (self, VertexBuffer, p);
    VertexArray* positions;
    Data_Get_Struct (val_positions, VertexArray, positions);
    float scale = NUM2DBL(val_scale); 
    float bias[3];
    bias[0] = NUM2DBL(rb_ary_entry(val_bias, 0));
    bias[1] = NUM2DBL(rb_ary_entry(val_bias, 1));
    bias[2] = NUM2DBL(rb_ary_entry(val_bias, 2));
    __TRY__;
    p->setPositions (positions, scale, bias);
    __CATCH__;
    return Qnil;
}

/**
 * VertexBuffer_TextureAccessor
 */

VALUE ruby_VertexBuffer_TextureAccessor_get_tex_coords (VALUE self, VALUE val_index)
{
    TextureAccessor* p;
    Data_Get_Struct (self, TextureAccessor, p);
    int   index = NUM2INT (val_index);
    float scale_bias[4];
    VertexArray* tex_coords;
    __TRY__;
    tex_coords = p->vertex_buffer->getTexCoords (index, scale_bias);
    __CATCH__;
    VALUE val_scale = rb_float_new(scale_bias[0]);
    VALUE val_bias  = rb_ary_new2 (3);
    VALUE val_ret   = rb_ary_new2 (3);
    if (tex_coords) {
        rb_ary_push (val_ret,  (VALUE)tex_coords->getExportedEntity());
        rb_ary_push (val_ret,  val_scale);
        rb_ary_push (val_bias, rb_float_new(scale_bias[1]));
        rb_ary_push (val_bias, rb_float_new(scale_bias[2]));
        rb_ary_push (val_bias, rb_float_new(scale_bias[3]));
        rb_ary_push (val_ret,  val_bias);
    } else {
        rb_ary_push (val_ret, Qnil);
        rb_ary_push (val_ret, Qnil);
        rb_ary_push (val_ret, Qnil);
    }

    return val_ret;
}

VALUE ruby_VertexBuffer_TextureAccessor_set_tex_coords (VALUE self, VALUE val_index, VALUE val_args)
{
    VALUE val_tex_coords = rb_ary_entry(val_args, 0);
    VALUE val_scale      = rb_ary_entry(val_args, 1);
    VALUE val_bias       = rb_ary_entry(val_args, 2);
    TextureAccessor* p;
    Data_Get_Struct (self, TextureAccessor, p);
    int   index          = NUM2INT(val_index);
    float scale          = NUM2DBL(val_scale);
    float bias[4];
    bias[0] = NUM2DBL(rb_ary_entry(val_bias, 0));
    bias[1] = NUM2DBL(rb_ary_entry(val_bias, 1));
    bias[2] = NUM2DBL(rb_ary_entry(val_bias, 2));
    VertexArray* tex_coords;
    Data_Get_Struct (val_tex_coords, VertexArray, tex_coords);
    __TRY__;
    p->vertex_buffer->setTexCoords (index, tex_coords, scale, bias);
    __CATCH__;
    return Qnil;
}

void register_VertexBuffer ()
{
    // VertexBuffer
    rb_cVertexBuffer        = rb_define_class_under (rb_mM3G, "VertexBuffer",        rb_cObject3D);

    rb_define_alloc_func (rb_cVertexBuffer, ruby_VertexBuffer_allocate);
    rb_define_private_method (rb_cVertexBuffer, "initialize", (VALUE(*)(...))ruby_VertexBuffer_initialize, 0);

    rb_define_method (rb_cVertexBuffer, "colors",         (VALUE(*)(...))ruby_VertexBuffer_get_colors,        0);
    rb_define_method (rb_cVertexBuffer, "default_color",  (VALUE(*)(...))ruby_VertexBuffer_get_default_color, 0);
    rb_define_method (rb_cVertexBuffer, "normals",        (VALUE(*)(...))ruby_VertexBuffer_get_normals,       0);
    rb_define_method (rb_cVertexBuffer, "positions",      (VALUE(*)(...))ruby_VertexBuffer_get_positions,     0);
    rb_define_method (rb_cVertexBuffer, "tex_coords",     (VALUE(*)(...))ruby_VertexBuffer_get_tex_coords,    0);
    rb_define_method (rb_cVertexBuffer, "colors=",        (VALUE(*)(...))ruby_VertexBuffer_set_colors,        1);
    rb_define_method (rb_cVertexBuffer, "default_color=", (VALUE(*)(...))ruby_VertexBuffer_set_default_color, 1);
    rb_define_method (rb_cVertexBuffer, "normals=",       (VALUE(*)(...))ruby_VertexBuffer_set_normals,       1);
    rb_define_method (rb_cVertexBuffer, "positions=",     (VALUE(*)(...))ruby_VertexBuffer_set_positions,     1);

    // VertexBuffer_TextureAccessor
    rb_cVertexBuffer_TextureAccessor  = rb_define_class_under (rb_cVertexBuffer, "TextureAccessor", rb_cObject);

    rb_define_method (rb_cVertexBuffer_TextureAccessor, "[]",        (VALUE(*)(...))ruby_VertexBuffer_TextureAccessor_get_tex_coords,   1);
    rb_define_method (rb_cVertexBuffer_TextureAccessor, "[]=",       (VALUE(*)(...))ruby_VertexBuffer_TextureAccessor_set_tex_coords,   2);
}
