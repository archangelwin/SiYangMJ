#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////////////
// global modify flag
#define GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG						1
//////////////////////////////////////////////////////////////////////////////////
//������

//��Ϸ����
#define KIND_ID						300									//��Ϸ I D
#define GAME_NAME					TEXT("�����齫")					//��Ϸ����

//�������
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#define GAME_PLAYER					2									//��Ϸ����
#else
#define GAME_PLAYER					4									//��Ϸ����
#endif
#define VERSION_SERVER				PROCESS_VERSION(7,0,2)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(7,0,2)				//����汾

//////////////////////////////////////////////////////////////////////////////////
//״̬����

#define GAME_SCENE_FREE				GAME_STATUS_FREE					//�ȴ���ʼ
#define GAME_SCENE_PLAY				GAME_STATUS_PLAY					//��Ϸ����


//��������
#define MAX_WEAVE					4									//������
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#define MAX_INDEX					16									//�������
#else
#define MAX_INDEX					34									//�������
#endif
#define MAX_COUNT					14									//�����Ŀ
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#define MAX_REPERTORY				64									//�����
#else
#define MAX_REPERTORY				136									//�����
#endif
#define MAX_HUA_CARD				0									//���Ƹ���

//�˿˶���
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#define HEAP_FULL_COUNT				16									//����ȫ��
#else
#define HEAP_FULL_COUNT				34									//����ȫ��
#endif

#define MAX_RIGHT_COUNT				1									//���ȨλDWORD����			

//////////////////////////////////////////////////////////////////////////

//�������
struct CMD_WeaveItem
{
	BYTE							cbWeaveKind;						//�������
	BYTE							cbCenterCard;						//�����˿�
	BYTE							cbPublicCard;						//������־
	WORD							wProvideUser;						//��Ӧ�û�
	BYTE							cbCardData[4];						//�������
};

//��������
struct CMD_C_AdminReq
{
	BOOL bBit;															//ռλ����
	BYTE cbReqType;														//��������
#define RQ_ASK_CARD_DATA				1
#define RQ_SET_CARD_DATA				2
	BYTE cbSelectCount;													//ѡ����Ŀ
	BYTE cbHandSelectData[MAX_COUNT];									//ѡ������
	BYTE cbHeapSelectData[MAX_COUNT];									//ѡ������
};

//����ظ�
struct CMD_S_CommandResult
{
	BOOL bBit;															//ռλ����
	BYTE cbAckType;														//�ظ�����
#define ACK_ASK_CARD_DATA				1
#define ACK_SET_CARD_DATA				2
	BYTE cbResult;														//���ؽ��
#define CR_ACCEPT						3								//����
#define CR_REFUSAL						4								//�ܾ�
	BYTE cbRepertoryCardCount;											//�ƶ���Ŀ
	BYTE cbRepertoryCard[MAX_REPERTORY];								//�ƶ�����
};

//��������
struct CMD_C_CommandUpdate
{
	BYTE cbCardCount;													//ѡ����Ŀ
	BYTE cbHandCardData[MAX_COUNT];										//ѡ������
	BYTE cbSelectCardData[MAX_COUNT];									//ѡ������
};

//��������
struct CMD_S_OtherCard
{
	BYTE cbCardCount;													//ѡ����Ŀ
	BYTE cbCardData[MAX_COUNT];											//ѡ������
};

#define IDM_ADMIN_COMMDN				WM_USER+1000
#define IDM_ADMIN_UPDATE				WM_USER+1001

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				101									//��������
#define SUB_S_SEND_CARD				102									//�����˿�
#define SUB_S_OPERATE_NOTIFY		104									//������ʾ
#define SUB_S_OPERATE_RESULT		105									//��������
#define SUB_S_GAME_END				106									//��Ϸ����
#define SUB_S_TRUSTEE				107									//�û��й�
#define SUB_S_REPLACE_CARD			108									//�û�����
#define SUB_S_AMDIN_COMMAND			180									//ϵͳ����
#define SUB_S_OTHER_CARD			181									//��������
#define SUB_S_UPDATE_CARD			182									//��������

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONGLONG						lCellScore;							//�������
	WORD							wBankerUser;						//ׯ���û�
	bool							bTrustee[GAME_PLAYER];				//�Ƿ��й�

	//��ʷ����
	LONGLONG						lTurnScore[GAME_PLAYER];			//������Ϣ
	LONGLONG						lCollectScore[GAME_PLAYER];			//������Ϣ
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��Ϸ����
	LONGLONG						lCellScore;									//��Ԫ����
	WORD							wBankerUser;								//ׯ���û�
	WORD							wCurrentUser;								//��ǰ�û�

	//״̬����
	BYTE							cbActionCard;								//�����˿�
	BYTE							cbActionMask;								//��������
	BYTE							cbLeftCardCount;							//ʣ����Ŀ
	bool							bTrustee[GAME_PLAYER];						//�Ƿ��й�

	//������Ϣ
	WORD							wOutCardUser;								//�����û�
	BYTE							cbOutCardData;								//�����˿�
	BYTE							cbDiscardCount[GAME_PLAYER];				//������Ŀ
	BYTE							cbDiscardCard[GAME_PLAYER][60];				//������¼

	//�˿�����
	BYTE							cbCardCount;								//�˿���Ŀ
	BYTE							cbCardData[MAX_COUNT];						//�˿��б�
	BYTE							cbSendCardData;								//�����˿�

	//����˿�
	BYTE							cbWeaveCount[GAME_PLAYER];					//�����Ŀ
	CMD_WeaveItem					WeaveItemArray[GAME_PLAYER][MAX_WEAVE];		//����˿�

#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
	//������Ϣ
	WORD							wHeapHead;									//����ͷ��
	WORD							wHeapTail;									//����β��
	BYTE							cbHeapCardInfo[GAME_PLAYER][2];				//������Ϣ
#endif

	//��ʷ����
	LONGLONG						lTurnScore[GAME_PLAYER];					//������Ϣ
	LONGLONG						lCollectScore[GAME_PLAYER];					//������Ϣ
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{	
	WORD							wBankerUser;								//ׯ���û�
	WORD							wCurrentUser;								//��ǰ�û�
	BYTE							cbUserAction;								//�û�����
	BYTE							cbLianZhuangCount;							//��ׯ����
	BYTE							cbCardData[MAX_COUNT*GAME_PLAYER];			//�˿��б�
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
	WORD							wHeapHead;									//������ͷ
	WORD							wHeapTail;									//������β
	BYTE							cbHeapCardInfo[GAME_PLAYER][2];				//������Ϣ
#endif
	//����
	BYTE							cbSick[2];							//����
};

//��������
struct CMD_S_OutCard
{
	WORD							wOutCardUser;						//�����û�
	BYTE							cbOutCardData;						//�����˿�
};

//�����˿�
struct CMD_S_SendCard
{
	BYTE							cbCardData;							//�˿�����
	BYTE							cbActionMask;						//��������
	WORD							wCurrentUser;						//��ǰ�û�
	WORD							wSendCardUser;						//�����û�
	bool							bTail;								//ĩβ����
	BYTE							cbLeftCardCount;					//ʣ����Ŀ
};

//������ʾ
struct CMD_S_OperateNotify
{
	WORD							wResumeUser;						//��ԭ�û�
	BYTE							cbActionMask;						//��������
	BYTE							cbActionCard;						//�����˿�
};

//��������
struct CMD_S_OperateResult
{
	WORD							wOperateUser;						//�����û�
	BYTE							cbActionMask;						//��������
	WORD							wProvideUser;						//��Ӧ�û�
	BYTE							cbOperateCode;						//��������
	BYTE							cbOperateCard[3];					//�����˿�
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONGLONG						lGameTax;							//��Ϸ˰��
	//������Ϣ
	WORD							wProvideUser;						//��Ӧ�û�
	BYTE							cbProvideCard;						//��Ӧ�˿�
	DWORD							dwChiHuKind[GAME_PLAYER];			//��������
	DWORD							dwChiHuRight[GAME_PLAYER][MAX_RIGHT_COUNT];		//��������

	//������Ϣ
	LONGLONG						lGameScore[GAME_PLAYER];			//��Ϸ����
	LONGLONG						lGangScore[GAME_PLAYER];			//���Ƶ÷�

	//�˿���Ϣ
	BYTE							cbCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							cbCardData[GAME_PLAYER][MAX_COUNT];	//�˿�����

	WORD							wLeftUser;							//�������
};

//�û��й�
struct CMD_S_Trustee
{
	bool							bTrustee;							//�Ƿ��й�
	WORD							wChairID;							//�й��û�
};

//��������
struct CMD_S_ReplaceCard
{
	WORD							wReplaceUser;						//�����û�
	BYTE							cbReplaceCard;						//�����˿�
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_OUT_CARD				1									//��������
#define SUB_C_OPERATE_CARD			3									//�����˿�
#define SUB_C_TRUSTEE				4									//�û��й�
#define SUB_C_REPLACE_CARD			5									//�û�����
#define SUB_C_AMDIN_COMMAND			8									//ϵͳ����

//��������
struct CMD_C_OutCard
{
	BYTE							cbCardData;							//�˿�����
};

//��������
struct CMD_C_OperateCard
{
	BYTE							cbOperateCode;						//��������
	BYTE							cbOperateCard[3];					//�����˿�
};

//�û��й�
struct CMD_C_Trustee
{
	bool							bTrustee;							//�Ƿ��й�	
};

//��������
struct CMD_C_ReplaceCard
{
	BYTE							cbCardData;							//�˿�����
};

//////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif
