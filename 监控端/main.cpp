#include <stdio.h>
#include "network_tools.h"
#include <graphics.h>  //easyͼ�ο� qt  



// �������Ƶ��أ���ʹ��C ���Կ����ģ�

sockaddr_in sockAddr; //�����ַ
SOCKET  serverSocket;  // �����׽���

// �������
// ����
// ������������
// �����Ĵﵽ����ͷ����Ƶ
// ��Ѫ�磺¼������
// ������͸���и����������ʵ�֣�Ҳ�����Լ�ʵ�֣�


void receivePhoto() {
	int len;
	// �Ƚ�����Ƭ�ĳ���
	recv(serverSocket, (char*)&len, 4, NULL);

	char buff[4096];

	FILE* file = fopen("jk.jpg", "wb");

	int count = 0;
	while (1) {
		int ret = recv(serverSocket, buff, sizeof(buff), NULL);
		if (ret < 0) {
			printf("����ʧ��!\n");
			break;
		} else if (ret == 0) {
			printf("�Զ˹ر�\n");
			break;
		}
		else {
			printf("���յ� %d �ֽ�\n", ret);
			// �ѽ��յ������ݣ����浽�ļ������
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
	// ���ͼ��ָ��
	char buff[] = "TOU_PAI";
	int ret =  send(serverSocket, buff,  strlen(buff)+1, 0);
	if (ret <= 0) {
		printf("��������ʧ��\n");
	}
	else {
		printf("�ѷ��ͼ������\n");
		receivePhoto();
	}
 
}

int main(void) {
	// �����ʼ������·��
	// �ʣ���Ƚ���Ϥ�ĵ�������������ʲô��
	// libevent  nginx
	serverSocket =  webInitTCP(
		"127.0.0.1", //����ض˵�IP��ַ
		&sockAddr,   //�����ַ
		2021);       //�˿ںţ� �ٶȣ�������80, ���̷���xx��

	// ���ӱ���ضˣ������������ӣ�
	connect(serverSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr));

	//������ش���
	initgraph(640, 480);

	// ���͡����
	//�Զ��庯�����������ͼ������,���ҽ��ܱ���ض˷�������Ƭ
	while (1) {
		cmdJianKong();

		// ��ʾ��ض˷�������Ƭ�� ʹ��easyx
		loadimage(0, _T("jk.jpg"));

		Sleep(10);
	}

	system("pause");
	return 0;
}

/*
   �׶�԰���ϵͳ

   �ҳ�ע�ᣬAPP, ����200

   �Զ���׽ÿ��С���ľ���˲�� ���� ����ר��
   500 ��Ǯ

   1000+
*/