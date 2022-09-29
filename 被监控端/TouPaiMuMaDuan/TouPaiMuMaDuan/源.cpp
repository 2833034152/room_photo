// ʹ������ͷ���м��
// ʹ��OpenCV��ʵ��
// ������OpenCV�Ļ���
// 1. ��װOpencv
// 2. ����opencv��ͷ�ļ�����Ŀ¼
// 3. ���opencv�Ŀ��ļ�Ŀ¼
// 4. ����VS��ʹ����Щ���ļ�
/*
	opencv_core249d.lib
	opencv_highgui249d.lib
	opencv_imgproc249d.lib

	opencv_core249d.dll
	opencv_highgui249d.dll
	opencv_imgproc249d.dll
*/
#include "network_tools.h"
#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>

using namespace cv;

SOCKET serverSocket;
sockaddr_in sockAddr;  //�������˵�ַ

#define SERVER_PORT  2021

VideoCapture cap; //����ͷ

void touPai(const char* fileName) {
	
	

	Mat frame; //opencv�б�ʾͼƬ����
	cap >> frame;

	IplImage outImage = frame; //IplImageҲ��opencv����������ͼƬ���ݵ�
	cvSaveImage(fileName, &outImage, 0); // ����ͼƬ
	
}

int main(void) {
	// ��1����������ʾ�����ĸ�IP��ַ���Լ�أ���˭���Խ��룩
	// �����0�� ��ʾ�κοͻ��ˣ������Խ���
	serverSocket = webInitTCP(0, &sockAddr, SERVER_PORT);
	// ���׽���
	bind(serverSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr));
	// ������������
	listen(serverSocket, 1);

	// ����ߵĵ�ַ
	SOCKADDR client;
	int nSize = sizeof(client);
	SOCKET clientSock;

	

	while (1) {
		// �ȴ����˷��ͼ��ָ��
		printf("�ȴ����ƶ˷�������...\n");
		clientSock = accept(serverSocket, &client, &nSize);
		printf("���ƶ��Ѿ����룡\n");

		cap.open(0);

		while (1) {
			// ���ܡ����ָ�
			char buff[4096];

			printf("�ȴ�ָ��...\n");
			int ret = recv(clientSock, buff, sizeof(buff), 0);
			if (ret <= 0) break;

			// �ж��յ����ǲ��ǡ���ء�ָ��
			if (strcmp(buff, "TOU_PAI") == 0) {
				//  ��ʽ���
				touPai("tou_pai.jpg");
				printf("�Ѿ����㣡\n");

				FILE* file = fopen("tou_pai.jpg", "rb");
				fseek(file, 0, SEEK_END);
				int len = ftell(file);
				send(clientSock,(char*)&len, 4, NULL);
				
				fseek(file, 0, SEEK_SET);
				while (1) {
					int ret = fread(buff, 1, 4096, file);
					if (ret <= 0) break;
					send(clientSock, buff, ret, NULL);
				}
				fclose(file);
				printf("�Ѿ��������\n");

				/*char pack[] = { 0x01, 0x02, 0x03, 0x04};
				send(clientSock, pack, 4, NULL);
				printf("��������Ѿ��������\n");*/
			}
		}

		
		closesocket(clientSock);
		cap.release(); //�ر�����ͷ
	}

	closesocket(serverSocket);

	
	return 0;
}
