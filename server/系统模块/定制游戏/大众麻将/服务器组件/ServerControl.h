#pragma once

//��Ϸ���ƻ���
class IServerControl
{
public:
	IServerControl(void){};
	virtual ~IServerControl(void){};

public:
	//��Ҫ����
	virtual bool __cdecl NeedControl() = NULL;
	//��ɿ���
	virtual bool __cdecl CompleteControl() = NULL;	
	//��ȡ����
	virtual void __cdecl SetCardData(BYTE cbCardIndex[], BYTE cbRepertoryCard[], BYTE cbRepertoryCardCount) = NULL;
	//����������
	virtual bool __cdecl ServerControl(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem, ITableFrame * pITableFrame) = NULL;
};
