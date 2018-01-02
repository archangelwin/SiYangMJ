#pragma once

//游戏控制基类
class IServerControl
{
public:
	IServerControl(void){};
	virtual ~IServerControl(void){};

public:
	//需要控制
	virtual bool __cdecl NeedControl() = NULL;
	//完成控制
	virtual bool __cdecl CompleteControl() = NULL;	
	//获取数据
	virtual void __cdecl SetCardData(BYTE cbCardIndex[], BYTE cbRepertoryCard[], BYTE cbRepertoryCardCount) = NULL;
	//服务器控制
	virtual bool __cdecl ServerControl(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem, ITableFrame * pITableFrame) = NULL;
};
