#pragma once

#include <engine/core/util/AssertX.h>
#include "engine/core/render/interface/mesh/MeshVertexData.h"
#include "Shader.h"

namespace Echo
{
	// ֧�ֵ���ɫ����������
	enum ShaderParamType
	{
		SPT_UNKNOWN,
		SPT_INT,
		SPT_FLOAT,
		SPT_VEC2,
		SPT_VEC3,
		SPT_VEC4,
		SPT_MAT4,
		SPT_TEXTURE,

		SPT_MAX
	};

	/**
	* ShaderProgram 2014-11-18
	*/
	class Renderable;
	class ShaderProgramRes;
	class ShaderProgram
	{
	public:
		// ��������
		struct Uniform
		{
			String			m_name;			// ����
			ShaderParamType	m_type;			// ����
			int				m_count;		// ��С
			int				m_sizeInBytes;	// ��С
			int				m_location;		// λ��
			Byte*			m_origin_value;
			Byte*			m_value;		// ��ǰֵ
			bool			m_isDirty;		// ֵ�Ƿ����˱仯

			// ���캯��
			Uniform()
				: m_name("UnKnown")
				, m_type(SPT_UNKNOWN)
				, m_count(-1)
				, m_location(-1)
				, m_origin_value(NULL)
				, m_value(NULL)
				, m_isDirty(true)
			{}

			// ��������
			~Uniform()
			{
				ECHO_FREE(m_value);
			}

			// ���õ�ǰֵ
			void resetValue()
			{
				m_isDirty = true;
			}

			// ����ֵ
			void setValue(const void* value)
			{
				EchoAssert(value);
				m_origin_value = (Byte*)value;

				// ����ѿռ�
				if (!m_value)
				{
					m_value = (Byte*)ECHO_MALLOC(m_sizeInBytes);
				}

				// ������Ϊtrue,��ճ�ʼ����ֱ�ӿ�������
				if (m_isDirty)
				{
					memcpy(m_value, value, m_sizeInBytes);
				}
				else if ( memcmp(m_value, value, m_sizeInBytes) != 0)
				{
					m_isDirty = true;
					memcpy(m_value, value, m_sizeInBytes);
				}
			}

		};
		typedef std::map<int, Uniform> UniformArray;

	public:
		ShaderProgram(ShaderProgramRes* material);
		virtual ~ShaderProgram();

		// ��ȡ������ɫ��
		Shader* getShader(Shader::ShaderType type) const { return m_pShaders[(ui32)type]; }

		virtual bool attachShader(Shader* pShader);
		virtual Shader* detachShader(Shader::ShaderType type);

		// ������ɫ��
		virtual bool linkShaders() = 0;

		inline bool isLinked() const { return m_bLinked; }

		// ���ݲ�������ȡ���������ַ
		virtual int	getParamPhysicsIndex(const String& paramName);

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual i32 getAtrribLocation(VertexSemantic vertexSemantic);

		// �󶨼���������
		virtual void bindRenderable(Renderable* renderInput) = 0;

	public:
		// ���ݱ�����������ֵ
		void setUniform(const char* name, const void* value, ShaderParamType uniformType, ui32 count);

		// ���������������ñ���ֵ
		virtual void setUniform(ui32 physicIdx, const void* value, ShaderParamType uniformType, ui32 count);

		// Ӧ�ñ���
		virtual void bindUniforms() = 0;

		// ��ȡ��������
		UniformArray* getUniforms(){ return &m_uniforms; }

		// ���ݱ������ͻ�ȡ���ӦByteSize
		static int getUniformByteSizeByUniformType(ShaderParamType uniformType);

		// �������ƻ�ȡUniform
		Uniform* getUniform(const String& name);

		// �������Ϸ���
		bool checkValid();

	protected:
		ShaderProgramRes*	m_material;							// ��������
		Shader*				m_pShaders[Shader::ST_SHADERCOUNT];
		bool				m_bLinked;
		UniformArray		m_uniforms;							// ������Ϣ
	};
}
