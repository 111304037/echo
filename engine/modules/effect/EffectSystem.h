#ifndef __ECHO_EFFECT_SYSTEM_BASE_H__
#define __ECHO_EFFECT_SYSTEM_BASE_H__

#include "Engine/Core.h"
#include "EffectRender.h"
#include "EffectIElement.h"
#include "Engine/core/Resource/EchoThread.h"
#include "engine/core/Scene/Node.h"

namespace Echo
{
	/**
	 * ��Чϵͳ
	 */
	class EffectSystem : public IElement
	{
		friend class EffectPrepareEvent;
		friend class EffectDestroyEvent;
		friend class EffectSystemManager;
	public:
		//interface
		virtual void getPropertyList(PropertyList& list);
		virtual bool getPropertyType(const String& name, PropertyType& type);
		virtual bool getPropertyValue(const String& name, String& value);
		virtual bool setPropertyValue(const String& name,const String& value);

	public:
		//default property
		static const i32	DEFAULT_TIME_STEP;
		static const float	DEFAULT_SCALE_VELOCITY;
		static const float	DEFAULT_SCALE_TIME;

		//effect state
		enum EffectSystemState
		{
			PSS_CREATED,	
			PSS_PREPARING,	
			PSS_PREPARED,
			PSS_SCHEDULE,
			PSS_STARTED,		
			PSS_STOPPED,	
			PSS_PAUSED		
		};

		//delay queue
		enum DelayQueueType
		{
			DQT_START,
			DQT_PAUSE,
			DQT_STOP,
			DQT_UNPREPARE
		};

		struct DelayQueueItem
		{
			DelayQueueType mType;
			i32 mTime;
		};

		// typedef
		typedef vector<EffectLayer*>::type			EffectLayerList;
		typedef EffectLayerList::iterator			EffectLayerIterator;
		typedef EffectLayerList::const_iterator     EffectLayerConstIterator;

		typedef vector<DelayQueueItem>::type		EffectDelayQueue;
		typedef EffectDelayQueue::iterator			EffectDelayIterator;

		typedef vector<ui32>::type                  UbiLayerFallowPair;

		// ��ȡΨһ��ʶ��
		ui32 getIdentifier() const { return m_identifier; }

		// name time.
		inline const String& getName() const { return mName; }
		void setTemplateName(const String& templateName) { mTemplateName = templateName; }
		inline const String& getTemplateName() const { return mTemplateName; }
		inline i32 getTimeElapsedSinceStart(void) const { return mTimeElapsedSinceStart; };

		// ��ȡ��Чʱ��
		float getEffectTimeLength(void) const;

		// layer
		EffectLayer* createLayer(EFFECT_LAYER_TYPE layerType);
		virtual void addLayer(EffectLayer* layer);
		EffectLayer* getLayer(size_t index) const;
		EffectLayer* getLayer(const String& name) const;
		size_t getNumLayers(void) const;
		void destroyLayer(EffectLayer* layer);
		void destroyLayer(size_t index);
		void destroyAllLayers(void);

		// inilised and update
		virtual void prepare_sys(bool bSingleThread = false );
		virtual void prepare_res();
		virtual void prepare_io_sys(void);  // stream threading
		void prepareLayer(void);			// main threading
		void unprepare_sys(void);
		void unprepareLayer(void);

		void importData(DataStream* datastream);
		void exportData(DataStream* datastream);

		// ��Чϵͳ����
		virtual void _update_sys(i32 timeElapsed, bool isSync=false);

		// �ڲ�����
		void updateInternal( i32 timeElapsed);

		virtual void _preUpdate(i32 timeElapsed);

		// �ύ����Ⱦ����
		virtual void submitToRenderQueue();

		// ������Ⱦ����(���ݱ��浽�����߳�)
		virtual void updateRenderData();

		// ������Ⱦ���ݵ����߳�
		void updateRenderDataToMainThread();

		// EffectLayer add renderable to EffectSystem
		void addRenderable(EffectRenderable* renderable);
		const std::vector<EffectRenderable*>* getRenderables() const { return &mRenderables; }

	private:
		// ��Ч�����
		virtual int _updateLayers(i32 timeElapsed);

	public:
		// operate
		void start(void);
		void schedule(i32 time);
		void stop(void);
		void pause(void);
		void resume(void);
		void startAndPause(i32 time);
		virtual void stepIn(i32 time);
		EffectSystemState getState(void) const { return mState; }
		bool isScheduled() const { return PSS_SCHEDULE == mState; }
		bool isPlaying(void) { return (PSS_STARTED == mState); }
		bool isPaused(void) { return PSS_PAUSED == mState; }
		bool isStoped(void) { return PSS_STOPPED == mState; }

		// ���õ�ǰ��Ч��ʹ�õ������
		void	setCurrentCamera(Camera* camera, bool recursive=true);
		Camera* getCurrentCamera();
		inline const Vector3& getCameraPosInEffect() const { return mCameraPosInEffect; }
		inline const Quaternion& getCameraOriInEffect() const { return mCameraOriInEffect; }
		inline Node* getEffectSceneNode() { return mParentNode; }
		void	setParentSceneNode(Node* pNode);
		inline const Box& getBoundingBox() const { return mAABB; };

		// only the timescale control, no position, no oritation, no scale, no color. 
		// pos, ori and scale use the parent bone to control
		// color and alpha use the layer to set.
		inline float getScaleTime(void) const { return mEffectSystemScaleTime; }
		
		// ��������ʱ��
		inline void setScaleTime(float scaleTime){ mEffectSystemScaleTime = scaleTime; }

		// ��ȡϵͳɫ
		inline const Color& getSystemColor(void) const{ return mEffectSystemColor; }

		// ����ϵͳɫ
		inline void setSystemColor(const Color& color){ mEffectSystemColor = color; }

		// this is to reduce the keyframe cal cost. use more.
		const Echo::vector<ui32>::type& getFallowPair(void) const;
		String getFallowPairString(void) const;
		void setFallowPair(const String& pairString);
		void setFallowPair(const Echo::vector<ui32>::type& pairVector);

		/** external params */
		void setExtenalParam(const Vector3& p1, const Vector3& p2);
		void setExtenalParamWithXYZ(Real x1, Real y1, Real z1, Real x2, Real y2, Real z2);

		// editor about.
		EffectLayerList getLayers(void) const;
		bool getShowVisualHelp(void) const;
		void setShowVisualHelp(bool show);
		void changeLayerPosition(size_t p1, size_t p2);
		void caculateAligenBox(bool fromChild = false);
		bool getVisible(void) const;
		void setVisible(bool visible);
		void setIsUIEffect(bool isUIeffect){mbIsUIEffect = isUIeffect;}
		bool getIsUIEffect(){return mbIsUIEffect;}
		EffectSystem& operator=(const EffectSystem& ps);
		bool getIsRealPlaying(){ return mbIsRealPlaying; }
	protected:
		// constructor
		EffectSystem(const String& name,bool isUIeffect = false);
		virtual ~EffectSystem(void);

		bool effectVisible();
		void handleDelayQueue();

	protected:
		ui32			m_identifier;			// Ψһ��ʶ
		String			mName;					// ����
		String			mTemplateName;			// ��Դ����
		EffectSystemState mState;				// ��Ч״̬
		i32				mTimeElapsedSinceStart;
		i32				mDelayTimeTick;		
		bool			mIsPreparing;
		bool			mIsDestroying;
		float			mEffectSystemScaleTime;
		Color			mEffectSystemColor;
		Box				mAABB;					// ��Χ��
		Camera*			mCurrentCamera;			// ��ǰ�����
		Vector3			mCameraPosInEffect;
		Quaternion		mCameraOriInEffect;
		Node*		mParentNode;
		EffectLayerList mLayers;				// ��Ч��

		EffectDelayQueue	mDelayQueue;	
		UbiLayerFallowPair	mFallowPair;
		std::vector<EffectRenderable*> mRenderables;

		bool		mbEffectSystemShowVisualHelp;
		bool		mbIsVisible;
		bool        mbIsUIEffect;
		bool		mbIsCallStartUnormal;		 //�Ƿ��Ƿ���������start����startandpuase��
		bool		mbIsRealPlaying;			 //�Ƿ����������ڲ�����,��Ϊ��������һЩ��StartAndPause֮��Ķ�������
		float		mCurDelta;
		i32			mSchedule;
	};

	/**
	 * �첽�̼߳�����Ч
	 */
	class EffectPrepareEvent : public StreamThread::Task
	{
	public:
		EffectPrepareEvent(EffectSystem* pEffect);
		virtual ~EffectPrepareEvent();

		// ����
		virtual bool process();

		// ���غ�ִ��
		virtual bool finished();

		// ʧ�ܺ��Ƿ��ظ�֪ͨ
		virtual bool isRepeatNotifyIfFail()	{ return true; }

		// �������Ƿ��Զ�����
		virtual bool isAutoDestroy() { return true; }

		// ���ظ��¼������ȼ���
		virtual StreamThread::TaskLevel GetLevel() { return StreamThread::TL_Normal; }

	protected:		
		//EffectSystemID		m_effectSystemID;
		EffectSystem*	mEffectSystem;
	};

	/**
	 * �����¼�
	 */
	class EffectDestroyEvent : public StreamThread::Task
	{
	public:
		EffectDestroyEvent(EffectSystem* effectSystem);
		virtual ~EffectDestroyEvent();

		virtual bool finished() ;

		virtual bool process()	{ return true; /* nothing to do...*/ }

		virtual bool isRepeatNotifyIfFail()	{ return true; }
		virtual bool isAutoDestroy()			{ return true; }

		// ���ظ��¼������ȼ���
		virtual StreamThread::TaskLevel GetLevel() { return StreamThread::TL_Normal; }

	protected:
		EffectSystemID	mpEffect;
		bool			mbRespond;
		EffectSystem*	mEffectSystem;
	};
}
#endif
