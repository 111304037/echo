#ifndef __ECHO_ANIMKEYFRAME_H__
#define __ECHO_ANIMKEYFRAME_H__

namespace Echo
{
	class Animation;


	class AnimKeyFrame
	{
	public:
		AnimKeyFrame(Animation* pAnim, ui32 time);
		virtual ~AnimKeyFrame();

		virtual void createAnimData(Dword boneCount);
		virtual void setAnimData(Dword handle, const Quaternion& ori, const Vector4& pos);

		// ���ö�������
		virtual void setAnimDatammap(ui32 boundCount, const char* dataPtr);

		virtual Animation* getAnim() const;

		virtual const Quaternion& getAnimQuaternion(ui32 idx) const;
		virtual const Vector4& getAnimTranslate(ui32 idx) const;

	protected:
		Animation*					m_pAnim;			// ��Ӧ��������
		vector<Quaternion>::type	m_KeyOrientation;	// ��ת����	
		vector<Vector4>::type		m_KeyPosition;		// λ������

#ifdef LIULIANG_ANIMATION_MMAP
		struct KeyData
		{
			Vector4	 pos;
			Quaternion	orientation;
		};

		ui32						m_boneCount;		// ��������
		const KeyData*				m_data;
#endif
	};
}

#endif