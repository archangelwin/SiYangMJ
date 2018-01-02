#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////

//����������
#ifdef _DEBUG
#define CARD_DISPATCHER_CONTROL
#endif

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//������

//��Ϸ����
#define KIND_ID						389									//��Ϸ I D
#define GAME_NAME					TEXT("�����齫")					//��Ϸ����

//�������
#define GAME_PLAYER					4									//��Ϸ����
#define VERSION_SERVER				PROCESS_VERSION(7,0,1)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(7,0,1)				//����汾

//////////////////////////////////////////////////////////////////////////////////

//��������
#define MAX_ZUI						9									//�������

#define HUANG_ZHUANG_HEAP_CNT		7									// ��ׯʣ�����

#define MAX_WEAVE					4									//������
#define MAX_INDEX					42									//�������
#define MAX_HAND_COUNT					14									//�����Ŀ
#define MAX_REPERTORY				144									//�����
#define FENG_INDEX					27									//��������
#define FLOWER_INDEX				31									//��������
#define MAX_FLOWER_CNT				20									//���Ƹ���
#define MAX_COLOR					5									//���ɫ
#define WEN_QIAN_INDEX				18									//��Ǯ����

#define MAX_HUA_INDEX				11									//��������

#define MAX_WEAVE_CNT				4									//���������

//�˿˶���
#define HEAP_FULL_COUNT				38									//����ȫ��
#define MAX_RIGHT_COUNT				1									//���ȨλDWORD����	

//��ֵ����
#define	MASK_COLOR					0xF0								//��ɫ����
#define	MASK_VALUE					0x0F								//��ֵ����

//////////////////////////////////////////////////////////////////////////////////
//״̬����

#define GAME_SCENE_FREE				GAME_STATUS_FREE					//�ȴ���ʼ
#define GAME_SCENE_CHOOSE			GAME_SCENE_FREE + 1				//�ȴ�ѡ��
#define GAME_SCENE_PLAY				GAME_STATUS_PLAY					//��Ϸ����

#define CARD_COLOR_NULL			0
#define CARD_COLOR_TONG			1
#define CARD_COLOR_WAN			2
#define CARD_COLOR_TIAO			3
#define CARD_COLOR_FENG			4

struct CMD_S_RECORD
{
	int		 nCount;
	BYTE cbHuCount[GAME_PLAYER];			//���ƴ���
	BYTE cbZimoCount[GAME_PLAYER];			//��������
	BYTE cbJiePaoCount[GAME_PLAYER];		//��������
	BYTE cbDiaoPaoCount[GAME_PLAYER];		//��������
	BYTE cbAnGang[GAME_PLAYER];				//���ܴ���
	BYTE cbMingGang[GAME_PLAYER];			//���ܴ���
	SCORE lAllScore[GAME_PLAYER];			//�ܽ����
	SCORE lDetailScore[GAME_PLAYER][32];	//���ֽ����
};

//����״̬
struct CMD_S_StatusFree
{
	//��Ϸ����
	LONG							lCellScore;									//��������

	//ʱ����Ϣ
	BYTE							cbTimeOutCard;								//����ʱ��
	BYTE							cbTimeOperateCard;						//����ʱ��
	BYTE							cbTimeStartGame;							//��ʼʱ��

	//��ʷ����
	SCORE							lTurnScore[GAME_PLAYER];					//������Ϣ
	SCORE							lCollectScore[GAME_PLAYER];					//������Ϣ

	//����					
	BYTE							cbPlayerCount;							//�������
	BYTE							cbMaxZuiCount;							//��������� 
	BYTE							cbZuiScore;								//��ķ���
};

//�������
struct tagWeaveItem
{
	BYTE							cbWeaveKind;								//�������
	BYTE							cbCenterCard;								//�����˿�
	BYTE							cbParam;									//���ͱ�־
	WORD							wProvideUser;								//��Ӧ�û�
	BYTE							cbCardData[MAX_WEAVE_CNT];								//�齫����
};

//��Ϸ״̬
struct CMD_S_StatusChooseZui
{
	BYTE							cbPlayerCount;								//�������
	BYTE							cbMaxZuiCount;								//��������� 
	BYTE							cbZuiScore;									//�����
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//ʱ����Ϣ
	BYTE							cbTimeOutCard;								//����ʱ��
	BYTE							cbTimeOperateCard;							//�з�ʱ��
	BYTE							cbTimeStartGame;							//��ʼʱ��

	//��Ϸ����
	LONGLONG						lCellScore;									//��Ԫ����
	WORD							wBankerUser;								//ׯ���û�
	WORD							wCurrentUser;								//��ǰ�û�
	BYTE							cbMagicIndex;								//��������

	//����					
	BYTE							cbPlayerCount;								//�������
	BYTE							cbMaxZuiCount;								//��������� 
	BYTE							cbZuiScore;									//�����

	//״̬����
	BYTE							cbActionCard;								//�����˿�
	BYTE							cbActionMask;								//��������
	BYTE							cbLeftCardCount;							//ʣ����Ŀ
	bool							bTrustee[GAME_PLAYER];						//�Ƿ��й�
	bool							bTing[GAME_PLAYER];							//�Ƿ�����
	bool							bIsGuoZhuang;
	BYTE							cbZuiInfo[GAME_PLAYER][MAX_ZUI];        // ÿ�����ѡ�����

	//������Ϣ
	WORD							wOutCardUser;								//�����û�
	BYTE							cbOutCardData;								//�����˿�
	BYTE							cbDiscardCount[GAME_PLAYER];				//������Ŀ
	BYTE							cbDiscardCard[GAME_PLAYER][60];				//������¼

	//�˿�����
	BYTE							cbCardCount[GAME_PLAYER];					//�˿���Ŀ
	BYTE							cbCardData[MAX_HAND_COUNT];					//�˿��б�
	BYTE							cbFlowerCount[GAME_PLAYER];
	BYTE							cbFlowerCardData[GAME_PLAYER][MAX_FLOWER_CNT];
	BYTE							cbTatalFlowerCount[GAME_PLAYER];
	BYTE							cbSendCardData;								//�����˿�

	//����˿�
	BYTE							cbWeaveItemCount[GAME_PLAYER];				//�����Ŀ
	tagWeaveItem					WeaveItemArray[GAME_PLAYER][MAX_WEAVE];		//����˿�

	//������Ϣ
	WORD							wHeapHead;									//����ͷ��
	WORD							wHeapTail;									//����β��
	BYTE							cbHeapCardInfo[GAME_PLAYER][2];				//������Ϣ

	BYTE							cbHuCardCount[MAX_HAND_COUNT];
	BYTE							cbHuCardData[MAX_HAND_COUNT][28];
	BYTE							cbOutCardCount;
	BYTE							cbOutCardDataEx[MAX_HAND_COUNT];


	//��ʷ����
	SCORE							lTurnScore[GAME_PLAYER];					//������Ϣ
	SCORE							lCollectScore[GAME_PLAYER];					//������Ϣ
};

//////////////////////////////////////////////////////////////////////////////////
//�����

#define SUB_S_SHOW_FLOWER			98									//������Ϣ
#define SUB_S_GAME_CHOOSE			99									//��Ϸѡ��
#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				101									//�û�����
#define SUB_S_SEND_CARD				102									//�����˿�
#define SUB_S_OPERATE_NOTIFY		103									//������ʾ
#define SUB_S_OPERATE_RESULT		104									//��������
#define SUB_S_LISTEN_CARD			105									//�û�����
#define SUB_S_TRUSTEE				106									//�û��й�
#define SUB_S_REPLACE_CARD			107									//�û�����
#define SUB_S_GAME_CONCLUDE			108									//��Ϸ����
#define SUB_S_SET_BASESCORE			109									//���û���
#define SUB_S_HU_CARD				110									//���ƺ�������
#define SUB_S_RECORD				111									//��Ϸ��¼
#define SUB_S_FLOWER_CNT			112									//��������

//����ѡ������
struct CMD_S_GameChoose
{
	bool							bChooseZui;
};

//�����˿�
struct CMD_S_GameStart
{
	WORD				 			wBankerUser;						//��ǰׯ��
	WORD				 			wReplaceUser;						//�����û�
	WORD							wSiceCount;							//���ӵ���
	WORD							wHeapHead;							//�ƶ�ͷ��
	WORD							wHeapTail;							//�ƶ�β��
	BYTE                            cbMagicIndex;                       //��������
	BYTE							cbHeapCardInfo[GAME_PLAYER][2];		//������Ϣ
	BYTE							cbUserAction;						//�û�����
	BYTE							cbCardData[MAX_HAND_COUNT];				//�齫�б�
	BYTE							cbOutCardCount;
	BYTE							cbOutCardData[MAX_HAND_COUNT][28];
	BYTE							cbZuiInfo[GAME_PLAYER][MAX_ZUI];	// ���ѡ�����
};

//������Ϣ
struct CMD_S_SHOW_FLOWER
{
	WORD							wChairID;								//�������
	BYTE							cbCardCount;							//��������
	BYTE							cbCardData[MAX_HAND_COUNT];					//����
	BYTE							cbGetCardData[MAX_HAND_COUNT];				//����
};


//�������˿�
struct CMD_S_AndroidCard
{
	BYTE							cbHandCard[GAME_PLAYER][MAX_HAND_COUNT];//�����˿�
	WORD							wCurrentUser ;						//��ǰ���
};

//�û�����
struct CMD_S_OutCard
{
	WORD							wOutCardUser;						//�����û�
	BYTE							cbOutCardData;						//�����˿�
	bool							bSysOut;							//�й�ϵͳ����
};

//�����˿�
struct CMD_S_SendCard
{
	BYTE							cbCardData;							//�˿�����
	BYTE							cbActionMask;						//��������
	WORD							wCurrentUser;						//��ǰ�û�
	WORD							wSendCardUser;						//�����û�
	WORD							wReplaceUser;						//�����û�
	bool							bTail;								//ĩβ����
	bool							bIsGuoZhuang;
};

//������ʾ
struct CMD_S_OperateNotify
{
	BYTE							cbActionMask;						//��������
	BYTE							cbActionCard;//[3];					//�����˿�
};

//��������
struct CMD_S_OperateResult
{
	WORD							wOperateUser;						//�����û�
	BYTE							cbActionMask;						//��������
	WORD							wProvideUser;						//��Ӧ�û�
	BYTE							cbOperateCode;						//��������
	BYTE							cbOperateCard[3];					//�����˿�
	bool							bIsGuoZhuang;
};

struct CMD_S_FlowerResult
{
	WORD							wOperateUser;						//�����û�
	BYTE							cbFlowerCnt;						//��������
};

struct CMD_S_Hu_Data
{
	BYTE							cbOutCardCount;								  //���ļ�������
	BYTE							cbOutCardData[MAX_HAND_COUNT];
	BYTE							cbHuCardCount[MAX_HAND_COUNT];				  //�����ܺ��ļ�����
	BYTE							cbHuCardData[MAX_HAND_COUNT][28];
	BYTE							cbHuCardRemainingCount[MAX_HAND_COUNT][28];   //����ʣ����
};

//��������
struct CMD_S_ListenCard
{
	WORD							wListenUser;				//�����û�
	bool							bListen;					//�Ƿ�����
	BYTE							cbHuCardCount;				//��������
	BYTE							cbHuCardData[MAX_INDEX];    //��������
};

//��Ϸ����
struct CMD_S_GameConclude
{
	//���ֱ���
	LONG							lCellScore;										//��Ԫ����
	SCORE							lGameScore[GAME_PLAYER];						//��Ϸ����
	SCORE							lRevenue[GAME_PLAYER];							//˰�ջ���
	SCORE							lGangScore[GAME_PLAYER];						//���ָ���Ӯ��
	SCORE							lZuiScore[GAME_PLAYER][MAX_ZUI];        // ÿ��

																					//������Ϣ
	WORD							wProvideUser;									//��Ӧ�û�
	BYTE							cbProvideCard;									//��Ӧ�˿�
	BYTE							cbSendCardData;									//�����
	DWORD							dwChiHuKind[GAME_PLAYER];						//��������
	DWORD							dwChiHuRight[GAME_PLAYER][MAX_RIGHT_COUNT];		//��������	
	WORD							wLeftUser;										//�������
	WORD							wLianZhuang;									//��ׯ
	
	//��Ϸ��Ϣ
	BYTE							cbCardCount[GAME_PLAYER];						//�˿���Ŀ
	BYTE							cbHandCardData[GAME_PLAYER][MAX_HAND_COUNT];	//�˿��б�

	BYTE							cbWeaveItemCount[GAME_PLAYER];					//�����Ŀ
	tagWeaveItem					WeaveItemArray[GAME_PLAYER][MAX_WEAVE];			//����˿�

	BYTE							cbHandFlowerCard[GAME_PLAYER][MAX_FLOWER_CNT];		//����
	BYTE							cbFlowerCount[GAME_PLAYER];							//�������
	BYTE							cbTotalFlowerCount[GAME_PLAYER];							//�������
	BYTE							cbChooseZuiInfo[GAME_PLAYER][MAX_ZUI];        // ÿ�����ѡ�����
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

//////////////////////////////////////////////////////////////////////////////////
//�����

#define SUB_C_OUT_CARD				1									//��������
#define SUB_C_OPERATE_CARD			2									//�����˿�
#define SUB_C_LISTEN_CARD			3									//�û�����
#define SUB_C_TRUSTEE				4									//�û��й�
#define SUB_C_REPLACE_CARD			5									//�û�����
#define SUB_C_SEND_CARD             6                                   //�����˿� 
#define SUB_C_CHOOSE_ZUI            7                                   //�����˿� 

//�û�ѡ��
struct CMD_C_ChooseZuiInfo
{
	BYTE							cbZui[MAX_ZUI];						// ���ѡ�����
};


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

//�û�����
struct CMD_C_ListenCard
{
	bool							bListenCard;						//�Ƿ�����	
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


//�����˿�
struct CMD_C_SendCard
{
	BYTE							cbControlGameCount;				   //���ƴ���
	BYTE                            cbCardCount;                       //�˿���Ŀ
	WORD                            wBankerUser;                       //����ׯ��
	BYTE                            cbCardData[MAX_REPERTORY];         //�˿�����
};

//���Ʒ������
struct tagTingCardResult
{
	BYTE bAbandonCount;													//�ɶ�������
	BYTE bAbandonCard[MAX_HAND_COUNT];									//�ɶ�����
	BYTE bTingCardCount[MAX_HAND_COUNT];								//������Ŀ
																		// ��һ��Max_count�Ǵ�ʲô�Ƴ�ȥ �ڶ����Ǵ�ʲô�Ƴ�ȥ����Ժ�����
	BYTE bTingCard[MAX_HAND_COUNT][MAX_HAND_COUNT - 1];					//����
	BYTE bRemainCount[MAX_HAND_COUNT];									//�����Ƶ�ʣ����
};

//////////////////////////////////////////////////////////////////////////////////

//���Ƶ÷�
struct tagGangScore
{
	bool		bMingGang;							//�ܸ���
	BYTE		cbGangCount;						//�ܸ���
	LONGLONG	lScore[MAX_WEAVE][GAME_PLAYER];		//ÿ���ܵ÷�
};


#pragma pack()

#endif