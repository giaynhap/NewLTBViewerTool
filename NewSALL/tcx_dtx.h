
#include <windows.h> 
#include <GL/gl.h>

typedef GLubyte GLchan;
#define UBYTE_TO_CHAN(b)  (b)
#define CHAN_MAX 255
#define RCOMP 0
#define GCOMP 1
#define BCOMP 2
#define ACOMP 3
#define 	GL_COMPRESSED_RGB_S3TC_DXT1_EXT   0x83F0
#define 	GL_COMPRESSED_RGBA_S3TC_DXT1_EXT   0x83F1
#define 	GL_COMPRESSED_RGBA_S3TC_DXT3_EXT   0x83F2
#define 	GL_COMPRESSED_RGBA_S3TC_DXT5_EXT   0x83F3
void fetch_2d_texel_rgb_dxt1(GLint srcRowStride, const GLubyte * pixdata,
	GLint i, GLint j, GLvoid * texel);
void fetch_2d_texel_rgba_dxt1(GLint srcRowStride, const GLubyte* pixdata,
	GLint i, GLint j, GLvoid* texel);
void fetch_2d_texel_rgba_dxt3(GLint srcRowStride, const GLubyte* pixdata,
	GLint i, GLint j, GLvoid* texel);
void fetch_2d_texel_rgba_dxt5(GLint srcRowStride, const GLubyte* pixdata,
	GLint i, GLint j, GLvoid* texel);

void tx_compress_dxtn(GLint srccomps, GLint width, GLint height,
	const GLubyte* srcPixData, GLenum destformat,
	GLubyte* dest, GLint dstRowStride);

 
#define EXP5TO8R(packedcol)					\
   ((((packedcol) >> 8) & 0xf8) | (((packedcol) >> 13) & 0x7))

#define EXP6TO8G(packedcol)					\
   ((((packedcol) >> 3) & 0xfc) | (((packedcol) >>  9) & 0x3))

#define EXP5TO8B(packedcol)					\
   ((((packedcol) << 3) & 0xf8) | (((packedcol) >>  2) & 0x7))

#define EXP4TO8(col)						\
   ((col) | ((col) << 4))