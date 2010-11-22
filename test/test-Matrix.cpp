#include <unittest++/UnitTest++.h>
#include <iostream>
#include "m3g/Matrix.hpp"
#include "m3g/Vector.hpp"
using namespace std;
using namespace m3g;

TEST (Matrix_default_value)
{
    float  m1[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
    Matrix mat1;
    CHECK_ARRAY_EQUAL (m1, mat1.m, 16);

    float  m2[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    Matrix mat2 (m2);
    CHECK_ARRAY_EQUAL (m2, mat2.m, 16);

    mat2.set (m1);
    CHECK_ARRAY_EQUAL (m1, mat2.m, 16);
}

TEST (Matrix_set_values)
{
    const Matrix mat1 (1,2,2,2,
                 -5,-6,0,0,
                 4,3,2,1,
                 2,0,0,2);
    Matrix mat2 = mat1.getInverse ();
    float m2[16] = {-0.3157895, 0.0526316,  0.3157895,  0.1578947,
                    0.2631579, -0.2105263, -0.2631579, -0.1315789,
                    0.0789474,  0.2368421,  0.4210526, -0.2894737,
                    0.3157895, -0.0526316, -0.3157895,  0.3421053};
    CHECK_ARRAY_CLOSE (m2, mat2.m, 16, 0.0001f);

    Matrix mat3 = mat1.get3x3 ();
    float m3[16] = {1,2,2,0,
                    -5,-6,0,0,
                    4,3,2,0,
                    0,0,0,1};
    CHECK_ARRAY_CLOSE (m3, mat3.m, 16, 0.0001f);   
}

TEST (Matrix_setIdentity)
{
    float  m[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
    Matrix mat (999,999,999,999, 999,999,999,999, 999,999,999,999, 999,999,999,999);

    mat.setIdentity();
    CHECK_ARRAY_EQUAL (m, mat.m, 16);
}

TEST (Matrix_setRotate)
{
    Matrix mat;
    Vector v;

    mat.setRotate (90, 1,0,0);


    Vector v0 = mat * Vector(0,1,0);

    CHECK_CLOSE (0.f, v0[0], 0.00001);
    CHECK_CLOSE (0.f, v0[1], 0.00001);
    CHECK_CLOSE (1.f, v0[2], 0.00001);

    mat.setRotate (90, 0,1,0);
    Vector v1 = mat * Vector(1,0,0);
  
    CHECK_CLOSE (0.f, v1[0], 0.00001);
    CHECK_CLOSE (0.f, v1[1], 0.00001);
    CHECK_CLOSE (-1.f, v1[2], 0.00001);

    mat.setRotate (90, -1,0,0);
    Vector v2 = mat * Vector (0,0,-1);

    CHECK_CLOSE (0.f, v2[0], 0.00001);
    CHECK_CLOSE (-1.f, v2[1], 0.00001);
    CHECK_CLOSE (0.f, v2[2], 0.00001);

}

TEST (Matrix_setScale)
{
    Matrix mat;
    mat.setScale (2,3,4);

    Vector v = mat * Vector(1,2,3);
    
    CHECK_CLOSE (2.f, v[0], 0.00001);
    CHECK_CLOSE (6.f, v[1], 0.00001);
    CHECK_CLOSE (12.f, v[2], 0.00001);

}

TEST (Matrix_setTranslate)
{
    Matrix mat;
    mat.setTranslate (2,5,8);
    Vector r = mat * Vector(1,2,3);

    CHECK_CLOSE (3, r[0], 0.00001);
    CHECK_CLOSE (7, r[1], 0.00001);
    CHECK_CLOSE (11, r[2], 0.00001);

}

TEST (Matrix_invert_1)
{
    Matrix mat1 (1, 2, 3, 4,  
                 0, 2, 4, 6,  
                 3, 1, 3, 1,  
                 -1, 1, 0, -1);
    Matrix mat2;

    mat2 = mat1.invert();

    //    0.6666667  - 0.5      8.327D-17  - 0.3333333  
    //    1.0833333  - 0.625  - 0.25         0.3333333  
    //  - 1.1666667    0.75     0.5          0.3333333  
    //    0.4166667  - 0.125  - 0.25       - 0.3333333  

    float m[16] = {0.6666667,  -0.5,     0.0,   -0.3333333,
                   1.0833333,  -0.625,  -0.25,   0.3333333,
                   -1.1666667,  0.75,    0.5,    0.3333333,
                   0.4166667, - 0.125, - 0.25,   - 0.3333333};
  
    CHECK_ARRAY_CLOSE (m, mat1.m, 16, 0.00001);
    CHECK_ARRAY_CLOSE (m, mat2.m, 16, 0.00001);
}

TEST (Matrix_invert_2)
{
    // 念のためもう1つ
    Matrix mat1 (3,  2, -1, -2,
                 1,  5,  7,  3,
                 -2, -1, 1,  2,
                 3,   4,  2, 1);
    Matrix mat2 = mat1;

    mat1.invert();

    Matrix mat3 = mat1 * mat2;
  
    float m[16] = {1,0,0,0,
                   0,1,0,0,
                   0,0,1,0,
                   0,0,0,1};
    CHECK_ARRAY_CLOSE (m, mat3.m, 16, 0.00001);
}

TEST (Matrix_transpose)
{
    Matrix mat1 (1, 2, 3, 4,  
                 0, 2, 4, 6,  
                 3, 1, 3, 1,  
                 -1, 1, 0, -1);
    Matrix mat2;

    mat2 = mat1.transpose();

    //    1.    0.    3.  - 1.  
    //    2.    2.    1.    1.  
    //    3.    4.    3.    0.  
    //    4.    6.    1.  - 1.  
 
    float m[16] = {1,0,3,-1, 2,2,1,1, 3,4,3,0, 4,6,1,-1};

    CHECK_ARRAY_CLOSE (m, mat1.m, 16, 0.0001);
    CHECK_ARRAY_CLOSE (m, mat2.m, 16, 0.0001);

}

 
TEST (Matrix_multiply)
{
    Matrix mat0 (1,2,3,4,
                 5,6,7,8,
                 1,2,3,4,
                 5,6,7,8);
    Matrix mat1 (1,2,1,2,
                 -1,0,-1,0,
                 1,1,-1,-1,
                 1,2,3,4);
    Matrix mat2 (1,1,1,1,
                 2,2,2,2,
                 3,3,3,3,
                 4,4,4,4);

    mat2 *= mat0;
    Matrix mat3 = mat0 * mat1;
  
    //    12.    16.    20.    24.  
    //    24.    32.    40.    48.  
    //    36.    48.    60.    72.  
    //    48.    64.    80.    96. 

    float m2[16] = {12,16,20,24, 24,32,40,48, 36,48,60,72, 48,64,80,96};

    CHECK_ARRAY_CLOSE (m2, mat2.m, 16, 0.00001);
  
    //    6.     13.    8.     15.  
    //    14.    33.    16.    35.  
    //    6.     13.    8.     15.  
    //    14.    33.    16.    35.  

    float m3[16] = {6,13,8,15, 14,33,16,35, 6,13,8,15, 14,33,16,35};

    CHECK_ARRAY_CLOSE (m3, mat3.m, 16, 0.00001);

}

TEST (Matrix_multiply_Vector)
{
    Matrix mat1 (1, 2, 3, 4,  
                 0, 2, 4, 6,  
                 3, 1, 3, 1,  
                 -1, 1, 0, -1);
    Vector vec (5,6,7,8);
    Vector vec2;

    vec2 = mat1 * vec;
  
    CHECK_CLOSE (70, vec2.x, 0.0001);
    CHECK_CLOSE (88, vec2.y, 0.0001);
    CHECK_CLOSE (50, vec2.z, 0.0001);
    CHECK_CLOSE (-7, vec2.w, 0.0001);

}

TEST (Matrix_make_parallel_projection_matrix)
{
    float  width  = 640;
    float  height = 480;
    float  aspect_ratio = width/height;
    float  near   = 1;
    float  far    = 100;
    Matrix proj   = make_parallel_projection_matrix (height, aspect_ratio, near, far);

    //cout << "parallel projection = " << proj << "\n";
    CHECK_CLOSE (0.0015625f, proj[0], 0.00001f);
    CHECK_CLOSE (0.f,        proj[1], 0.00001f);
    CHECK_CLOSE (0.f,        proj[2], 0.00001f);
    CHECK_CLOSE (0.f,        proj[3], 0.00001f);

    CHECK_CLOSE (0.f,        proj[4], 0.00001f);
    CHECK_CLOSE (0.00208333f,proj[5], 0.00001f);
    CHECK_CLOSE (0.f,        proj[6], 0.00001f);
    CHECK_CLOSE (0.f,        proj[7], 0.00001f);

    CHECK_CLOSE (0.f,        proj[8], 0.00001f);
    CHECK_CLOSE (0.f,        proj[9], 0.00001f);
    CHECK_CLOSE (-0.020202f, proj[10], 0.00001f);
    CHECK_CLOSE (-1.0202f,   proj[11], 0.00001f);

    CHECK_CLOSE (0.f,        proj[12], 0.00001f);
    CHECK_CLOSE (0.f,        proj[13], 0.00001f);
    CHECK_CLOSE (0.f,        proj[14], 0.00001f);
    CHECK_CLOSE (1.f,        proj[15], 0.00001f);
}

TEST (Matrix_make_perspective_projection_matrix)
{
    float  fovy   = 60;
    float  aspect_ratio = 640/480.f;
    float  near   = 1;
    float  far    = 100;
    Matrix proj   = make_perspective_projection_matrix (fovy, aspect_ratio, near, far);

    CHECK_CLOSE (1.29904f,   proj[0], 0.00001f);
    CHECK_CLOSE (0.f,        proj[1], 0.00001f);
    CHECK_CLOSE (0.f,        proj[2], 0.00001f);
    CHECK_CLOSE (0.f,        proj[3], 0.00001f);

    CHECK_CLOSE (0.f,        proj[4], 0.00001f);
    CHECK_CLOSE (1.73205f,   proj[5], 0.00001f);
    CHECK_CLOSE (0.f,        proj[6], 0.00001f);
    CHECK_CLOSE (0.f,        proj[7], 0.00001f);

    CHECK_CLOSE (0.f,        proj[8], 0.00001f);
    CHECK_CLOSE (0.f,        proj[9], 0.00001f);
    CHECK_CLOSE (-1.0202f,   proj[10], 0.00001f);
    CHECK_CLOSE (-2.0202f,   proj[11], 0.00001f);

    CHECK_CLOSE (0.f,        proj[12], 0.00001f);
    CHECK_CLOSE (0.f,        proj[13], 0.00001f);
    CHECK_CLOSE (-1.f,       proj[14], 0.00001f);
    CHECK_CLOSE (0.f,        proj[15], 0.00001f);


    //cout << "perspective projection = " << proj << "\n";
}
