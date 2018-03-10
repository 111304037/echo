#pragma once

namespace Echo
{
	// ����񶯽ṹ����
	struct CameraShakeInfo
	{
		enum ShakeType
		{
			ST_OLD = 0,			// Ĭ�Ϸ�ʽ
			ST_SQUARE  = 1,		// ����
			ST_RHOMBUS = 2,		// ���β�
			ST_LINEAR  = 3,		// ���Բ�
		};

		float beginTime;		// ��ʼʱ��
		float scale;			// �񶯷���
		float timelast;			// ʣ����ʱ��
		float duration;			// ����ʱ��
		float shakeSpeed;		//
		int shakeTimes;			// �𶯴���
		ShakeType shakeType;	// ����

		CameraShakeInfo()
		{
			beginTime  = 0.f;
			scale      = 0.f;
			timelast   = 0.f;
			duration   = -1.0f;
			shakeSpeed = 0.f;
			shakeTimes = 0;
			shakeType = ST_OLD;
		}
	};

	// �����������Զ
	struct CameraPush
	{	
		float beginTime;		// ��ʼʱ��
		float closeTime;		// ����ʱ��	
		float durationTime;		// ����ʱ��	
		float farawayTime;		// ��Զʱ��		
		float scale;			// ���		
		bool  isInversion;		// �Ƿ�ת	
		bool  isShow;			//�Ƿ���ʾ		
		bool  isAddToAllActors;	//�Ƿ�㲥

		CameraPush()
		{
			beginTime    = 0.0f;
			scale        = 0.0f;
			durationTime = 0.0f;
			closeTime	 = 0.0f;
			farawayTime  = 0.0f;
			isInversion  = false; 
			isShow		 = true;
			isAddToAllActors = false;
		}
	};

	//�����������Զ����
	struct CameraPushEvent
	{
		float beginTime;
		float closeTime;
		float durationTime;
		float farawayTime;
		float scale;
		float stopTime;
		float closeSpeed;
		float farawaySpeed;
		float curentScale;

		CameraPushEvent()
		{
			beginTime    = 0.0f;
			scale        = 0.0f;
			durationTime = 0.0f;
			closeTime	 = 0.0f;
			farawayTime  = 0.0f;
			closeSpeed   = 0.0f;
			farawaySpeed = 0.0f;
			curentScale  = 0.0f;
			stopTime     = 0.0f;
		}
	};

	/**
	 * �����Ч�������� 2014-6-13
	 * �����������,������ԶЧ����
	 */
	class CameraMain;
	class CameraShakeModule
	{
	public:
		CameraShakeModule( CameraMain& camera);

		/** ����(��λ��) */
		void frameMove( float delta); 

		/** ��ʼ */
		void start();

		/** ���� */
		void stop();

		/** ���������Զ */
		void addCameraPush( const CameraPush* pData );

		/** ����� */
		void addCameraShake(float beginTime, float scale, float durationTime, int shakeTimes, CameraShakeInfo::ShakeType type = CameraShakeInfo::ST_OLD);

		/** ��������ж� */
		bool isCameraShake() { return m_cameraShake.duration > 0.f || m_cameraPush.durationTime>0.f; }

	private:
		// ������
		void updateShake( float delta);

		/** ����������ƶ� */
		void updatePush( float delta);

		void stopShake();

		void stopPush();

		//
		void updateDefaultShake(float delta);
		void updateSquareShake(float delta);

	private:
		//��������
		float					m_cameraShakeDelta;
		int						m_cameraShakeStep1;		// ������𶯲���1
		int						m_cameraShakeStep2;		// ������𶯲���2
		CameraShakeInfo			m_cameraShake;			// �������
		float					m_cameraPushDelta;
		CameraPushEvent			m_cameraPush;			// �����������Զ
		CameraMain*				m_camera;				// ��Ӱ�������
		float					m_minShakeExtent;		// �𶯷�Χ
		float					m_maxShakeExtent;
		int						m_cameraShakedTimes; 
	private: 
		vector<Vector2>::type	m_shakeArray;			// �𶯵�Ԫ;	
	};
}