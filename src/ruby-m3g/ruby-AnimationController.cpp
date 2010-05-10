#include <iostream>
#include "ruby.h"
#include "m3g.hpp"
#include "ruby-m3g.hpp"
using namespace m3g;
using namespace std;

namespace {
	struct ActiveIntervalAccessor {
		AnimationController* animation_controller;
	};
	VALUE rb_cAnimationController_ActiveIntervalAccessor;
};

VALUE ruby_AnimationController_free (AnimationController* ptr)
{
    ptr->~AnimationController ();
    ruby_xfree (ptr);
}

VALUE ruby_AnimationController_allocate (VALUE self)
{
    void* p = ruby_xmalloc (sizeof(AnimationController));
    return Data_Wrap_Struct (self, 0, ruby_AnimationController_free, p);
}

VALUE ruby_AnimationController_initialize (VALUE self)
{
    AnimationController* p;
    Data_Get_Struct (self, AnimationController, p);
    new (p) AnimationController;
    p->setExportedEntity ((void*)self);
    return self;
}

VALUE ruby_AnimationController_get_active_interval (VALUE self)
{
  AnimationController* p;
  Data_Get_Struct (self, AnimationController, p);
  ActiveIntervalAccessor* accessor;
  VALUE                   val_accessor = Data_Make_Struct (rb_cAnimationController_ActiveIntervalAccessor, ActiveIntervalAccessor, 0, -1, accessor);
  accessor->animation_controller = p;
  return val_accessor;
}

VALUE ruby_AnimationController_get_position (VALUE self, VALUE val_world_time)
{
  AnimationController* p;
  Data_Get_Struct (self, AnimationController, p);
  int world_time = FIX2INT(val_world_time);

  float sequence_time = sequence_time = p->getPosition (world_time);

  return rb_float_new (sequence_time);
}

VALUE ruby_AnimationController_get_ref_world_time (VALUE self)
{
  AnimationController* p;
  Data_Get_Struct (self, AnimationController, p);

  int ref_world_time = p->getRefWorldTime ();

  return INT2FIX(ref_world_time);
}

VALUE ruby_AnimationController_get_speed (VALUE self)
{
  AnimationController* p;
  Data_Get_Struct (self, AnimationController, p);

  float speed = p->getSpeed ();

  return rb_float_new(speed);
}

VALUE ruby_AnimationController_get_weight (VALUE self)
{
  AnimationController* p;
  Data_Get_Struct (self, AnimationController, p);

  float weight = p->getWeight ();

  return rb_float_new(weight);
}

VALUE ruby_AnimationController_set_active_interval (VALUE self, VALUE val_args)
{
  VALUE val_start = rb_ary_entry(val_args, 0);
  VALUE val_end   = rb_ary_entry(val_args, 1);
  AnimationController* p;
  Data_Get_Struct (self, AnimationController, p);
  int start = FIX2INT(val_start);
  int end   = FIX2INT(val_end);

  p->setActiveInterval (start, end);

  return Qnil;
}

VALUE ruby_AnimationController_set_position (VALUE self, VALUE val_args)
{
  VALUE val_sequence_time = rb_ary_entry(val_args, 0);
  VALUE val_world_time    = rb_ary_entry(val_args, 1);
  AnimationController* p;
  Data_Get_Struct (self, AnimationController, p);
  float sequence_time = NUMERIC2FLOAT(val_sequence_time);
  int  world_time     = FIX2INT(val_world_time);

  p->setPosition (sequence_time, world_time);

  return Qnil;
}

VALUE ruby_AnimationController_set_speed (VALUE self, VALUE val_args)
{
  VALUE val_speed      = rb_ary_entry(val_args, 0);
  VALUE val_world_time = rb_ary_entry(val_args, 1);
  AnimationController* p;
  Data_Get_Struct (self, AnimationController, p);
  float speed      = NUMERIC2FLOAT (val_speed);
  int   world_time = FIX2INT (val_world_time);

  p->setSpeed (speed, world_time);

  return Qnil;
}

VALUE ruby_AnimationController_set_weight (VALUE self, VALUE val_weight)
{
  AnimationController* p;
  Data_Get_Struct (self, AnimationController, p);
  int weight = FIX2INT(val_weight);

  p->setWeight (weight);

  return Qnil;
}

/**
 * AnimationController::ActiveIntervalAccessor
 */


VALUE ruby_AnimationController_ActiveIntervalAccessor_get_start (VALUE self)
{
  ActiveIntervalAccessor* p;
  Data_Get_Struct (self, ActiveIntervalAccessor, p);
  int start = p->animation_controller->getActiveIntervalStart ();
  return rb_float_new(start);
}

VALUE ruby_AnimationController_ActiveIntervalAccessor_get_end (VALUE self)
{
  ActiveIntervalAccessor* p;
  Data_Get_Struct (self, ActiveIntervalAccessor, p);
  int end = p->animation_controller->getActiveIntervalEnd ();
  return rb_float_new(end);
}

/**
 * AnimationControllerクラスのRubyへの登録.
 */
void register_AnimationController ()
{
    // AnimationController
    rb_cAnimationController = rb_define_class_under (rb_mM3G, "AnimationController", rb_cObject3D);

    rb_define_alloc_func (rb_cAnimationController, ruby_AnimationController_allocate);
    rb_define_private_method (rb_cAnimationController, "initialize", (VALUE(*)(...))ruby_AnimationController_initialize, 0);

    rb_define_method (rb_cAnimationController, "active_interval",  (VALUE(*)(...))ruby_AnimationController_get_active_interval, 0); 
    rb_define_method (rb_cAnimationController, "position",         (VALUE(*)(...))ruby_AnimationController_get_position,        1); 
    rb_define_method (rb_cAnimationController, "ref_world_time",   (VALUE(*)(...))ruby_AnimationController_get_ref_world_time,  0); 
    rb_define_method (rb_cAnimationController, "speed",            (VALUE(*)(...))ruby_AnimationController_get_speed,           0); 
    rb_define_method (rb_cAnimationController, "weight",           (VALUE(*)(...))ruby_AnimationController_get_weight,          0); 
    rb_define_method (rb_cAnimationController, "active_interval=", (VALUE(*)(...))ruby_AnimationController_set_active_interval, 1);
    rb_define_method (rb_cAnimationController, "position=",        (VALUE(*)(...))ruby_AnimationController_set_position,        1);
    rb_define_method (rb_cAnimationController, "speed=",           (VALUE(*)(...))ruby_AnimationController_set_speed,           1);
    rb_define_method (rb_cAnimationController, "weight=",          (VALUE(*)(...))ruby_AnimationController_set_weight,          1); 

    // AnimationController::ActiveIntervalAccessor
     rb_cAnimationController_ActiveIntervalAccessor  = rb_define_class_under (rb_cAnimationController, "ActiveIntervalAccessor", rb_cObject);

     rb_define_method (rb_cAnimationController_ActiveIntervalAccessor, "start",        (VALUE(*)(...))ruby_AnimationController_ActiveIntervalAccessor_get_start, 0);
     rb_define_method (rb_cAnimationController_ActiveIntervalAccessor, "end",        (VALUE(*)(...))ruby_AnimationController_ActiveIntervalAccessor_get_end,     0);

}
