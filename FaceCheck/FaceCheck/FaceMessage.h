#pragma once
#define  IMGDATA_BUFFERSIZE			 10 * 1024
#define  XML_BUFFERSIZE			1024
#define MESSAGE_BUFFER_LEN		16 * 1024

class CFaceMessage
{
public:
	CFaceMessage();
	~CFaceMessage();

	// 构造报文
public:

	/*************************客户端构造报文***************************/
	/**
	 * 客户端测试用
	 */
	void FaceLoadImage();

	/**
	* 构造报文头
	* @param			dev_id			[in]			xml的缓冲区的大小
	* @param			type_msg		[in]			xml的缓冲区的大小
	*/
	void FaceCreateHeader(short dev_id, short type_msg);

	/**
	* 构造客户端XML
	* @param			xml_version			[in]			xml版本
	* @param			id_dev				[in]			客户端设备ID
	* @param			imgsize				[in]			图像数据量， 按base64
	*/
	void FaceCreateClientXML(	const char *msg_version, unsigned imgsize );

	/**
	* 转换图像数据编码为base64
	*/
	void TranslateImgCode(unsigned datasize);

	/**
	 * 设置图像信息
	 */
	void FaceSetImage(unsigned imgwidth, unsigned imgheight);


	/************************服务器构造报文*************************/

	// 解析报文
public:

	/************************服务器解析报文*************************/
	
	/**
	 * 提取head信息
	 */
	void GetMsgHead();

	/**
	* 提取xml信息
	*/
	void ParseClientXml();

	/**
	* 提取IMG
	* 要求提取到原始的二进制图片数据
	*/
	void GetMsgImg();



public:

	// 报文头
	char message_header[32];

	// XML缓冲区
	char str_face_xml[XML_BUFFERSIZE];

	char imgdata_base64[IMGDATA_BUFFERSIZE];		// base64图像数据
	
	unsigned long size_image_base64;	// 图像数据量（按base64）

	char message_all[MESSAGE_BUFFER_LEN]; // 完整数据报

	short id_dev;		// 请求的设备ID
	short size_xml;		// XML尺寸

private:

	// 客户端报文
	const char *client_xml_format = "<? xml version = \"1.0\" ?>\r\n"
									"<XMLFaceMessage>\r\n"
									"<FaceVersion>%s</FaceVersion>\r\n"
									"<ImgWidth>%d</ImgWidth>\r\n"
									"<ImgHeight>%d</ImgHeight>\r\n"
									"<ImgDateSize>%d</ ImgDateSize >\r\n"
									"</XMLFaceMessage>";		// XML 客户端模板

	char imgdata_binary[IMGDATA_BUFFERSIZE];		// 二进制图像数据
	unsigned img_width;
	unsigned img_height;

	// 服务器端报文 
	const char *server_xml_format = "<? xml version = \"1.0\" ?>\
									<XMLFaceMessage>\
									<FaceVersion>%s</FaceVersion>\
									<Userid>%d</Userid>\
									<Username>%s</Username>\
									<Usergender>%s</Usergender>\
									<Loginmessage>%s</Loginmessage>\
									</XMLFaceMessage>";		// XML 客户端模板
};

