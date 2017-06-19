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
* �ͻ��˲�����
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

	// ƴ�ӱ���
	memset(message_all, 0, sizeof message_all);
	memcpy_s(message_all, sizeof message_all, message_header, 32);
	memcpy_s(message_all + 32, sizeof message_all - 32, str_face_xml, strlen(str_face_xml) );
	memcpy_s(message_all +32 + strlen(str_face_xml), sizeof message_all - 32 - strlen(str_face_xml), imgdata_base64, strlen(imgdata_base64));
}

/**
* ���챨��ͷ
* @param			dev_id			[in]			xml�Ļ������Ĵ�С
* @param			type_msg		[in]			xml�Ļ������Ĵ�С
*/
void CFaceMessage::FaceCreateHeader(short dev_id, short type_msg)
{
	// ��ʼ������ͷ
	memset(message_header, 0, sizeof message_header);

	// ���ö���
	message_header[0] = 0x02;
	message_header[1] = 0x07;
	message_header[2] = 0x01;
	message_header[3] = 0x01;

	// ���ð��Ĵ�С
	UINT *messlen = (UINT *)&message_header[4];
	*messlen = 32 + strlen(str_face_xml) + strlen(imgdata_base64);

	// �����豸ID
	short *p_id_dev = (short *)&message_header[10];
	*p_id_dev = dev_id;

	// ���ð������� 101-xml
	short *p_type_msg = (short *)&message_header[12];
	*p_type_msg = type_msg;

	// ����XML��С
	short *p_size_xml = (short *)&message_header[14];
	*p_size_xml = strlen(str_face_xml);
}

/**
* ����ͻ���XML
* @param			xml_version			[in]			xml�汾
* @param			id_dev				[in]			�ͻ����豸ID
* @param			imgsize				[in]			ͼ���������� ��base64
*/
void CFaceMessage::FaceCreateClientXML( const char *msg_version,
										unsigned id_dev,
										unsigned imgsize)
{
	memset(str_face_xml, 0, sizeof str_face_xml);
	sprintf_s(str_face_xml, sizeof str_face_xml - 1, client_xml_format, msg_version, id_dev, img_width, img_height, imgsize);
}

/**
* ת��ͼ�����ݱ���Ϊbase64
*/
void CFaceMessage::TranslateImgCode( unsigned datasize )
{
	// ��ʼ��������
	memset(imgdata_base64, 0, IMGDATA_BUFFERSIZE);

	// ת��
	size_image_base64 = IMGDATA_BUFFERSIZE;
	BOOL isok = CryptBinaryToStringA((BYTE *)imgdata_binary,
		datasize,
		CRYPT_STRING_BASE64,
		imgdata_base64,
		&size_image_base64);

	// �쳣����봦��
	if (size_image_base64 > IMGDATA_BUFFERSIZE - 1)
	{
		// ������̫С
	}
}

/**
* ����ͼ����Ϣ
*/
void CFaceMessage::FaceSetImage(unsigned imgwidth, unsigned imgheight)
{
	img_width = imgwidth;
	img_height = imgheight;
}