#pragma once

#include "Resource.h"
#include "GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////

//Ӧ�ó�����
class CGameClientApp : public CGameFrameApp
{
	//��������
public:
	//���캯��
	CGameClientApp();
	//��������
	virtual ~CGameClientApp();

	//�̳к���
public:
	//��������
	virtual CGameFrameEngine * GetGameFrameEngine(DWORD dwSDKVersion);
};

//////////////////////////////////////////////////////////////////////////

//Ӧ�ó������
extern CGameClientApp theApp;

//////////////////////////////////////////////////////////////////////////
