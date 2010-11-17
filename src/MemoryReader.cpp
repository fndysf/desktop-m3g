#include "m3g/MemoryReader.hpp"
#include <istream>
#include <cassert>
using namespace std;

/**
 * png_set_read_fn() に渡すカスタムリーダー関数
 */
void my_png_read_func (png_structp png_ptr, png_bytep data, png_size_t length)
{
    MemoryReader* reader = (MemoryReader*)png_get_io_ptr (png_ptr);
    int byte = reader->read ((char*)data, length);
    if (byte < 0) {
        png_error(png_ptr, "png read error");
    }
}


MemoryReader:: MemoryReader (const char* p, int max_size) : buf(0), cur(0), size(0)
{
    buf  = p;
    size = max_size;
    cur  = 0;
}

MemoryReader:: ~MemoryReader ()
{
}

int MemoryReader:: read (char* p, int len)
{
    assert (p != NULL);
    assert (len > 0);

    if (cur + len > size)
        return -1;
    
    memcpy (p, buf+cur, len);
    cur += len;

    return len;
}

