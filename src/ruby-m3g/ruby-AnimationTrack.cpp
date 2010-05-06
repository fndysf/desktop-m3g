#include <iostream>
#include "ruby.h"
#include "m3g.hpp"
#include "ruby-m3g.hpp"
using namespace m3g;
using namespace std;


VALUE ruby_AnimationTrack_free (AnimationTrack* ptr)
{
    ptr->~AnimationTrack ();
    ruby_xfree (ptr);
}

VALUE ruby_AnimationTrack_allocate (VALUE self)
{
    void* p = ruby_xmalloc (sizeof(AnimationTrack));
    return Data_Wrap_Struct (self, 0, ruby_AnimationTrack_free, p);
}

VALUE ruby_AnimationTrack_initialize (VALUE self, VALUE val_keyframe_sequence, VALUE val_property)
{
    AnimationTrack* p;
    KeyframeSequence* key_seq;
    int property;

    Data_Get_Struct (self, AnimationTrack, p);
    Data_Get_Struct (val_keyframe_sequence, KeyframeSequence, key_seq);
    property = INT2FIX (val_property);

    new (p) AnimationTrack (key_seq, property);
    p->setExportedEntity ((void*)self);
    return self;
}

VALUE ruby_AnimationTrack_get_controller (VALUE self)
{
  AnimationTrack* p;
  AnimationController* anim_controller;

  Data_Get_Struct (self, AnimationTrack, p);

  anim_controller = p->getController ();

  if (anim_controller)
    return (VALUE)anim_controller->getExportedEntity();
  else
    return Qnil;
}

VALUE ruby_AnimationTrack_get_keyframe_sequence (VALUE self)
{
  AnimationTrack* p;
  KeyframeSequence* key_seq;

  Data_Get_Struct (self, AnimationTrack, p);

  key_seq = p->getKeyframeSequence ();

  if (key_seq)
    return (VALUE)key_seq->getExportedEntity();
  else
    return Qnil;
}

VALUE ruby_AnimationTrack_get_target_property (VALUE self)
{
  AnimationTrack* p;
  int property;

  Data_Get_Struct (self, AnimationTrack, p);

  property = p->getTargetProperty ();
  
  return INT2FIX(property);
}

VALUE ruby_AnimationTrack_set_controller (VALUE self, VALUE val_controller)
{
  AnimationTrack* p;
  AnimationController* anim_controller;

  Data_Get_Struct (self, AnimationTrack, p);
  Data_Get_Struct (val_controller, AnimationController, anim_controller);

  p->setController (anim_controller);

  return Qnil;
}


void register_AnimationTrack ()
{
    // AnimationTrack
    rb_define_const (rb_cAnimationTrack, "ALPHA",          INT2FIX(AnimationTrack::ALPHA));
    rb_define_const (rb_cAnimationTrack, "AMBIENT_COLOR",  INT2FIX(AnimationTrack::AMBIENT_COLOR));
    rb_define_const (rb_cAnimationTrack, "COLOR",          INT2FIX(AnimationTrack::COLOR));
    rb_define_const (rb_cAnimationTrack, "CROP",           INT2FIX(AnimationTrack::CROP));
    rb_define_const (rb_cAnimationTrack, "DENSITY",        INT2FIX(AnimationTrack::DENSITY));
    rb_define_const (rb_cAnimationTrack, "DIFFUSE_COLOR",  INT2FIX(AnimationTrack::DIFFUSE_COLOR));
    rb_define_const (rb_cAnimationTrack, "EMISSIVE_COLOR", INT2FIX(AnimationTrack::EMISSIVE_COLOR));
    rb_define_const (rb_cAnimationTrack, "FAR_DISTANCE",   INT2FIX(AnimationTrack::FAR_DISTANCE));
    rb_define_const (rb_cAnimationTrack, "FIELD_OF_VIEW",  INT2FIX(AnimationTrack::FIELD_OF_VIEW));
    rb_define_const (rb_cAnimationTrack, "INTENSITY",      INT2FIX(AnimationTrack::INTENSITY));
    rb_define_const (rb_cAnimationTrack, "MORPH_WEIGHTS",  INT2FIX(AnimationTrack::MORPH_WEIGHTS));
    rb_define_const (rb_cAnimationTrack, "NEAR_DISTANCE",  INT2FIX(AnimationTrack::NEAR_DISTANCE));
    rb_define_const (rb_cAnimationTrack, "ORIENTATION",    INT2FIX(AnimationTrack::ORIENTATION));
    rb_define_const (rb_cAnimationTrack, "PICKABILITY",    INT2FIX(AnimationTrack::PICKABILITY));
    rb_define_const (rb_cAnimationTrack, "SCALE",          INT2FIX(AnimationTrack::SCALE));
    rb_define_const (rb_cAnimationTrack, "SHININESS",      INT2FIX(AnimationTrack::SHININESS));
    rb_define_const (rb_cAnimationTrack, "SPECULAR_COLOR", INT2FIX(AnimationTrack::SPECULAR_COLOR));
    rb_define_const (rb_cAnimationTrack, "SPOT_ANGLE",     INT2FIX(AnimationTrack::SPOT_ANGLE));
    rb_define_const (rb_cAnimationTrack, "SPOT_EXPONENT",  INT2FIX(AnimationTrack::SPOT_EXPONENT));
    rb_define_const (rb_cAnimationTrack, "TRANSLATION",    INT2FIX(AnimationTrack::TRANSLATION));
    rb_define_const (rb_cAnimationTrack, "VISIBILITY",     INT2FIX(AnimationTrack::VISIBILITY));

    rb_define_alloc_func (rb_cAnimationTrack, ruby_AnimationTrack_allocate);
    rb_define_private_method (rb_cAnimationTrack, "initialize", (VALUE(*)(...))ruby_AnimationTrack_initialize, 2);

    rb_define_method (rb_cAnimationTrack, "controller",        (VALUE(*)(...))ruby_AnimationTrack_get_controller, 0); 
    rb_define_method (rb_cAnimationTrack, "keyframe_sequence", (VALUE(*)(...))ruby_AnimationTrack_get_keyframe_sequence, 0); 
    rb_define_method (rb_cAnimationTrack, "target_property",   (VALUE(*)(...))ruby_AnimationTrack_get_target_property, 0); 
    rb_define_method (rb_cAnimationTrack, "controller=",        (VALUE(*)(...))ruby_AnimationTrack_set_controller, 1); 
}
