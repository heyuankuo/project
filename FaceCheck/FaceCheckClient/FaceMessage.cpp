#include "FaceMessage.h"

#include <windows.h>
#include <iostream>
#include <fstream>

#pragma comment(lib, "Crypt32.lib")

using std::ifstream;
using std::ios_base;

CFaceMessage::CFaceMessage()
{
}


CFaceMessage::~CFaceMessage()
{
}

/**
* 客户端测试用
*/
void CFaceMessage::FaceLoadImage()
{
	ifstream ifs("E:\\110.jpg", ios_base::binary | ios_base::in);

	// get length of file:
	ifs.seekg(0, ios_base::end);
	unsigned length = ifs.tellg();
	ifs.seekg(0, ios_base::beg);

	memset(imgdata_binary, 0, sizeof imgdata_binary);
	ifs.read(imgdata_binary, length);
	ifs.close();
	ifs.clear();

	TranslateImgCode( length );
	FaceSetImage(100, 100);
	FaceCreateClientXML( "1.0", 1, strlen(imgdata_base64));
	FaceCreateHeader( 1, 101);

	// 拼接报文
	memset(message_all, 0, sizeof message_all);
	memcpy_s(message_all, sizeof message_all, message_header, 32);
	memcpy_s(message_all + 32, sizeof message_all - 32, str_face_xml, strlen(str_face_xml) );
	memcpy_s(message_all +32 + strlen(str_face_xml), sizeof message_all - 32 - strlen(str_face_xml), imgdata_base64, strlen(imgdata_base64));
}

/**
* 构造报文头
* @param			dev_id			[in]			xml的缓冲区的大小
* @param			type_msg		[in]			xml的缓冲区的大小
*/
void CFaceMessage::FaceCreateHeader(short dev_id, short type_msg)
{
	// 初始化报文头
	memset(message_header, 0, sizeof message_header);

	// 设置对齐
	message_header[0] = 0x02;
	message_header[1] = 0x07;
	message_header[2] = 0x01;
	message_header[3] = 0x01;

	// 设置包文大小
	UINT *messlen = (UINT *)&message_header[4];
	*messlen = 32 + strlen(str_face_xml) + strlen(imgdata_base64);

	// 设置设备ID
	short *p_id_dev = (short *)&message_header[10];
	*p_id_dev = dev_id;

	// 设置包文类型 101-xml
	short *p_type_msg = (short *)&message_header[12];
	*p_type_msg = type_msg;

	// 设置XML大小
	short *p_size_xml = (short *)&message_header[14];
	*p_size_xml = strlen(str_face_xml);
}

/**
* 构造客户端XML
* @param			xml_version			[in]			xml版本
* @param			id_dev				[in]			客户端设备ID
* @param			imgsize				[in]			图像数据量， 按base64
*/
void CFaceMessage::FaceCreateClientXML( const char *msg_version,
										unsigned id_dev,
										unsigned imgsize)
{
	memset(str_face_xml, 0, sizeof str_face_xml);
	sprintf_s(str_face_xml, sizeof str_face_xml - 1, client_xml_format, msg_version, id_dev, img_width, img_height, imgsize);
}

/**
* 转换图像数据编码为base64
*/
void CFaceMessage::TranslateImgCode( unsigned datasize )
{
	// 初始化缓冲区
	memset(imgdata_base64, 0, IMGDATA_BUFFERSIZE);

	// 转换
	size_image_base64 = IMGDATA_BUFFERSIZE;
	BOOL isok = CryptBinaryToStringA((BYTE *)imgdata_binary,
		datasize,
		CRYPT_STRING_BASE64,
		imgdata_base64,
		&size_image_base64);

	// 异常检测与处理
	if (size_image_base64 > IMGDATA_BUFFERSIZE - 1)
	{
		// 缓冲区太小
	}
}

/**
* 设置图像信息
*/
void CFaceMessage::FaceSetImage(unsigned imgwidth, unsigned imgheight)
{
	img_width = imgwidth;
	img_height = imgheight;
}