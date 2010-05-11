#include <iostream>
#include "ruby.h"
#include "m3g.hpp"
#include "ruby-m3g.hpp"
using namespace m3g;
using namespace std;

VALUE ruby_VertexArray_free (VertexArray* ptr)
{
    __TRY__;
    ptr->~VertexArray ();
    __CATCH__;
    ruby_xfree (ptr);
}

VALUE ruby_VertexArray_allocate (VALUE self)
{
    void* p = ruby_xmalloc (sizeof(VertexArray));
    return Data_Wrap_Struct (self, 0, ruby_VertexArray_free, p);
}

VALUE ruby_VertexArray_initialize (VALUE self, VALUE val_num_vertices, VALUE val_num_components, VALUE val_component_size)
{
    VertexArray* p;
    Data_Get_Struct (self, VertexArray, p);
    int num_vertices    = NUM2INT (val_num_vertices);
    int num_components  = NUM2INT (val_num_components);
    int component_size  = NUM2INT (val_component_size);
    __TRY__;
    new (p) VertexArray (num_vertices, num_components, component_size);
    __CATCH__;
    p->setExportedEntity ((void*)self);
    return self;
}

VALUE ruby_VertexArray_get (int argc, VALUE* argv, VALUE self)
{
    VertexArray* p;
    Data_Get_Struct (self, VertexArray, p);
    VALUE val_first_vertex;
    VALUE val_num_vertices;
    int   first_vertex;
    int   num_vertices;
    int   num = rb_scan_args (argc, argv, "11", &val_first_vertex, &val_num_vertices);
    switch (num) {
    case 1:
        first_vertex = NUM2INT (val_first_vertex);
        num_vertices = 1;
        break;
    case 2:
        first_vertex = NUM2INT (val_first_vertex);
        num_vertices = NUM2INT (val_num_vertices);
        break;
    default:
        rb_raise (rb_eIllegalArgumentException, "Num of argument is invalid, num=%d.", num);
    }

    int component_size  = p->getComponentType ();
    int component_count = p->getComponentCount ();
    int num_components  = num_vertices * component_count;

    VALUE val_values = rb_ary_new2 (num_components);
    switch (component_size) {
    case 1: {
        char* values = (char*)ruby_xmalloc (sizeof(char)*num_components);
        __TRY__;
        p->get (first_vertex, num_vertices, values);
        __CATCH__;
        for (int i = 0; i < num_components; i++) {
            rb_ary_push (val_values, INT2NUM(values[i]));
        }
        ruby_xfree (values);
        break;
    }
    case 2: {
        short* values = (short*)ruby_xmalloc (sizeof(short)*num_components);
        __TRY__;
        p->get (first_vertex, num_vertices, values);
        __CATCH__;
        for (int i = 0; i < num_components; i++) {
            rb_ary_push (val_values, INT2NUM(values[i]));
        }
        ruby_xfree (values);
        break;
    }
    case 4: {
        float* values = (float*)ruby_xmalloc (sizeof(float)*num_components);
        __TRY__;
        p->get (first_vertex, num_vertices, values);
        __CATCH__;
        for (int i = 0; i < num_components; i++) {
            rb_ary_push (val_values, rb_float_new(values[i]));
        }
        ruby_xfree (values);
        break;
    }
    default: {
        rb_raise (rb_eInternalException, "Component size is invalid. size=%d", component_size);
    }
    }


    return val_values;
}

VALUE ruby_VertexArray_get_component_count (VALUE self)
{
    VertexArray* p;
    Data_Get_Struct (self, VertexArray, p);
    int component_count;
    __TRY__;
    component_count = p->getComponentCount ();
    __CATCH__;
    return INT2NUM(component_count);
}

VALUE ruby_VertexArray_get_component_type (VALUE self)
{
    VertexArray* p;
    Data_Get_Struct (self, VertexArray, p);
    int component_type;
    __TRY__;
    component_type = p->getComponentType ();
    __CATCH__;
    return INT2NUM(component_type);
}

VALUE ruby_VertexArray_get_vertex_count (VALUE self)
{
    VertexArray* p;
    Data_Get_Struct (self, VertexArray, p);
    int vertex_count;
    __TRY__;
    vertex_count = p->getVertexCount ();
    __CATCH__;
    return INT2NUM(vertex_count);
}

VALUE ruby_VertexArray_set (int argc, VALUE* argv, VALUE self)
{
    VertexArray* p;
    Data_Get_Struct (self, VertexArray, p);
    int   first_vertex;
    int   num_vertices;
    VALUE val_values;
    VALUE val_arg1, val_arg2, val_arg3;

    int   num = rb_scan_args (argc, argv, "21", &val_arg1, &val_arg2, &val_arg3);
    switch (num) {
    case 2:
        first_vertex     = NUM2INT(val_arg1);
        num_vertices     = 1;
        val_values       = val_arg2;
        break;
    case 3:
        first_vertex     = NUM2INT(val_arg1);
        num_vertices     = NUM2INT(val_arg2);
        val_values       = val_arg3;
        break;
    default:
        rb_raise (rb_eInternalException, "Num of argument is invalid. num=%d", num);
    }        

    int component_size  = p->getComponentType ();
    int component_count = p->getComponentCount ();
    int num_components  = num_vertices * component_count;

    //cout << first_vertex << ", " << num_vertices << ", " << component_size << "\n";

    switch (component_size) {
    case 1: {
        char* values = (char*)ruby_xmalloc(sizeof(char)*num_components);
        for (int i = 0; i < num_components; i++) {
            values[i] = NUM2INT(rb_ary_entry(val_values, i));
        }
        __TRY__;
        p->set (first_vertex, num_vertices, values);
        __CATCH__;
        ruby_xfree (values);
        break;
    }
    case 2: {
        short* values = (short*)ruby_xmalloc(sizeof(short)*num_components);
        for (int i = 0; i < num_components; i++) {
            values[i] = NUM2INT(rb_ary_entry(val_values, i));
        }
        __TRY__;
        p->set (first_vertex, num_vertices, values);
        __CATCH__;
        ruby_xfree (values);
        break;
    }
    case 4: {
        float* values = (float*)ruby_xmalloc(sizeof(float)*num_components);
        for (int i = 0; i < num_components; i++) {
            values[i] = NUM2DBL(rb_ary_entry(val_values, i));
        }
        __TRY__;
        p->set (first_vertex, num_vertices, values);
        __CATCH__;
        ruby_xfree (values);
        break;
    }
    default: {
        rb_raise (rb_eInternalException, "Component size is invalid, size=%d.", component_size);
    }
    }

    return Qnil;
}

VALUE ruby_VertexArray_convert (VALUE self, VALUE val_component_type)
{
    VertexArray* p;
    Data_Get_Struct (self, VertexArray, p);
    int component_type = NUM2INT(val_component_type);
    __TRY__;
    p->convert (component_type);
    __CATCH__;
    return Qnil;
}


void register_VertexArray ()
{
    // VertexArray
    rb_cVertexArray         = rb_define_class_under (rb_mM3G, "VertexArray",         rb_cObject3D);

    rb_define_alloc_func (rb_cVertexArray, ruby_VertexArray_allocate);
    rb_define_private_method (rb_cVertexArray, "initialize", (VALUE(*)(...))ruby_VertexArray_initialize, 3);

    rb_define_method (rb_cVertexArray, "component_count", (VALUE(*)(...))ruby_VertexArray_get_component_count, 0);
    rb_define_method (rb_cVertexArray, "component_type",  (VALUE(*)(...))ruby_VertexArray_get_component_type,  0);
    rb_define_method (rb_cVertexArray, "vertex_count",    (VALUE(*)(...))ruby_VertexArray_get_vertex_count,    0);
    rb_define_method (rb_cVertexArray, "convert",         (VALUE(*)(...))ruby_VertexArray_convert,             1);
    rb_define_method (rb_cVertexArray, "[]",              (VALUE(*)(...))ruby_VertexArray_get,                -1);
    rb_define_method (rb_cVertexArray, "[]=",             (VALUE(*)(...))ruby_VertexArray_set,                -1);
}
