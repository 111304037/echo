#pragma once

#include "EffectLayer.h"

namespace Echo
{
	/**
	 * ���鶯��
	 */
	class EffectLayerSpriteAnimation : public EffectLayer
	{
	public:
		virtual void getPropertyList(PropertyList & list);
		virtual bool getPropertyType(const String & name, PropertyType &type);
		virtual bool getPropertyValue(const String & name, String &value);
		virtual bool setPropertyValue(const String & name,const String & value);

	public:
		EffectLayerSpriteAnimation();
		virtual ~EffectLayerSpriteAnimation();

		virtual int _update(i32 time);
		virtual void updateRenderData();
		
		// �ύ����Ⱦ����
		virtual void submitToRenderQueue(std::vector<EffectRenderable*>& renderables);

		virtual void unprepare();

		virtual void _notifyAffected();

		virtual void importData(DataStream* pStream, int version);
		virtual void exportData(DataStream* pStream, int version);
		virtual void mergeAligenBox(Box& aabb);

		virtual void copyAttributesTo(EffectParticle* particle);


	protected:
		i32			muSegment;	
		i32			mvSegment;
		float		mWidth;
		float		mHeight;
		i32			mSpeedTime;
		i32			mInnerTime;
	};
}