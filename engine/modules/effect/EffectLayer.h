#ifndef __ECHO_EFFECT_LAYER_H__
#define __ECHO_EFFECT_LAYER_H__

#include "Engine/Core.h"
#include "EffectHeader.h"
#include "EffectIElement.h"
#include "EffectKeyFrame.h"
#include "EffectMaterial.h"
#include "EffectParticle.h"
#include "EffectRender.h"
#include "EffectSystem.h"

namespace Echo
{
	class EffectLayer : public IElement , public EffectParticle
	{
	public:
		//interface
		virtual void getPropertyList(PropertyList & list);
		virtual bool getPropertyType(const String & name, PropertyType &type);
		virtual bool getPropertyValue(const String & name, String &value);
		virtual bool setPropertyValue(const String & name,const String & value);

	public:
		//default property
		static const bool		DEFAULT_IS_PUPPET;
		static const bool		DEFAULT_IS_SPACEWARP;
		static const int		DEFAULT_LOD_LEAVE;
		static const int		DEFAULT_RENDER_GROUP;
		static const int		DEFAULT_REPLAY_COUNT;
		static const int		DEFAULT_REPLAY_TIME;
		static const bool		DEFAULT_RESTATE_LOOPEND;
		static const float		DEFAULT_LY_WIDTH;
		static const float		DEFAULT_LY_HEIGHT;
		static const bool		DEFAULT_LY_PERPENDICULAR;
		static const bool		DEFAULT_LY_NOSCALEWIDTH;
		static const bool		DEFAULT_LY_NOSCALEHEIGHT;
		static const float		DEFAULT_LY_CENTEROFFSETW;
		static const float		DEFAULT_LY_CENTEROFFSETH;
		static const float		DEFAULT_LY_OFFSETVIEWPORT;
		static const float      DEFAULT_LY_BOUNDALPHA;

		typedef vector<EffectKeyFrame*>::type EffectKeyFrameList;
		typedef EffectKeyFrameList::iterator EffectKeyFrameIterator;
		typedef EffectKeyFrameList::const_iterator EffectKeyFrameConstIterator;
		typedef map<int, EffectSound*>::type EffectSoundMap;

		EffectLayer(void);
		virtual ~EffectLayer(void);

		// KeyFrame 
		EffectKeyFrame*	createkeyFrame(size_t index = -1);
		void			addKeyFrame(EffectKeyFrame* keyFrame, size_t index = -1);
		void			removeKeyFrame(EffectKeyFrame* keyFrame);
		EffectKeyFrame*	getKeyFrame(size_t index) const;
		inline size_t	getNumKeyFrames()const;
		void			destroyKeyFrame(size_t index);
		void			destroyKeyFrame(EffectKeyFrame* keyFrame);
		void			destroyAllKeyFrames();

		// Material
		EffectMaterial*	createMaterial();
		void			setEffectMaterial(EffectMaterial* material);
		EffectMaterial*	getMaterial() const { return mMaterial; }
		void			destroyMaterial();

		// Layer general
		String			getName() const;
		void			setName(const String& name);
		EFFECT_LAYER_TYPE getLayerType() const { return mType; }
		virtual void	setVisible(bool visible);
		bool			getVisible()const;
		bool			isParticleSystem() { return mbParticleSystem; }

		// ������Ⱦ����
		void setRenderLevel( EFFECT_RENDER_LEVEL level) { m_renderLevel = level; }

		// ��ȡ��Ⱦ����
		EFFECT_RENDER_LEVEL getRenderLevel() const { return (EFFECT_RENDER_LEVEL)m_renderLevel; }

		// initialize an io
		virtual void	prepare_res();
		virtual void	prepare_io();
		virtual void	prepare();
		virtual void	unprepare();

		// update and render
		virtual int		_update(i32 time);
		virtual void	_updateKeyFrame(i32 time);

		// ���µ���Ⱦ����
		virtual void updateRenderData() = 0;

		// ������Ⱦ���ݵ����߳�
		virtual void updateRenderDataToMainThread();

		// �ύ����Ⱦ����
		virtual void submitToRenderQueue(std::vector<EffectRenderable*>& renderables) = 0;

		// foreditor caculate aabb
		virtual void    mergeAligenBox(Box& aabb) = 0;

		virtual void	importData(DataStream* pStream, int version);
		virtual void	exportData(DataStream* pStream, int version);

		// all state operation
		virtual void	_notifyStart(void);
		virtual void	_notifyReStart(void);
		virtual void	_notifyStop();
		virtual void	_notifyPause();
		virtual void	_notifyResume();
		virtual void	_notifyAffected();
		virtual void	_notifyUnAffected();
		virtual void    _notifyExtenalParam(const Vector3& p1, const Vector3& p2);

		// 
		virtual void	_notifyShowVisualHelp(bool show);
		virtual void	_notifyKeyframeEnd() { int i = 0; }
		virtual void	copyAttributesTo (EffectParticle* particle);
		void			setParentSystem(EffectSystem* system) { mParentSystem = system; }
		EffectSystem*	getParentSystem(){ return mParentSystem; }
		void			setRenderQueueGroup(EFFECT_RENDER_GROUP rg);
		EFFECT_RENDER_GROUP		getRenderQueueGroup() const;


		virtual Matrix4	getTransMatrix();

		float			getLayerTimeLength() const;
		void			setFallowLayer(EffectLayer* layer);
		EffectLayer*	getFallowLayer() const;
		void			fallowMovement();

		bool			isPlayEnd(void) const { return mbPlayEnd; };

		void			playEffectSound(int index);	//������Ч���ص���Ч
		void			resetEffectSoundState();	//������Ч��Ч��״̬����Ҫ��Ϊ����������������Чϵͳѭ��
		void			stopEffectSound();			//����Чֹͣʱ��,�����Ч�Ĳ���״̬
		void			destroyEffectSound();		//�����������ڲ��Ż��߳��е���Ч

		std::vector<std::pair<String, String> >	getAllProperties();

	protected:
		i32			m_renderLevel;				// ����,�����Ƿ���Ⱦ
		EFFECT_RENDER_GROUP		mRenderGroup;			
		i32			mReplayCount;		
		i32			mReplayTime;		
		bool		mbRestateLoopEnd;		
		String		mName;				
		EFFECT_LAYER_TYPE mType;		
		EffectSystem* mParentSystem;	
		size_t		mVisualbleParticleNum;	
		bool		mbVisible;					

		i32			mCurrentTime;	
		i32			mReplayIdelTime;	
		bool		mbReplayIdel;				
		size_t		mFrameIndex;				
		bool		mbPlayEnd;					
		bool		mbAffected;			
		i32			mInerReplayCount; 

		EffectLayer* mpFallowLayer;
		EffectKeyFrameList mKeyFrames;
		EffectMaterial* mMaterial;

		bool		mbParticleSystem; 
		bool		mbShowVisualHelp;
		EffectSoundMap	mSoundMap;

		vector<EffectVertexFmt>::type mVertexData;				// ���������Ķ�������(�����߳�)
		vector<EffectVertexFmt>::type m_vertexDataMainThread;	// ���������Ķ�������(���߳�)
	};
}

#endif