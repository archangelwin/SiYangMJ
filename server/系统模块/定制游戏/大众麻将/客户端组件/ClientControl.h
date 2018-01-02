#pragma once

//��Ϸ���ƻ���
class IClientControlDlg : public CDialog 
{
public:
	IClientControlDlg(UINT UID, CWnd* pParent) : CDialog(UID, pParent){}
	virtual ~IClientControlDlg(void){}

	//���¿���
	virtual void __cdecl OnAllowControl(bool bEnable) = NULL;
	//������
	virtual bool __cdecl ReqResult(const void * pBuffer)=NULL;
	//������Ϣ
	virtual bool __cdecl DealGameMessage(const void * pBuffer)=NULL;
	//Ԥ�ȴ���
	virtual void __cdecl PreDealClientData()=NULL;
	//��������
	virtual void __cdecl SetLeftCardCount(BYTE cbRepertoryCardCount)=NULL;
	//��������
	virtual void __cdecl SetHandCardData(BYTE cbHandCardData[], BYTE cbCardCount, BYTE cbCurrentCard)=NULL;
	//���õ�ǰ
	virtual void __cdecl SetCurrentCard(BYTE cbCurrentCard)=NULL;
};
