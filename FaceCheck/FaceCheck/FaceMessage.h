#pragma once
#define  IMGDATA_BUFFERSIZE			 10 * 1024
#define  XML_BUFFERSIZE			1024
#define MESSAGE_BUFFER_LEN		16 * 1024

class CFaceMessage
{
public:
	CFaceMessage();
	~CFaceMessage();

	// ���챨��
public:

	/*************************�ͻ��˹��챨��***************************/
	/**
	 * �ͻ��˲�����
	 */
	void FaceLoadImage();

	/**
	* ���챨��ͷ
	* @param			dev_id			[in]			xml�Ļ������Ĵ�С
	* @param			type_msg		[in]			xml�Ļ������Ĵ�С
	*/
	void FaceCreateHeader(short dev_id, short type_msg);

	/**
	* ����ͻ���XML
	* @param			xml_version			[in]			xml�汾
	* @param			id_dev				[in]			�ͻ����豸ID
	* @param			imgsize				[in]			ͼ���������� ��base64
	*/
	void FaceCreateClientXML(	const char *msg_version, unsigned imgsize );

	/**
	* ת��ͼ�����ݱ���Ϊbase64
	*/
	void TranslateImgCode(unsigned datasize);

	/**
	 * ����ͼ����Ϣ
	 */
	void FaceSetImage(unsigned imgwidth, unsigned imgheight);


	/************************���������챨��*************************/

	// ��������
public:

	/************************��������������*************************/
	
	/**
	 * ��ȡhead��Ϣ
	 */
	void GetMsgHead();

	/**
	* ��ȡxml��Ϣ
	*/
	void ParseClientXml();

	/**
	* ��ȡIMG
	* Ҫ����ȡ��ԭʼ�Ķ�����ͼƬ����
	*/
	void GetMsgImg();



public:

	// ����ͷ
	char message_header[32];

	// XML������
	char str_face_xml[XML_BUFFERSIZE];

	char imgdata_base64[IMGDATA_BUFFERSIZE];		// base64ͼ������
	
	unsigned long size_image_base64;	// ͼ������������base64��

	char message_all[MESSAGE_BUFFER_LEN]; // �������ݱ�

	short id_dev;		// ������豸ID
	short size_xml;		// XML�ߴ�

private:

	// �ͻ��˱���
	const char *client_xml_format = "<? xml version = \"1.0\" ?>\r\n"
									"<XMLFaceMessage>\r\n"
									"<FaceVersion>%s</FaceVersion>\r\n"
									"<ImgWidth>%d</ImgWidth>\r\n"
									"<ImgHeight>%d</ImgHeight>\r\n"
									"<ImgDateSize>%d</ ImgDateSize >\r\n"
									"</XMLFaceMessage>";		// XML �ͻ���ģ��

	char imgdata_binary[IMGDATA_BUFFERSIZE];		// ������ͼ������
	unsigned img_width;
	unsigned img_height;

	// �������˱��� 
	const char *server_xml_format = "<? xml version = \"1.0\" ?>\
									<XMLFaceMessage>\
									<FaceVersion>%s</FaceVersion>\
									<Userid>%d</Userid>\
									<Username>%s</Username>\
									<Usergender>%s</Usergender>\
									<Loginmessage>%s</Loginmessage>\
									</XMLFaceMessage>";		// XML �ͻ���ģ��
};
