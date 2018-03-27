#ifndef __ECHO_MATERIALCONTROLLER_H__
#define __ECHO_MATERIALCONTROLLER_H__

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include <engine/core/Memory/MemManager.h>
#include <engine/core/Util/Array.hpp>
#include "render/RenderDef.h"
#include "render/ShaderProgram.h"
#include "MaterialInst.h"

namespace Echo
{
	/**
	 * ���ʲ���������                                                       
	 */
	class MaterialController
	{
		friend class MaterialInst;
	public:
		// ���ʿ�������
		enum MATERIAL_CONTROL_TYPE
		{
			MCT_RollLoop = 1,
			MCT_Linear = 2, // �������Ա仯;
			MCT_Bezier = 3, // �������ڱ��������߱仯;
			MCT_Script = 4, // �������ڽű��仯;
			MCT_Time   = 5, // ʱ��

			MCT_None = 10,
		};

		// ��������γ��ö��;
		enum ValueBindType
		{
			BT_ValueX = 1 << 0,
			BT_ValueY = 1 << 1,
			BT_ValueZ = 1 << 2,
			BT_ValueW = 1 << 3,
			BT_ValueAll = BT_ValueX | BT_ValueY | BT_ValueZ | BT_ValueW,
		};

		// ѭ����ʽö��;
		enum ValueLoopType
		{
			LT_None = 0, // 0 ~ 1 
			LT_Loop = 1, // 0 ~ 1, 0 ~ 1, 0 ~ 1
			LT_Revert = 2, // 0 ~ 1 ~ 0 ~ 1 ~ 0
		};

		// ��������ö��;
		enum ValueRevertType
		{
			RT_Forward = 0,
			RT_Backward = 1,
		};

	public:
		explicit MaterialController(MaterialInst* ins);
		virtual ~MaterialController();

		// ���º���
		virtual void Update(ui32 delta);
		virtual void GetPropertyValue(const String& name, String& value);
		virtual bool SetPropertyValue(const String& name,const String& value);
		virtual void GetPropertyList(StringArray& list);
		virtual bool GetPropertyType(const String& name, ShaderParamType& type);

		virtual MaterialController*	Clone(MaterialInst* instance);

		virtual char* getControllerName() const; 

		// ���ÿ��ƵĲ���
		virtual void SetControlUniform(MaterialInst::uniform* uniform){ m_PUniform = uniform;}

		virtual void SetControlUniformName(const String& name){ m_uniformName = name; }

		// ���ÿ������Ĳ���ʵ��;
		void SetMaterialInstance(MaterialInst* instance) { m_MateIns = instance; }

		// ��ȡ���Ʋ���
		MaterialInst::uniform*	GetControlUniform(){return m_PUniform;}

		//��ȡ����������
		MATERIAL_CONTROL_TYPE	GetControlType() { return m_controlType;}	

		// �ļ��򿪱���
		void importData(rapidxml::xml_node<>* parentNode);
		void exportData(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* parentNode);

	protected:
		MATERIAL_CONTROL_TYPE		m_controlType;
		MaterialInst*			m_MateIns;
		MaterialInst::uniform*	m_PUniform;
		String						m_uniformName;

	protected:
		StringArray m_propertyList;

		typedef std::map<String, std::function<void(const String&)> > PropertySetHandlerMap;

		// ��Ծ������͵�SetPropertyImplMap;
		PropertySetHandlerMap m_setPropertyValueImpl;

		typedef std::map<String, std::function<String(void)> > PropertyGetHandlerMap;

		// ��Ծ������͵�GetPropertyImplMap; 
		PropertyGetHandlerMap m_getPropertyValueImpl;

		typedef Echo::array<std::function<void(ui32)>, ShaderParamType::SPT_MAX> ShaderParamHandler;

		// ���Uniform���͵�Handler������;
		ShaderParamHandler m_updateUniformImpls;
	};

	class RollLoopController : public MaterialController
	{
	public:
		explicit RollLoopController(MaterialInst* ins);
		~RollLoopController();

		virtual void Update(ui32 delta);
		virtual void GetPropertyValue(const String & name, String &value);
		virtual bool SetPropertyValue(const String & name,const String & value);
		virtual void GetPropertyList(StringArray& list);
		virtual bool GetPropertyType(const String& name, ShaderParamType& type);

		virtual RollLoopController* Clone(MaterialInst* instance);


		virtual void SetControlUniform(MaterialInst::uniform* uniform);
	protected:
		void initPropertyListMap();

	private:
		void*		m_beginValue;   //��ʼֵ
		void*		m_endValue;		//����ֵ
		void*		m_rollSpeed;	//�����ٶ�
	};

	class TimeController : public MaterialController
	{
		TimeController(const TimeController& rhs);
		TimeController& operator=(const TimeController& rhs);

	public:
		explicit TimeController(MaterialInst* ins);
		~TimeController();

		virtual void Update(ui32 delta) override;;
		virtual TimeController* Clone(MaterialInst* instance) override;
		virtual char* getControllerName() const override;

	private:
		float m_time;
	};

	// MCT_Linear
	class LinearController : public MaterialController
	{
		LinearController(const LinearController& rhs); 
		LinearController& operator=(const LinearController& rhs); 

	public: 
		explicit LinearController(MaterialInst* instance); 
		~LinearController(); 

	public: 
		virtual void Update(ui32 delta) override; 

		virtual bool SetPropertyValue(const String& name, const String& value) override; 
		virtual void GetPropertyValue(const String& name, String& value) override; 

		virtual void GetPropertyList(StringArray& list) override; 
		virtual bool GetPropertyType(const String& name, ShaderParamType& type) override;

		virtual LinearController* Clone(MaterialInst* instance) override;

		virtual char* getControllerName() const override; 

	private: 
		// for constructor && copy constructor
		void initPropertyListMap(); 

		void initUniformUpdaterFunctionArray(); 
		void initSetPropertyFunctionMap(); 
		void initGetPropertyFunctionMap(); 

		void SetPropertyBegin(const String& begin); 
		String GetPropertyBegin() const; 

		void SetPropertyEnd(const String& end); 
		String GetPropertyEnd() const; 
		
		void SetPropertyDelta(const String& delta); 
		String GetPropertyDelta() const;
		
		void SetPropertyBindType(const String& bindType); 
		String GetPropertyBindType() const; 
		
		void SetPropertyLoopType(const String& loopType); 
		String GetPropertyLoopType() const; 

	private: 
		// uniform modifier.
		void UniformModifierInt(ui32 delta); 
		void UniformModifierFloat(ui32 delta);
		void UniformModifierVector2(ui32 delta);
		void UniformModifierVector3(ui32 delta);
		void UniformModifierVector4(ui32 delta);
		void UniformModifierMatrix4(ui32 delta);
		void UniformModifierTexture(ui32 delta);

	private: 
		float m_begin;				 // ��ʼֵ;
		float m_end;				 // ����ֵ;

		float m_delta;				 // �䶯���ȣ�0.0 ~ 1.0��

		ValueBindType m_bindType;		 // ��������γ�ȣ�Ĭ��ΪALL��
		ValueLoopType m_loopType;		 // ������ѭ����ʽ��Ĭ��ΪNONE��
		ValueRevertType m_revertType;	 // ������ǰ������Ĭ��ΪForward��
	};

	// MCT_Bezier
	class BezierController : public MaterialController
	{
		BezierController(const BezierController& rhs); 
		BezierController& operator=(const BezierController& rhs); 

	public: 
		explicit BezierController(MaterialInst* instance); 
		~BezierController(); 

	public:
		virtual void Update(ui32 delta) override;

		virtual bool SetPropertyValue(const String& name, const String& value) override;
		virtual void GetPropertyValue(const String& name, String& value) override;

		virtual void GetPropertyList(StringArray& list) override;
		virtual bool GetPropertyType(const String& name, ShaderParamType& type) override;

		virtual BezierController* Clone(MaterialInst* instance) override;
	
		virtual char* getControllerName() const override; 

	private:
		// for constructor && copy constructor
		void initPropertyListMap();

		void initUniformUpdaterFunctionArray();
		void initSetPropertyFunctionMap();
		void initGetPropertyFunctionMap();

		void setPropertyBeginNode(const String& beginNode); 
		String getPropertyBeginNode() const; 

		void setPropertyControlNode1(const String& controlNode1); 
		String getPropertyControlNode1() const; 

		void setPropertyControlNode2(const String& controlNode2); 
		String getPropertyControlNode2() const; 

		void setPropertyEndNode(const String& endNode); 
		String getPropertyEndNode() const; 

		void setPropertyBindType(const String& bindType); 
		String getPropertyBindType() const; 

		void setPropertyLoopType(const String& loopType); 
		String getPropertyLoopType() const; 

	private:
		// uniform modifier.
		void UniformModifierInt(ui32 delta);
		void UniformModifierFloat(ui32 delta);
		void UniformModifierVector2(ui32 delta);
		void UniformModifierVector3(ui32 delta);
		void UniformModifierVector4(ui32 delta);
		void UniformModifierMatrix4(ui32 delta);
		void UniformModifierTexture(ui32 delta);

	private: 
		Vector3 calcBezier3(ui32 delta); 

	private: 
		// for Bezier3
		Vector3 m_beginNode; 
		Vector3 m_controlNode1;
		Vector3 m_controlNode2; 
		Vector3 m_endNode; 

		ValueBindType m_bindType; 
		ValueLoopType m_loopType; 
		ValueRevertType m_revertType; 

	private: 
		float m_bezierAmount; 
	};

	// MCT_Script
	class ScriptController : public MaterialController
	{
		ScriptController(const ScriptController& rhs);
		ScriptController& operator=(const ScriptController& rhs);

	public:
		explicit ScriptController(MaterialInst* instance);
		~ScriptController();

	public:
		virtual void Update(ui32 delta) override;

		virtual bool SetPropertyValue(const String& name, const String& value) override;
		virtual void GetPropertyValue(const String& name, String& value) override;

		virtual void GetPropertyList(StringArray& list) override;
		virtual bool GetPropertyType(const String& name, ShaderParamType& type) override;

		virtual ScriptController* Clone(MaterialInst* instance) override;

		virtual char* getControllerName() const; 

	private:
		// for constructor && copy constructor
		void initPropertyListMap();

	private:
		// �ű�·��;
		String m_scriptPath; 

	private: 
		// Callbacks; 
	};
}

#endif