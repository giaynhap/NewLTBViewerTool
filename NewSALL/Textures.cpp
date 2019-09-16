#include "Textures.h"
#include "tcx_dtx.h"
int g_iTextureNums = 0;
int meshText = 0;
int GenerateTexturesID(void)
{
	return TEXTUREID_BASE + g_iTextureNums++;
}


int get_file_size(FILE* p_file, bool reset) // path to file
{
	int old = ftell(p_file);
	fseek(p_file, 0, SEEK_END);
	int size = ftell(p_file);
	if (reset == true)fseek(p_file, 0, 0);
	else fseek(p_file, old, 0);
	return size;
}

int LoadDTXImage(const char* szFileName, int Index, int* pWidth, int* pHeight)
{
	FILE* pFile;
	pFile = fopen(szFileName, "rb");

	if (!pFile)
		return false;

	dtx_header_t Header;
	memset(&Header, 0, sizeof(Header));

	fread(&Header, sizeof(Header), 1, pFile);

	if (Header.iResType != 0 || Header.iVersion != -5 || Header.usMipmaps == 0)
	{
		fclose(pFile);
		return -1;
	}
	static unsigned char pBuffer[1024 * 1024 * 4];
	static unsigned char imageData[1024 * 1024 * 4];
	memset(pBuffer, 0, sizeof(pBuffer));
	memset(imageData, 0, sizeof(imageData));
	*pWidth = Header.usWidth;
	*pHeight = Header.usHeight;

	int iBpp = Header.ubExtra[2];
	int iSize;
	int pInternalFormat;
	if (iBpp == 3)
	{
		iSize = Header.usWidth * Header.usHeight * 4;
		pInternalFormat = GL_RGBA;
	}
	else if (iBpp == 4)
	{
		iSize = (Header.usWidth * Header.usHeight) >> 1;
		pInternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
	}
	else if (iBpp == 5)
	{
		iSize = Header.usWidth * Header.usHeight;
		pInternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
	}
	else if (iBpp == 6)
	{
		iSize = Header.usWidth * Header.usHeight;
		pInternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
	}
	else
	{
		iSize = 0;
	}

	int pImageSize = iSize;


	int pcurcolor = 0;

	if (iSize == 0)
	{
		fclose(pFile);
		return -1;
	}

	fread(pBuffer, iSize, 1, pFile);

	if (iBpp == 3)
	{
		for (unsigned int i = 0; i < iSize; i += 4)
		{
			pBuffer[i + 0] ^= pBuffer[i + 2];
			pBuffer[i + 2] ^= pBuffer[i + 0];
			pBuffer[i + 0] ^= pBuffer[i + 2];

		}

	}
	else
	{
		char data[4];
		for (int y = 0; y < Header.usHeight; ++y)
			for (int x = 0; x < Header.usWidth; ++x)
			{
				switch (iBpp) {
				case 4:
					memset(data, 0, 4);
					fetch_2d_texel_rgba_dxt1(Header.usWidth, pBuffer, x, y, &data);
					imageData[pcurcolor++] = data[0];
					imageData[pcurcolor++] = data[1];
					imageData[pcurcolor++] = data[2];
					if (data[3] < 0 || data[3]>255) data[3] = 0;
					imageData[pcurcolor++] = data[3];
					break;
				case 5:
					memset(data, 0, 4);
					fetch_2d_texel_rgba_dxt3(Header.usWidth, pBuffer, x, y, &data);
					imageData[pcurcolor++] = data[0];
					imageData[pcurcolor++] = data[1];
					imageData[pcurcolor++] = data[2];
					if (data[3] < 0 || data[3]>255) data[3] = 0;
					imageData[pcurcolor++] = data[3];
					break;
				case 6:
					memset(data, 0, 4);
					fetch_2d_texel_rgba_dxt5(Header.usWidth, pBuffer, x, y, &data);
					imageData[pcurcolor++] = data[0];
					imageData[pcurcolor++] = data[1];
					imageData[pcurcolor++] = data[2];
					if (data[3] < 0 || data[3]>255) data[3] = 0;
					imageData[pcurcolor++] = data[3];

					break;
				}

			}
		pImageSize = pcurcolor;
		memcpy(pBuffer, imageData, pcurcolor);
	}
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Index);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *pWidth, *pHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pBuffer);
	//free(pBuffer);
	fclose(pFile);

	return 1;
}
int LoadTgaImage(const char* szFileName, int Index, int* SizeW, int* SizeH)
{
	FILE* pFile;
	pFile = fopen(szFileName, "rb");
	if (pFile == NULL)
	{

		return -1;
	}

	int fileLength = get_file_size(pFile);

	byte* buffer = (byte*)malloc(fileLength);
	fread(buffer, fileLength, 1, pFile);


	int	i, columns, rows, row_inc, row, col;
	byte* buf_p, * pixbuf, * targa_rgba;
	byte	palette[256][4], red = 0, green = 0, blue = 0, alpha = 0;
	int	readpixelcount, pixelcount;
	bool	compressed;
	loadtga_s	targa_header;

	buf_p = (byte*)buffer;
	targa_header.id_length = *buf_p++;
	targa_header.colormap_type = *buf_p++;
	targa_header.image_type = *buf_p++;

	targa_header.colormap_index = buf_p[0] + buf_p[1] * 256;		buf_p += 2;
	targa_header.colormap_length = buf_p[0] + buf_p[1] * 256;		buf_p += 2;
	targa_header.colormap_size = *buf_p;				buf_p += 1;
	targa_header.x_origin = *(short*)buf_p;			buf_p += 2;
	targa_header.y_origin = *(short*)buf_p;			buf_p += 2;
	targa_header.width = *(short*)buf_p;		buf_p += 2;
	targa_header.height = *(short*)buf_p;		buf_p += 2;
	targa_header.pixel_size = *buf_p++;
	targa_header.attributes = *buf_p++;
	if (targa_header.id_length != 0) buf_p += targa_header.id_length;	 

	if (targa_header.image_type == 1 || targa_header.image_type == 9)
	{

		if (targa_header.pixel_size != 8)
		{
		
			return false;
		}
		if (targa_header.colormap_length != 256)
		{
	
			return false;
		}
		if (targa_header.colormap_index)
		{
			
			return false;
		}
		if (targa_header.colormap_size == 24)
		{
			for (i = 0; i < targa_header.colormap_length; i++)
			{
				palette[i][2] = *buf_p++;
				palette[i][1] = *buf_p++;
				palette[i][0] = *buf_p++;
				palette[i][3] = 255;
			}
		}
		else if (targa_header.colormap_size == 32)
		{
			for (i = 0; i < targa_header.colormap_length; i++)
			{
				palette[i][2] = *buf_p++;
				palette[i][1] = *buf_p++;
				palette[i][0] = *buf_p++;
				palette[i][3] = *buf_p++;
			}
		}
		else
		{

			return false;
		}
	}
	else if (targa_header.image_type == 2 || targa_header.image_type == 10)
	{
	
		if (targa_header.pixel_size != 32 && targa_header.pixel_size != 24)
		{
		
			return false;
		}
	}
	else if (targa_header.image_type == 3 || targa_header.image_type == 11)
	{
	
		if (targa_header.pixel_size != 8)
		{
			return false;
		}
	}

	columns = targa_header.width;
	rows = targa_header.height;

	int size = columns * rows * 4;
	targa_rgba = (byte*)malloc(size);

	pixbuf = targa_rgba + (rows - 1) * columns * 4;
	row_inc = -columns * 4 * 2;

	compressed = (targa_header.image_type == 9 || targa_header.image_type == 10 || targa_header.image_type == 11);
	for (row = col = 0; row < rows; )
	{
		pixelcount = 0x10000;
		readpixelcount = 0x10000;

		if (compressed)
		{
			pixelcount = *buf_p++;
			if (pixelcount & 0x80)  
				readpixelcount = 1;
			pixelcount = 1 + (pixelcount & 0x7f);
		}

		while (pixelcount-- && (row < rows))
		{
			if (readpixelcount-- > 0)
			{
				switch (targa_header.image_type)
				{
				case 1:
				case 9:
				
					blue = *buf_p++;
					red = palette[blue][0];
					green = palette[blue][1];
					alpha = palette[blue][3];
					blue = palette[blue][2];
					//if( alpha != 255 ) image.flags |= IMAGE_HAS_ALPHA;
					break;
				case 2:
				case 10:
			
					blue = *buf_p++;
					green = *buf_p++;
					red = *buf_p++;
					alpha = 255;
					if (targa_header.pixel_size == 32)
					{
						alpha = *buf_p++;
						//if( alpha != 255 )
							//image.flags |= IMAGE_HAS_ALPHA;
					}
					break;
				case 3:
				case 11:
		
					blue = green = red = *buf_p++;
					alpha = 255;
					break;
				}
			}


			*pixbuf++ = red;
			*pixbuf++ = green;
			*pixbuf++ = blue;
			*pixbuf++ = alpha;
			if (++col == columns)
			{
				row++;
				col = 0;
				pixbuf += row_inc;
			}
		}
	}


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Index);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, targa_header.width, targa_header.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, targa_rgba);
	free(targa_rgba);
	free(buffer);
	fclose(pFile);

	*SizeW = targa_header.width;
	*SizeH = targa_header.height;

	return 1;
}


static void dxt135_decode_imageblock(const GLubyte* img_block_src,
	GLint i, GLint j, GLuint dxt_type, GLvoid* texel) {
	GLchan* rgba = (GLchan*)texel;
	const GLushort color0 = img_block_src[0] | (img_block_src[1] << 8);
	const GLushort color1 = img_block_src[2] | (img_block_src[3] << 8);
	const GLuint bits = img_block_src[4] | (img_block_src[5] << 8) |
		(img_block_src[6] << 16) | (img_block_src[7] << 24);
	GLubyte bit_pos = 2 * (j * 4 + i);
	GLubyte code = (GLubyte)((bits >> bit_pos) & 3);

	rgba[ACOMP] = CHAN_MAX;
	switch (code) {
	case 0:
		rgba[RCOMP] = UBYTE_TO_CHAN(EXP5TO8R(color0));
		rgba[GCOMP] = UBYTE_TO_CHAN(EXP6TO8G(color0));
		rgba[BCOMP] = UBYTE_TO_CHAN(EXP5TO8B(color0));
		break;
	case 1:
		rgba[RCOMP] = UBYTE_TO_CHAN(EXP5TO8R(color1));
		rgba[GCOMP] = UBYTE_TO_CHAN(EXP6TO8G(color1));
		rgba[BCOMP] = UBYTE_TO_CHAN(EXP5TO8B(color1));
		break;
	case 2:
		if (color0 > color1) {
			rgba[RCOMP] = UBYTE_TO_CHAN(((EXP5TO8R(color0) * 2 + EXP5TO8R(color1)) / 3));
			rgba[GCOMP] = UBYTE_TO_CHAN(((EXP6TO8G(color0) * 2 + EXP6TO8G(color1)) / 3));
			rgba[BCOMP] = UBYTE_TO_CHAN(((EXP5TO8B(color0) * 2 + EXP5TO8B(color1)) / 3));
		}
		else {
			rgba[RCOMP] = UBYTE_TO_CHAN(((EXP5TO8R(color0) + EXP5TO8R(color1)) / 2));
			rgba[GCOMP] = UBYTE_TO_CHAN(((EXP6TO8G(color0) + EXP6TO8G(color1)) / 2));
			rgba[BCOMP] = UBYTE_TO_CHAN(((EXP5TO8B(color0) + EXP5TO8B(color1)) / 2));
		}
		break;
	case 3:
		if ((dxt_type > 1) || (color0 > color1)) {
			rgba[RCOMP] = UBYTE_TO_CHAN(((EXP5TO8R(color0) + EXP5TO8R(color1) * 2) / 3));
			rgba[GCOMP] = UBYTE_TO_CHAN(((EXP6TO8G(color0) + EXP6TO8G(color1) * 2) / 3));
			rgba[BCOMP] = UBYTE_TO_CHAN(((EXP5TO8B(color0) + EXP5TO8B(color1) * 2) / 3));
		}
		else {
			rgba[RCOMP] = 0;
			rgba[GCOMP] = 0;
			rgba[BCOMP] = 0;
			if (dxt_type == 1) rgba[ACOMP] = UBYTE_TO_CHAN(0);
		}
		break;
	default:
		break;
	}
}

void fetch_2d_texel_rgb_dxt1(GLint srcRowStride, const GLubyte* pixdata,
	GLint i, GLint j, GLvoid* texel)
{


	const GLubyte* blksrc = (pixdata + ((srcRowStride + 3) / 4 * (j / 4) + (i / 4)) * 8);
	dxt135_decode_imageblock(blksrc, (i & 3), (j & 3), 0, texel);
}

void fetch_2d_texel_rgba_dxt1(GLint srcRowStride, const GLubyte* pixdata,
	GLint i, GLint j, GLvoid* texel)
{

	const GLubyte* blksrc = (pixdata + ((srcRowStride + 3) / 4 * (j / 4) + (i / 4)) * 8);
	dxt135_decode_imageblock(blksrc, (i & 3), (j & 3), 1, texel);
}

void fetch_2d_texel_rgba_dxt3(GLint srcRowStride, const GLubyte* pixdata,
	GLint i, GLint j, GLvoid* texel) {

	GLchan* rgba = (GLchan*)texel;
	const GLubyte* blksrc = (pixdata + ((srcRowStride + 3) / 4 * (j / 4) + (i / 4)) * 16);

	const GLubyte anibble = (blksrc[((j & 3) * 4 + (i & 3)) / 2] >> (4 * (i & 1))) & 0xf;
	dxt135_decode_imageblock(blksrc + 8, (i & 3), (j & 3), 2, texel);
	rgba[ACOMP] = UBYTE_TO_CHAN((GLubyte)(EXP4TO8(anibble)));


}

void fetch_2d_texel_rgba_dxt5(GLint srcRowStride, const GLubyte* pixdata,
	GLint i, GLint j, GLvoid* texel) {
	GLchan* rgba = (GLchan*)texel;
	const GLubyte* blksrc = (pixdata + ((srcRowStride + 3) / 4 * (j / 4) + (i / 4)) * 16);
	const GLubyte alpha0 = blksrc[0];
	const GLubyte alpha1 = blksrc[1];

	const GLubyte bit_pos = ((j & 3) * 4 + (i & 3)) * 3;
	const GLubyte acodelow = blksrc[2 + bit_pos / 8];
	const GLubyte acodehigh = blksrc[3 + bit_pos / 8];
	const GLubyte code = (acodelow >> (bit_pos & 0x7) |
		(acodehigh << (8 - (bit_pos & 0x7)))) & 0x7;

	dxt135_decode_imageblock(blksrc + 8, (i & 3), (j & 3), 2, texel);

	if (code == 0)
		rgba[ACOMP] = UBYTE_TO_CHAN(alpha0);
	else if (code == 1)
		rgba[ACOMP] = UBYTE_TO_CHAN(alpha1);
	else if (alpha0 > alpha1)
		rgba[ACOMP] = UBYTE_TO_CHAN(((alpha0 * (8 - code) + (alpha1 * (code - 1))) / 7));
	else if (code < 6)
		rgba[ACOMP] = UBYTE_TO_CHAN(((alpha0 * (6 - code) + (alpha1 * (code - 1))) / 5));
	else if (code == 6)
		rgba[ACOMP] = 0;
	else
		rgba[ACOMP] = CHAN_MAX;

}