#include <stdio.h>
#include "network_tools.h"
#include <graphics.h>  //easy图形库 qt  



// 今天的视频监控，是使用C 语言开发的！

sockaddr_in sockAddr; //网络地址
SOCKET  serverSocket;  // 网络套接字

// 待解决：
// 升级
// 发布到公网？
// 怎样的达到摄像头的视频
// 热血哥：录音功能
// 内网穿透（有付费软件可以实现，也可以自己实现）


void receivePhoto() {
	int len;
	// 先接受照片的长度
	recv(serverSocket, (char*)&len, 4, NULL);

	char buff[4096];

	FILE* file = fopen("jk.jpg", "wb");

	int count = 0;
	while (1) {
		int ret = recv(serverSocket, buff, sizeof(buff), NULL);
		if (ret < 0) {
			printf("接受失败!\n");
			break;
		} else if (ret == 0) {
			printf("对端关闭\n");
			break;
		}
		else {
			printf("接收到 %d 字节\n", ret);
			// 把接收到的数据，保存到文件里！！！
			fwrite(buff, ret, 1, file);
		}
 
		count = count + ret;
		if (count == len) {
			break;
		}
	}
	fclose(file);
}

void cmdJianKong() {
	// 发送监控指令
	char buff[] = "TOU_PAI";
	int ret =  send(serverSocket, buff,  strlen(buff)+1, 0);
	if (ret <= 0) {
		printf("监控命令发送失败\n");
	}
	else {
		printf("已发送监控命令\n");
		receivePhoto();
	}
 
}

int main(void) {
	// 网络初始化（套路）
	// 问：你比较熟悉的第三方网络框架是什么？
	// libevent  nginx
	serverSocket =  webInitTCP(
		"127.0.0.1", //被监控端的IP地址
		&sockAddr,   //网络地址
		2021);       //端口号， 百度（上网：80, 网盘服务：xx）

	// 连接被监控端（发起网络连接）
	connect(serverSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr));

	//创建监控窗口
	initgraph(640, 480);

	// 发送“命令”
	//自定义函数，用来发送监控命令,并且接受被监控端发来的照片
	while (1) {
		cmdJianKong();

		// 显示监控端发来的照片， 使用easyx
		loadimage(0, _T("jk.jpg"));

		Sleep(10);
	}

	system("pause");
	return 0;
}

/*
   幼儿园监控系统

   家长注册，APP, 半年200

   自动捕捉每个小孩的精彩瞬间 生成 个人专辑
   500 分钱

   1000+
*/