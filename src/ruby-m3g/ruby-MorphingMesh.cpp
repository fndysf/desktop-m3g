#include <iostream>
#include "ruby.h"
#include "m3g.hpp"
#include "ruby-m3g.hpp"
using namespace m3g;
using namespace std;

namespace {
    struct MorphTargetAccessor {
        MorphingMesh* mesh;
    };
    VALUE rb_cMorphingMesh_MorphTargetAccessor;
}

VALUE ruby_MorphingMesh_free (MorphingMesh* ptr)
{
    __TRY__;
    ptr->~MorphingMesh ();
    __CATCH__;
    ruby_xfree (ptr);
}

VALUE ruby_MorphingMesh_allocate (VALUE self)
{
    void* p = ruby_xmalloc (sizeof(MorphingMesh));
    return Data_Wrap_Struct (self, 0, ruby_MorphingMesh_free, p);
}

VALUE ruby_MorphingMesh_initialize (VALUE self, VALUE val_base, VALUE val_targets, VALUE val_submeshes, VALUE val_appearances)
{
    MorphingMesh* p;
    VertexBuffer* base;
    Data_Get_Struct (self, MorphingMesh, p);
    Data_Get_Struct (val_base, VertexBuffer, base);
    
    if (TYPE(val_targets) == T_ARRAY) {
        int            num_target  = RARRAY_LEN(val_targets);
        int            num_submesh = RARRAY_LEN(val_submeshes);
        VertexBuffer** targets     = (VertexBuffer**)ruby_xmalloc (sizeof(VertexBuffer*)*num_target);
        IndexBuffer**  submeshes   = (IndexBuffer**)ruby_xmalloc (sizeof(IndexBuffer*)*num_submesh);
        Appearance**   appearances = (Appearance**)ruby_xmalloc (sizeof(Appearance*)*num_submesh);
        for (int i = 0; i < num_target; i++) {
            Data_Get_Struct (rb_ary_entry(val_targets, i), VertexBuffer, targets[i]);
        }
        for (int i = 0; i < num_submesh; i++) {
            Data_Get_Struct (rb_ary_entry(val_submeshes, i), IndexBuffer, submeshes[i]);
            Data_Get_Struct (rb_ary_entry(val_appearances, i), Appearance, appearances[i]);
        }
        __TRY__;
        new (p) MorphingMesh (base,
                              num_target,
                              targets,
                              num_submesh,
                              submeshes,
                              appearances);
        __CATCH__;
    } else {
        int            num_target = RARRAY_LEN(val_targets);
        VertexBuffer** targets    = (VertexBuffer**)ruby_xmalloc (sizeof(VertexBuffer*)*num_target);
        IndexBuffer*   submesh;
        Appearance*    appearance;
        for (int i = 0; i < num_target; i++) {
            Data_Get_Struct (rb_ary_entry(val_targets, i), VertexBuffer, targets[i]);
        }
        Data_Get_Struct (val_submeshes, IndexBuffer, submesh);
        Data_Get_Struct (val_appearances, Appearance, appearance);
        __TRY__;
        new (p) MorphingMesh (base,
                              num_target,
                              targets,
                              submesh,
                              appearance);
        __CATCH__;
    }
    return Qnil;
}

VALUE ruby_MorphingMesh_get_morph_target (VALUE self)
{
    MorphingMesh* p;
    Data_Get_Struct (self, MorphingMesh, p);
    MorphTargetAccessor* accessor;
    VALUE            val_accessor = Data_Make_Struct (rb_cMorphingMesh_MorphTargetAccessor, MorphTargetAccessor, 0, -1, accessor);
    accessor->mesh = p;
    return val_accessor;
}

VALUE ruby_MorphingMesh_get_morph_target_count (VALUE self)
{
    MorphingMesh* p;
    Data_Get_Struct (self, MorphingMesh, p);
    int count;
    __TRY__;
    count = p->getMorphTargetCount();
    __CATCH__;
    return INT2NUM(count);
}

VALUE ruby_MorphingMesh_get_weights (VALUE self)
{
    MorphingMesh* p;
    Data_Get_Struct (self, MorphingMesh, p);
    int count;
    __TRY__;
    count = p->getMorphTargetCount();
    __CATCH__;

    float* weights = (float*)ruby_xmalloc (sizeof(float)*count);
    __TRY__;
    p->getWeights (weights);
    __CATCH__;

    VALUE val_weights = rb_ary_new2 (count);
    for (int i = 0; i < count; i++) {
        rb_ary_push (val_weights, rb_float_new(weights[i]));
    }
    ruby_xfree (weights);

    return Qnil;
}

VALUE ruby_MorphingMesh_set_weights (VALUE self, VALUE val_weights)
{
    MorphingMesh* p;
    Data_Get_Struct (self, MorphingMesh, p);
    int count = RARRAY_LEN(val_weights);

    float* weights = (float*)ruby_xmalloc (sizeof(float)*count);
    for (int i = 0; i < count; i++) {
        weights[i] = NUM2DBL (rb_ary_entry(val_weights, i));
    }
    __TRY__;
    p->setWeights (count, weights);
    __CATCH__;

    return Qnil;
}

/**
 * MorphingMesh_MorphTarget
 */
VALUE ruby_MorphTargetAccessor_get_target (VALUE self, VALUE val_index)
{
    MorphTargetAccessor* p;
    Data_Get_Struct (self, MorphTargetAccessor, p);
    int           index    = NUM2INT (val_index);
    VertexBuffer* vertices;
    __TRY__;
    vertices = p->mesh->getMorphTarget (index);
    __CATCH__;

    return vertices ? (VALUE)vertices->getExportedEntity() : Qnil;
}


void register_MorphingMesh ()
{
    // MorphingMesh
    rb_cMorphingMesh        = rb_define_class_under (rb_mM3G, "MorphingMesh",        rb_cMesh);

    rb_define_alloc_func (rb_cMorphingMesh, ruby_MorphingMesh_allocate);
    rb_define_private_method (rb_cMorphingMesh, "initialize", (VALUE(*)(...))ruby_MorphingMesh_initialize, -1);

    rb_define_method (rb_cMorphingMesh, "morph_target",       (VALUE(*)(...))ruby_MorphingMesh_get_morph_target,       1);
    rb_define_method (rb_cMorphingMesh, "morph_target_count", (VALUE(*)(...))ruby_MorphingMesh_get_morph_target_count, 0);
    rb_define_method (rb_cMorphingMesh, "weights",            (VALUE(*)(...))ruby_MorphingMesh_get_weights,            0);
    rb_define_method (rb_cMorphingMesh, "weights=",           (VALUE(*)(...))ruby_MorphingMesh_set_weights,            1);

    // MorphingMesh_MorphTargetAccessor
    rb_cMorphingMesh_MorphTargetAccessor  = rb_define_class_under (rb_cMorphingMesh, "MorphTargetAccessor", rb_cObject);

    rb_define_method (rb_cMorphingMesh_MorphTargetAccessor, "[]",     (VALUE(*)(...))ruby_MorphTargetAccessor_get_target, 1);
}
