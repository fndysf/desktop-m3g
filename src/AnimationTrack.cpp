#include <iostream>
#include "m3g/AnimationTrack.hpp"
#include "m3g/KeyframeSequence.hpp"
#include "m3g/AnimationController.hpp"
#include "m3g/Exception.hpp"
using namespace std;
using namespace m3g;

const int AnimationTrack:: ALPHA;
const int AnimationTrack:: AMBIENT_COLOR;
const int AnimationTrack:: COLOR;
const int AnimationTrack:: CROP;
const int AnimationTrack:: DENSITY;
const int AnimationTrack:: DIFFUSE_COLOR;
const int AnimationTrack:: EMISSIVE_COLOR;
const int AnimationTrack:: FAR_DISTANCE;
const int AnimationTrack:: FIELD_OF_VIEW;
const int AnimationTrack:: INTENSITY;
const int AnimationTrack:: MORPH_WEIGHTS;
const int AnimationTrack:: NEAR_DISTANCE;
const int AnimationTrack:: ORIENTATION;
const int AnimationTrack:: PICKABILITY;
const int AnimationTrack:: SCALE;
const int AnimationTrack:: SHININESS;
const int AnimationTrack:: SPECULAR_COLOR;
const int AnimationTrack:: SPOT_ANGLE;
const int AnimationTrack:: SPOT_EXPONENT;
const int AnimationTrack:: TRANSLATION;
const int AnimationTrack:: VISIBILITY;

static
bool is_property_has_valid_component (int property, int component_count)
{
    switch (property) {
    case AnimationTrack:: ALPHA:         return (component_count == 1) ? true : false;
    case AnimationTrack:: AMBIENT_COLOR: return (component_count == 3) ? true : false;
    case AnimationTrack:: COLOR:         return (component_count == 3) ? true : false;
    case AnimationTrack:: CROP:          return (component_count == 2 || component_count == 4) ? true : false;
    case AnimationTrack:: DENSITY:       return (component_count == 1) ? true : false;
    case AnimationTrack:: DIFFUSE_COLOR: return (component_count == 3) ? true : false;
    case AnimationTrack:: EMISSIVE_COLOR:return (component_count == 3) ? true : false;
    case AnimationTrack:: FAR_DISTANCE:  return (component_count == 1) ? true : false;
    case AnimationTrack:: FIELD_OF_VIEW: return (component_count == 1) ? true : false;
    case AnimationTrack:: INTENSITY:     return (component_count == 1) ? true : false;
    case AnimationTrack:: MORPH_WEIGHTS: return (component_count >= 1) ? true : false;
    case AnimationTrack:: NEAR_DISTANCE: return (component_count == 1) ? true : false;
    case AnimationTrack:: ORIENTATION:   return (component_count == 4) ? true : false;
    case AnimationTrack:: PICKABILITY:   return (component_count == 1) ? true : false;
    case AnimationTrack:: SCALE:         return (component_count == 1 || component_count == 3) ? true : false;
    case AnimationTrack:: SHININESS:     return (component_count == 1) ? true : false;
    case AnimationTrack:: SPECULAR_COLOR:return (component_count == 3) ? true : false;
    case AnimationTrack:: SPOT_ANGLE:    return (component_count == 1) ? true : false;
    case AnimationTrack:: SPOT_EXPONENT: return (component_count == 1) ? true : false;
    case AnimationTrack:: TRANSLATION:   return (component_count == 3) ? true : false;
    case AnimationTrack:: VISIBILITY:    return (component_count == 1) ? true : false;
    default: throw InternalException (__FILE__, __func__, "Unknown property, property=%d", property);
    }
}


AnimationTrack:: AnimationTrack (KeyframeSequence* sequence_, int property_) :
    property(0), keyframe_sequence(0), animation_controller(0)
{
    if (sequence_ == NULL) {
        throw NullPointerException (__FILE__, __func__, "KeyframeSequence is NULL.");
    }
    if (property_ < ALPHA || property_ > VISIBILITY) {
        throw IllegalArgumentException (__FILE__, __func__, "Property is unkwon, property=%d", property_);
    }
    if (!is_property_has_valid_component (property_, sequence_->getComponentCount())) {
        throw IllegalArgumentException (__FILE__, __func__, "Component count of KeyframeSequence is invalid for this property, count=%d, property=%d", sequence_->getComponentCount(), property_);
    }


    keyframe_sequence = sequence_;
    property          = property_;
}

AnimationTrack:: ~AnimationTrack ()
{
}

AnimationTrack* AnimationTrack:: duplicate () const
{
    AnimationTrack* anim_track = new AnimationTrack (keyframe_sequence, property);
    this->Object3D      :: copy (anim_track);
    this->AnimationTrack:: copy (anim_track);
    return anim_track;
}

void AnimationTrack:: copy (AnimationTrack* anim_track) const
{
    if (anim_track == NULL) {
        throw NullPointerException (__FILE__, __func__, "AnimationTrack is NUll.");
    }
    anim_track->property             = property;
    anim_track->keyframe_sequence    = keyframe_sequence;
    anim_track->animation_controller = animation_controller;
}

void AnimationTrack:: mark (void(*func)(void*)) const
{
    if (func == NULL) {
        return;
    }

    Object:: mark (func);
    if (keyframe_sequence) {
        keyframe_sequence->mark (func);
    }
    if (animation_controller) {
        animation_controller->mark (func);
    }
    
}

AnimationController* AnimationTrack:: getController () const
{
    return animation_controller;
}

KeyframeSequence* AnimationTrack:: getKeyframeSequence () const
{
    return keyframe_sequence;
}

int AnimationTrack:: getTargetProperty () const
{
    return property;
}

void AnimationTrack:: setController (AnimationController* controller)
{
    animation_controller = controller;
}

static
const char* property_to_string (int property)
{
    switch (property) {
    case AnimationTrack::ALPHA:          return "ALPHA";
    case AnimationTrack::AMBIENT_COLOR:  return "AMBIENT_COLOR";
    case AnimationTrack::COLOR:          return "COLOR";
    case AnimationTrack::CROP:           return "CROP";
    case AnimationTrack::DENSITY:        return "DENSITY";
    case AnimationTrack::DIFFUSE_COLOR:  return "DIFFUSE_COLOR";
    case AnimationTrack::EMISSIVE_COLOR: return "EMISSIVE_COLOR";
    case AnimationTrack::FAR_DISTANCE:   return "FAR_DISTANCE";
    case AnimationTrack::FIELD_OF_VIEW:  return "FIELD_OF_VIEW";
    case AnimationTrack::INTENSITY:      return "INTENSITY";
    case AnimationTrack::MORPH_WEIGHTS:  return "MORPH_WEIGHTS";
    case AnimationTrack::NEAR_DISTANCE:  return "NEAR_DISTANCE";
    case AnimationTrack::ORIENTATION:    return "ORIENTATION";
    case AnimationTrack::PICKABILITY:    return "PICKABILITY";
    case AnimationTrack::SCALE:          return "SCALE";
    case AnimationTrack::SHININESS:      return "SHININESS";
    case AnimationTrack::SPECULAR_COLOR: return "SPECULAR_COLOR";
    case AnimationTrack::SPOT_ANGLE:     return "SPOT_ANGLE";
    case AnimationTrack::SPOT_EXPONENT:  return "SPOT_EXPONENT";
    case AnimationTrack::TRANSLATION:    return "TRANSLATION";
    case AnimationTrack::VISIBILITY:     return "VISIBILITY";
    default: return "Unknown";
    }
}

int AnimationTrack:: getReferences (Object3D** references) const
{
    int n = 0;
    if (keyframe_sequence)
        n++;
    if (animation_controller)
        n++;
    
    if (references) {
        int i = 0;
        if (keyframe_sequence)
            references[i++] = keyframe_sequence;
        if (animation_controller)
            references[i++] = animation_controller;
    }
    return n;
}


std::ostream& AnimationTrack:: print (std::ostream& out) const
{
    out << "AnimationTrack: ";
    out << "  property=" << property_to_string(property);
    out << ", keyframe_sequence=[";
    for (int i = 0;  i < keyframe_sequence->getKeyframeCount(); i++) {
        out << i << ":" << keyframe_sequence->getKeyframe (i, 0) << ",";
    }
    out << "]";
    if (animation_controller)
        out << ", animation_controller=0x" << hex << animation_controller << dec;
    else
        out << ", animation_controller=0";
    return out;
}

std::ostream& operator<< (std::ostream& out, const AnimationTrack& track)
{
    return track.print(out);
}
