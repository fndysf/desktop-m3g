

#include "m3g/Config.hpp"


#ifndef __M3G_GL_HPP__
#define __M3G_GL_HPP__

// メモ：
// OpenGLのOS毎の違いを吸収するヘッダーファイル
// とにかく微妙な差が多すぎる。

// 
// glDrawElements()に渡せるインデックスのタイプは
// OpenGLはGL_UNSIGNED_INTまで、OpenGL ES GL_UNSIGNED_SHORT

// indexno
// OpenGL - INT_MAX
// OpenGL ES - USHORT_MAX

 
// =====  Ubuntu(Linux系） =====
// - nVIDIA系は GL_GLEXT_PROTOTYPES を定義して
//   gl.h と glext.h をインクルードする
// - ATI系は gl.h のみをインクルードすれば十分。
//   定義しても動作に問題はない
// - 2011.02.23 gluは必要無くなったのでコメントアウト.
#ifdef USE_GL
#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif
#include <GL/gl.h>
#include <GL/glext.h>

#include <climits>
typedef int IndexType;
const int INDEX_MAX  = INT_MAX;
const int INDEX_TYPE = GL_UNSIGNED_INT;

// quick hack
inline static
void glOrthof ( GLdouble left, GLdouble right,
                GLdouble bottom, GLdouble top,
                GLdouble near_val, GLdouble far_val )
{
    glOrtho (left, right, bottom, top, near_val, far_val);
}
#endif // USE_GL

// =====  Android =====
// - Android用OpenGL ES.
#ifdef USE_GL_ES
#include <GLES/gl.h>
#include <GLES/glext.h>

#include <climits>
typedef unsigned short IndexType ;
const int INDEX_MAX  = USHRT_MAX;
const int INDEX_TYPE = GL_UNSIGNED_SHORT;

// quick hack
inline static
void glClearDepth ( GLclampf depth)
{
    glClearDepthf (depth);
}
#endif

#if defined(USE_GL) && defined(USE_GL_ES)
#error "Can't define both USE_GL and USE_GL_ES"
#endif


#endif // USE_GL_ES
