#include <unittest++/UnitTest++.h>
#include <iostream>
#include "m3g/KeyframeSequence.hpp"
using namespace std;
using namespace m3g;


TEST (KeyframeSequence_default_variables)
{
    KeyframeSequence* key_seq = new KeyframeSequence (10, 3, KeyframeSequence::LINEAR);

    CHECK_EQUAL (10,                         key_seq->getKeyframeCount());
    CHECK_EQUAL (3,                          key_seq->getComponentCount());
    CHECK_EQUAL (KeyframeSequence::LINEAR,   key_seq->getInterpolationType());
    CHECK_EQUAL (KeyframeSequence::CONSTANT, key_seq->getRepeatMode());
    CHECK_EQUAL (0,                          key_seq->getValidRangeFirst());
    CHECK_EQUAL (9,                          key_seq->getValidRangeLast());
    for (int i = 0; i < 10; i++) {
        float value[3];
        CHECK_EQUAL (0, key_seq->getKeyframe (i, value));
        CHECK_EQUAL (0, value[0]);
        CHECK_EQUAL (0, value[1]);
        CHECK_EQUAL (0, value[2]);
    }

    delete key_seq;
}

TEST (KeyframeSequence_set_variables)
{
    KeyframeSequence* key_seq = new KeyframeSequence (10, 3, KeyframeSequence::LINEAR);
    key_seq->setDuration (1000);
    key_seq->setRepeatMode (KeyframeSequence::LOOP);
    key_seq->setValidRange (5,9);

    CHECK_EQUAL (10,   key_seq->getKeyframeCount());
    CHECK_EQUAL (3,    key_seq->getComponentCount());
    CHECK_EQUAL (KeyframeSequence::LINEAR,   key_seq->getInterpolationType());
    CHECK_EQUAL (KeyframeSequence::LOOP, key_seq->getRepeatMode());
    CHECK_EQUAL (1000, key_seq->getDuration());
    CHECK_EQUAL (5,    key_seq->getValidRangeFirst());
    CHECK_EQUAL (9,   key_seq->getValidRangeLast());

    for (int i = 0; i < 10; i++) {
        int   time      = i*10;
        float values[3] = {i,i+1,i+2};
        key_seq->setKeyframe (i,   time, values);
    }
    for (int i = 0; i < 10; i++) {
        float values[3];
        int   time = key_seq->getKeyframe (i, values);
        CHECK_EQUAL (i*10, time);
        CHECK_EQUAL (i   , values[0]);
        CHECK_EQUAL (i+1 , values[1]);
        CHECK_EQUAL (i+2 , values[2]);
    }

    delete key_seq;
}

TEST (KeyframeSequence_interpolate_step)
{
    KeyframeSequence* keyframe = new KeyframeSequence (3, 1, KeyframeSequence::STEP);
    keyframe->setDuration  (1000);
    keyframe->setValidRange (0,2);

    float values[] = {0,1,3};
    keyframe->setKeyframe (0,   0, &values[0]);
    keyframe->setKeyframe (1, 100, &values[1]);
    keyframe->setKeyframe (2, 200, &values[2]);

    float value;

    keyframe->sample (-50, &value);
    CHECK_CLOSE (0, value, 0.00001);

    keyframe->sample (0, &value);
    CHECK_CLOSE (0, value, 0.00001);

    keyframe->sample (50, &value);
    CHECK_CLOSE (0, value, 0.00001);

    keyframe->sample (100, &value);
    CHECK_CLOSE (1, value, 0.00001);

    keyframe->sample (150, &value);
    CHECK_CLOSE (1, value, 0.00001);

    keyframe->sample (200, &value);
    CHECK_CLOSE (3, value, 0.00001);

    keyframe->sample (250, &value);
    CHECK_CLOSE (3, value, 0.00001);
}



TEST (KeyframeSequence_interpolate_linear_constant)
{
    KeyframeSequence* keyframe = new KeyframeSequence (3, 1, KeyframeSequence::LINEAR);
    keyframe->setDuration  (1000);
    keyframe->setValidRange (0,2);

    float values[] = {0,1,3};
    keyframe->setKeyframe (0,   0, &values[0]);
    keyframe->setKeyframe (1, 100, &values[1]);
    keyframe->setKeyframe (2, 200, &values[2]);

    float value;

    keyframe->sample (-50, &value);
    CHECK_CLOSE (0, value, 0.00001);

    keyframe->sample (0, &value);
    CHECK_CLOSE (0, value, 0.00001);

    keyframe->sample (50, &value);
    CHECK_CLOSE (0.5, value, 0.00001);

    keyframe->sample (100, &value);
    CHECK_CLOSE (1, value, 0.00001);

    keyframe->sample (150, &value);
    CHECK_CLOSE (2, value, 0.00001);

    keyframe->sample (200, &value);
    CHECK_CLOSE (3, value, 0.00001);

    keyframe->sample (250, &value);
    CHECK_CLOSE (3, value, 0.00001);
}


TEST (KeyframeSequence_interpolate_linear_loop)
{
    KeyframeSequence* keyframe = new KeyframeSequence (3, 1, KeyframeSequence::LINEAR);
    keyframe->setDuration  (1000);
    keyframe->setValidRange (0,2);
    keyframe->setRepeatMode (KeyframeSequence::LOOP);

    float values[] = {0,1,3};
    keyframe->setKeyframe (0,   0, &values[0]);
    keyframe->setKeyframe (1, 100, &values[1]);
    keyframe->setKeyframe (2, 200, &values[2]);

    float value;

    keyframe->sample (-50, &value);
    CHECK_CLOSE (0, value, 0.00001);

    keyframe->sample (0, &value);
    CHECK_CLOSE (0, value, 0.00001);

    keyframe->sample (50, &value);
    CHECK_CLOSE (0.5, value, 0.00001);

    keyframe->sample (100, &value);
    CHECK_CLOSE (1, value, 0.00001);

    keyframe->sample (150, &value);
    CHECK_CLOSE (2, value, 0.00001);

    keyframe->sample (200, &value);
    CHECK_CLOSE (3, value, 0.00001);

    keyframe->sample (250, &value);
    CHECK_CLOSE (3, value, 0.00001);
}


TEST (KeyframeSequence_duplicate)
{
    KeyframeSequence* kseq0 = new KeyframeSequence (10, 3, KeyframeSequence::LINEAR);
    kseq0->setDuration (1000);
    kseq0->setRepeatMode (KeyframeSequence::LOOP);
    kseq0->setValidRange (5,9);

    float values[] = {0,1,3};
    kseq0->setKeyframe (0,   0, &values[0]);
    kseq0->setKeyframe (1, 100, &values[1]);
    kseq0->setKeyframe (2, 200, &values[2]);

    KeyframeSequence* kseq1 = kseq0->duplicate();

    CHECK_EQUAL (kseq0->getKeyframeCount()    , kseq1->getKeyframeCount());
    CHECK_EQUAL (kseq0->getComponentCount()   , kseq1->getComponentCount());
    CHECK_EQUAL (kseq0->getInterpolationType(), kseq1->getInterpolationType());
    CHECK_EQUAL (kseq0->getRepeatMode()       , kseq1->getRepeatMode());
    CHECK_EQUAL (kseq0->getDuration()         , kseq1->getDuration());
    CHECK_EQUAL (kseq0->getValidRangeFirst()  , kseq1->getValidRangeFirst());
    CHECK_EQUAL (kseq0->getValidRangeLast()   , kseq1->getValidRangeLast());

    delete kseq0;
    delete kseq1;
}


