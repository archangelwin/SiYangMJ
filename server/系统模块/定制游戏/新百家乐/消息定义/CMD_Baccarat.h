#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

//1�ֽڶ���
#pragma pack(1)

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						122									//��Ϸ I D
#define GAME_NAME					TEXT("�°ټ���")						//��Ϸ����

#define GAME_PLAYER					MAX_CHAIR							//��Ϸ����

//�������
#define VERSION_SERVER				PROCESS_VERSION(7,0,1)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(7,0,1)				//����汾

//״̬����
#define GAME_SCENE_FREE				GAME_STATUS_FREE					//�ȴ���ʼ
#define GAME_SCENE_BET				GAME_STATUS_PLAY					//��ע״̬
#define	GAME_SCENE_END				GAME_STATUS_PLAY+1					//����״̬

//�������
#define AREA_XIAN					0									//�м�����
#define AREA_PING					1									//ƽ������
#define AREA_ZHUANG					2									//ׯ������
#define AREA_XIAN_TIAN				3									//������
#define AREA_ZHUANG_TIAN			4									//ׯ����
#define AREA_TONG_DUI				5									//ͬ��ƽ
#define AREA_XIAN_DUI				6									//�ж���
#define AREA_ZHUANG_DUI				7									//ׯ����
#define AREA_MAX					8									//�������

//������multiple
#define MULTIPLE_XIAN				2									//�мұ���
#define MULTIPLE_PING				9									//ƽ�ұ���
#define MULTIPLE_ZHUANG				2									//ׯ�ұ���
#define MULTIPLE_XIAN_TIAN			3									//����������
#define MULTIPLE_ZHUANG_TIAN		3									//ׯ��������
#define MULTIPLE_TONG_DIAN			33									//ͬ��ƽ����
#define MULTIPLE_XIAN_PING			12									//�ж��ӱ���
#define MULTIPLE_ZHUANG_PING		12									//ׯ���ӱ���


//���ʶ���
#define RATE_TWO_PAIR				12									//��������
#define SERVER_LEN					32									//���䳤��

#define IDM_UPDATE_STORAGE			WM_USER+1001

#ifndef _UNICODE
#define myprintf	_snprintf
#define mystrcpy	strcpy
#define mystrlen	strlen
#define myscanf		_snscanf
#define	myLPSTR		LPCSTR
#else
#define myprintf	swprintf
#define mystrcpy	wcscpy
#define mystrlen	wcslen
#define myscanf		_snwscanf
#define	myLPSTR		LPWSTR
#endif

//��¼��Ϣ
struct tagServerGameRecord
{
	BYTE							cbKingWinner;						//����Ӯ��
	bool							bPlayerTwoPair;						//���ӱ�ʶ
	bool							bBankerTwoPair;						//���ӱ�ʶ
	BYTE							cbPlayerCount;						//�мҵ���
	BYTE							cbBankerCount;						//ׯ�ҵ���
};

struct tagCustomAndroid
{
	//��ׯ
	BOOL							nEnableRobotBanker;				//�Ƿ���ׯ
	LONGLONG						lRobotBankerCountMin;			//��ׯ����
	LONGLONG						lRobotBankerCountMax;			//��ׯ����
	LONGLONG						lRobotListMinCount;				//�б�����
	LONGLONG						lRobotListMaxCount;				//�б�����
	LONGLONG						lRobotApplyBanker;				//����������
	LONGLONG						lRobotWaitBanker;				//��������

	//��ע
	LONGLONG						lRobotMinBetTime;				//��ע�������
	LONGLONG						lRobotMaxBetTime;				//��ע�������
	LONGLONG						lRobotMinJetton;				//��ע������
	LONGLONG						lRobotMaxJetton;				//��ע������
	LONGLONG						lRobotBetMinCount;				//��ע��������
	LONGLONG						lRobotBetMaxCount;				//��ע��������
	LONGLONG						lRobotAreaLimit;				//��������

	//��ȡ��
	LONGLONG						lRobotScoreMin;					//�������
	LONGLONG						lRobotScoreMax;					//�������
	LONGLONG						lRobotBankGetMin;				//ȡ����Сֵ(��ׯ)
	LONGLONG						lRobotBankGetMax;				//ȡ�����ֵ(��ׯ)
	LONGLONG						lRobotBankGetBankerMin;			//ȡ����Сֵ(��ׯ)
	LONGLONG						lRobotBankGetBankerMax;			//ȡ�����ֵ(��ׯ)
	LONGLONG						lRobotBankStoMul;				//���ٷֱ�
	
	//������
	int								nAreaChance[8];		//������
	//���캯��
	tagCustomAndroid()
	{
		DefaultCustomRule();
	}

	void DefaultCustomRule()
	{
		nEnableRobotBanker = TRUE;
		lRobotBankerCountMin = 5;
		lRobotBankerCountMax = 10;
		lRobotListMinCount = 2;
		lRobotListMaxCount = 5;
		lRobotApplyBanker = 5;
		lRobotWaitBanker = 3;

		lRobotMinBetTime = 6;
		lRobotMaxBetTime = 8;
		lRobotMinJetton = 100;
		lRobotMaxJetton = 5000000;
		lRobotBetMinCount = 4;
		lRobotBetMaxCount = 8;
		lRobotAreaLimit = 10000000;
		
		lRobotScoreMin = 1000000;
		lRobotScoreMax = 100000000;
		lRobotBankGetMin = 100;
		lRobotBankGetMax = 30000000;
		lRobotBankGetBankerMin = 10000000;
		lRobotBankGetBankerMax = 50000000;
		lRobotBankStoMul = 50;

		int nTmpAreaChance[8] = {3, 1, 3, 1, 1, 1, 1, 2};
		memcpy(nAreaChance, nTmpAreaChance, sizeof(nAreaChance));

	}
};

//��ע��Ϣ
struct tagUserBet
{
	TCHAR							szNickName[32];						//�û��ǳ�
	DWORD							dwUserGameID;						//�û�ID
	LONGLONG						lUserStartScore;					//�û����
	LONGLONG						lUserWinLost;						//�û����
	LONGLONG						lUserBet[AREA_MAX];				//�û���ע
};

//��ע��Ϣ����
typedef CWHArray<tagUserBet,tagUserBet&> CUserBetArray;

//������
#define RQ_REFRESH_STORAGE		1
#define RQ_SET_STORAGE			2
//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_FREE				99									//��Ϸ����
#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_PLACE_JETTON			101									//�û���ע
#define SUB_S_GAME_END				102									//��Ϸ����
#define SUB_S_APPLY_BANKER			103									//����ׯ��
#define SUB_S_CHANGE_BANKER			104									//�л�ׯ��
#define SUB_S_CHANGE_USER_SCORE		105									//���»���
#define SUB_S_SEND_RECORD			106									//��Ϸ��¼
#define SUB_S_PLACE_JETTON_FAIL		107									//��עʧ��
#define SUB_S_CANCEL_BANKER			108									//ȡ������
#define SUB_S_AMDIN_COMMAND			109									//����Ա����
#define SUB_S_UPDATE_STORAGE        110									//���¿��
#define SUB_S_SEND_USER_BET_INFO    111									//������ע
#define SUB_S_USER_SCORE_NOTIFY		112									//������ע

//����ظ�
struct CMD_S_CommandResult
{
	BYTE cbAckType;					//�ظ�����
		 #define ACK_SET_WIN_AREA  1
		 #define ACK_PRINT_SYN     2
		 #define ACK_RESET_CONTROL 3
	BYTE cbResult;
	#define CR_ACCEPT  2			//����
	#define CR_REFUSAL 3			//�ܾ�
	BYTE cbExtendData[20];			//��������
};


//���¿��
struct CMD_S_UpdateStorage
{
	BYTE                            cbReqType;						//��������
	LONGLONG						lStorageStart;				//��ʼ���
	LONGLONG						lStorageDeduct;				//���˥��
	LONGLONG						lStorageCurrent;				//��ǰ���
	LONGLONG						lStorageMax1;					//�������1
	LONGLONG						lStorageMul1;					//ϵͳ��ָ���1
	LONGLONG						lStorageMax2;					//�������2
	LONGLONG						lStorageMul2;					//ϵͳ��ָ���2
};

//������ע
struct CMD_S_SendUserBetInfo
{
	LONGLONG						lUserStartScore[GAME_PLAYER];				//��ʼ����
	LONGLONG						lUserJettonScore[GAME_PLAYER][AREA_MAX];//������ע
};

//ʧ�ܽṹ
struct CMD_S_PlaceBetFail
{
	WORD							wPlaceUser;							//��ע���
	BYTE							lBetArea;							//��ע����
	LONGLONG						lPlaceScore;						//��ǰ��ע
};

//����ׯ��
struct CMD_S_ApplyBanker
{
	WORD							wApplyUser;							//�������
};

//ȡ������
struct CMD_S_CancelBanker
{
	WORD							wCancelUser;						//ȡ�����
};

//�л�ׯ��
struct CMD_S_ChangeBanker
{
	WORD							wBankerUser;						//��ׯ���
	LONGLONG						lBankerScore;						//ׯ�ҷ���
};

//��Ϸ״̬
struct CMD_S_StatusFree
{
	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	//�����Ϣ
	LONGLONG						lPlayFreeSocre;						//������ɽ��

	//ׯ����Ϣ
	WORD							wBankerUser;						//��ǰׯ��
	LONGLONG						lBankerScore;						//ׯ�ҷ���
	LONGLONG						lBankerWinScore;					//ׯ��Ӯ��
	WORD							wBankerTime;						//ׯ�Ҿ���

	//�Ƿ�ϵͳ��ׯ
	bool							bEnableSysBanker;					//ϵͳ��ׯ

	//������Ϣ
	LONGLONG						lApplyBankerCondition;				//��������
	LONGLONG						lAreaLimitScore;					//��������

	//������Ϣ
	TCHAR							szGameRoomName[SERVER_LEN];			//��������
	bool							bGenreEducate;						//�Ƿ���ϰ��
	tagCustomAndroid				CustomAndroid;						//����������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��
	BYTE							cbGameStatus;						//��Ϸ״̬

	//��ע��
	LONGLONG						lAllBet[AREA_MAX];					//����ע
	LONGLONG						lPlayBet[AREA_MAX];					//�����ע

	//��һ���
	LONGLONG						lPlayBetScore;						//��������ע	
	LONGLONG						lPlayFreeSocre;						//������ɽ��

	//�����Ӯ
	LONGLONG						lPlayScore[AREA_MAX];				//�����Ӯ
	LONGLONG						lPlayAllScore;						//��ҳɼ�
	LONGLONG						lRevenue;							//˰��

	//ׯ����Ϣ
	WORD							wBankerUser;						//��ǰׯ��
	LONGLONG						lBankerScore;						//ׯ�ҷ���
	LONGLONG						lBankerWinScore;					//ׯ��Ӯ��
	WORD							wBankerTime;						//ׯ�Ҿ���

	//�Ƿ�ϵͳ��ׯ
	bool							bEnableSysBanker;					//ϵͳ��ׯ

	//������Ϣ
	LONGLONG						lApplyBankerCondition;				//��������
	LONGLONG						lAreaLimitScore;					//��������

	//�˿���Ϣ
 	BYTE							cbCardCount[2];						//�˿���Ŀ
	BYTE							cbTableCardArray[2][3];				//�����˿�

	//������Ϣ
	TCHAR							szGameRoomName[SERVER_LEN];			//��������
	bool							bGenreEducate;						//�Ƿ���ϰ��
	tagCustomAndroid				CustomAndroid;						//����������
};

//��Ϸ����
struct CMD_S_GameFree
{
	BYTE							cbTimeLeave;						//ʣ��ʱ��
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	WORD							wBankerUser;						//ׯ��λ��
	LONGLONG						lBankerScore;						//ׯ�ҽ��

	LONGLONG						lPlayBetScore;						//��������ע	
	LONGLONG						lPlayFreeSocre;						//������ɽ��

	int								nChipRobotCount;					//�������� (��ע������)
	__int64                         nListUserCount;						//�б�����
	int								nAndriodCount;						//�������б�����
};

//�û���ע
struct CMD_S_PlaceBet
{
	WORD							wChairID;							//�û�λ��
	BYTE							cbBetArea;							//��������
	LONGLONG						lBetScore;							//��ע��Ŀ
	BYTE							cbAndroidUser;						//������ʶ
	BYTE							cbAndroidUserT;						//������ʶ
};

//��Ϸ����
struct CMD_S_GameEnd
{
	//�¾���Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	//�˿���Ϣ
	BYTE							cbCardCount[2];						//�˿���Ŀ
	BYTE							cbTableCardArray[2][3];				//�����˿�
 
	//ׯ����Ϣ
	LONGLONG						lBankerScore;						//ׯ�ҳɼ�
	LONGLONG						lBankerTotallScore;					//ׯ�ҳɼ�
	INT								nBankerTime;						//��ׯ����

	//��ҳɼ�
	LONGLONG						lPlayScore[AREA_MAX];				//��ҳɼ�
	LONGLONG						lPlayAllScore;						//��ҳɼ�

	//ȫ����Ϣ
	LONGLONG						lRevenue;							//��Ϸ˰��
};

struct CMD_S_UserScoreNotify
{
	WORD							wChairID;							//���ID
	//��һ���
	LONGLONG						lPlayBetScore;						//��������ע
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_PLACE_JETTON			1									//�û���ע
#define SUB_C_APPLY_BANKER			2									//����ׯ��
#define SUB_C_CANCEL_BANKER			3									//ȡ������
#define SUB_C_AMDIN_COMMAND			4									//����Ա����
#define SUB_C_UPDATE_STORAGE        5									//���¿��

struct CMD_C_AdminReq
{
	BYTE cbReqType;
		 #define RQ_SET_WIN_AREA	1
		 #define RQ_RESET_CONTROL	2
		 #define RQ_PRINT_SYN		3
	BYTE cbExtendData[20];			//��������
};

//�û���ע
struct CMD_C_PlaceBet
{
	
	BYTE							cbBetArea;						//��������
	LONGLONG						lBetScore;						//��ע��Ŀ
};

//���¿��
struct CMD_C_UpdateStorage
{
	BYTE                            cbReqType;						//��������
	LONGLONG						lStorageDeduct;					//���˥��
	LONGLONG						lStorageCurrent;				//��ǰ���
	LONGLONG						lStorageMax1;					//�������1
	LONGLONG						lStorageMul1;					//ϵͳ��ָ���1
	LONGLONG						lStorageMax2;					//�������2
	LONGLONG						lStorageMul2;					//ϵͳ��ָ���2
};

//��ԭ������
#pragma pack()
//////////////////////////////////////////////////////////////////////////

#endif
