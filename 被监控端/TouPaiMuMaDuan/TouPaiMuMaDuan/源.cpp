// 使用摄像头进行监控
// 使用OpenCV来实现
// 先配置OpenCV的环境
// 1. 安装Opencv
// 2. 配置opencv的头文件包含目录
// 3. 配合opencv的库文件目录
// 4. 告诉VS会使用哪些库文件
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
sockaddr_in sockAddr;  //服务器端地址

#define SERVER_PORT  2021

VideoCapture cap; //摄像头

void touPai(const char* fileName) {
	
	

	Mat frame; //opencv中表示图片数据
	cap >> frame;

	IplImage outImage = frame; //IplImage也是opencv中用来处理图片数据的
	cvSaveImage(fileName, &outImage, 0); // 保存图片
	
}

int main(void) {
	// 第1个参数，表示允许哪个IP地址可以监控，（谁可以接入）
	// 如果是0， 表示任何客户端，都可以接入
	serverSocket = webInitTCP(0, &sockAddr, SERVER_PORT);
	// 绑定套接字
	bind(serverSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr));
	// 创建监听队列
	listen(serverSocket, 1);

	// 监控者的地址
	SOCKADDR client;
	int nSize = sizeof(client);
	SOCKET clientSock;

	

	while (1) {
		// 等待别人发送监控指令
		printf("等待控制端发起连接...\n");
		clientSock = accept(serverSocket, &client, &nSize);
		printf("控制端已经接入！\n");

		cap.open(0);

		while (1) {
			// 接受“监控指令”
			char buff[4096];

			printf("等待指令...\n");
			int ret = recv(clientSock, buff, sizeof(buff), 0);
			if (ret <= 0) break;

			// 判断收到的是不是“监控”指令
			if (strcmp(buff, "TOU_PAI") == 0) {
				//  正式监控
				touPai("tou_pai.jpg");
				printf("已经拍摄！\n");

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
				printf("已经发送完毕\n");

				/*char pack[] = { 0x01, 0x02, 0x03, 0x04};
				send(clientSock, pack, 4, NULL);
				printf("结束标记已经发送完毕\n");*/
			}
		}

		
		closesocket(clientSock);
		cap.release(); //关闭摄像头
	}

	closesocket(serverSocket);

	
	return 0;
}
