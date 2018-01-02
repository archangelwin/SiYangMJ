#pragma once

//游戏控制基类
class IClientControlDlg : public CDialog 
{
public:
	IClientControlDlg(UINT UID, CWnd* pParent) : CDialog(UID, pParent){}
	virtual ~IClientControlDlg(void){}

	//更新控制
	virtual void __cdecl OnAllowControl(bool bEnable) = NULL;
	//申请结果
	virtual bool __cdecl ReqResult(const void * pBuffer)=NULL;
	//处理消息
	virtual bool __cdecl DealGameMessage(const void * pBuffer)=NULL;
	//预先处理
	virtual void __cdecl PreDealClientData()=NULL;
	//设置数据
	virtual void __cdecl SetLeftCardCount(BYTE cbRepertoryCardCount)=NULL;
	//设置手牌
	virtual void __cdecl SetHandCardData(BYTE cbHandCardData[], BYTE cbCardCount, BYTE cbCurrentCard)=NULL;
	//设置当前
	virtual void __cdecl SetCurrentCard(BYTE cbCurrentCard)=NULL;
};
