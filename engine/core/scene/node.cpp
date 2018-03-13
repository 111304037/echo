#include "Node.h"
#include "engine/core/Util/LogManager.h"

namespace Echo
{
	Node::Node()
		: m_pParent(NULL)
		, m_bVisible(true)
		, m_posLocal(Vector3::ZERO)
		, m_ortLocal(Quaternion::IDENTITY)
		, m_sclLocal(Vector3::ONE)
		, m_bModify(false)
		, m_bMatrixDirty(false)
	{
		m_sclWorld = m_sclLocal;
		m_ortWorld = m_ortLocal;
		m_posWorld = m_posLocal;

		m_matWorld = Matrix4::IDENTITY;
		
		needUpdate();
        
        m_children.clear();

		// ��1��ʼ����
		static int identifier=1;
		m_identifier = identifier++;
	}

	// ��������
	Node::~Node()
	{
		for (Node* n : m_children)
		{
			n->m_pParent = nullptr;
		}
	}

	// ���ø��ڵ�
	void Node::setParent(Node* pParent)
	{
		if(m_pParent)
		{
			ChildNodeSet::iterator it = m_pParent->m_children.find(this);
			if(it != m_pParent->m_children.end())
			{
				m_pParent->m_children.erase(it);
			}
		}

		m_pParent = pParent;

		if(m_pParent)
			m_pParent->m_children.insert(this);

		needUpdate();
	}

	void Node::setVisible(bool bVisible)
	{
		m_bVisible = bVisible;
	}

	void Node::scale(const Vector3& scl)
	{
		m_sclLocal = m_sclLocal * scl;
		
		needUpdate();
	}

	void Node::roll(const Real randian)
	{
		rotate(Vector3::UNIT_Z, randian);
	}

	void Node::pitch(const Real randian)
	{
		rotate(Vector3::UNIT_X, randian);
	}

	void Node::yaw(const Real randian)
	{
		rotate(Vector3::UNIT_Y, randian);
	}

	void Node::rotate(const Vector3& vAxis, const Real randian)
	{
		Quaternion rot(vAxis, randian);
		rotate(rot);
	}

	void Node::rotate(const Quaternion& rot)
	{
		// Normalise quaternion to avoid drift
		Quaternion qnorm = rot;
		qnorm.normalize();

		// Note the order of the mult, i.e. q comes after
		m_ortLocal = m_ortLocal * qnorm;

		needUpdate();
	}

	void Node::translate(const Vector3& d)
	{
		m_posLocal += m_ortLocal * d;

		needUpdate();
	}

	void Node::setLocalScaling(const Vector3& scl)
	{
		m_sclLocal = scl;
		needUpdate();
	}

	void Node::setLocalScalingXYZ(Real x, Real y, Real z)
	{
		Vector3 scl(x, y, z);
		setLocalScaling(scl);
	}

	void Node::setLocalOrientation(const Quaternion& ort)
	{
		m_ortLocal = ort;
		m_ortLocal.normalize();

		needUpdate();
	}

	void Node::setLocalPosition(const Vector3& pos)
	{
		m_posLocal = pos;
		needUpdate();
	}

	void Node::setLocalPositionXYZ(Real posX, Real posY, Real posZ)
	{
		Vector3 pos(posX, posY, posZ);
		setLocalPosition(pos);
	}

	void Node::setWorldOrientation(const Quaternion& ort)
	{
		if(m_pParent)
		{
			Quaternion ortLocal;
			m_pParent->convertWorldToLocalOrientation(ortLocal, ort);
			setLocalOrientation(ortLocal);
		}
		else
		{
			setLocalOrientation(ort);
		}
	}

	void Node::setWorldPosition(const Vector3& pos)
	{
		if(m_pParent)
		{
			Vector3 posLocal;
			m_pParent->convertWorldToLocalPosition(posLocal, pos);
			setLocalPosition(posLocal);
		}
		else
		{
			setLocalPosition(pos);
		}
	}

	Node* Node::getParent() const
	{
		return m_pParent;
	}

	Node* Node::getChild(ui32 idx)
	{
		return *std::next(m_children.begin(), idx);
	}

	bool Node::isVisible() const
	{
		return m_bVisible;
	}

	const Vector3& Node::getLocalScaling() const
	{
		return m_sclLocal;
	}

	const Quaternion& Node::getLocalOrientation() const
	{
		return m_ortLocal;
	}

	const Vector3& Node::getLocalPosition() const
	{
		return m_posLocal;
	}

	const Vector3& Node::getWorldScaling() const
	{
		return m_sclWorld;
	}

	const Quaternion& Node::getWorldOrientation() const
	{
		return m_ortWorld;
	}

	void Node::getWorldOrientationWXYZ(Real* w /* = 0 */, Real* x /* = 0 */, Real* y /* = 0 */, Real* z /* = 0 */)
	{
		*w = m_ortWorld.w;
		*x = m_ortWorld.x;
		*y = m_ortWorld.y;
		*z = m_ortWorld.z;
	}

	const Vector3& Node::getWorldPosition() const
	{
		return m_posWorld;
	}
	
	void Node::getWorldPositionXYZ(Real* x /* = 0 */, Real* y /* = 0 */, Real* z /* = 0 */)
	{
		*x = m_posWorld.x;
		*y = m_posWorld.y;
		*z = m_posWorld.z;
	}

	void Node::setWorldMatrix(const Matrix4& tempMatrix)
	{
		m_matWorld = tempMatrix;
	}
	
	const Matrix4& Node::getWorldMatrix()
	{
		if (m_bMatrixDirty)
		{
			if(m_bModify)
				update();

			m_matWorld.makeScaling(getWorldScaling());

			Matrix4 matRot;
			matRot.fromQuan(getWorldOrientation());
			m_matWorld = m_matWorld * matRot;

			m_matWorld.translate(getWorldPosition());
			m_bMatrixDirty = false;
		}

		return m_matWorld;
	}

	Matrix4* Node::getWorldMatrixPtr()
	{
		return &m_matWorld;
	}

	Matrix4 Node::getInverseWorldMatrix() const
	{
		Matrix4 inverseMat;
		inverseMat.makeTranslation(-m_posWorld);
		Matrix4 matRot;
		Quaternion rot = m_ortWorld;
		rot.conjugate();
		matRot.fromQuan(rot);
		inverseMat = inverseMat * matRot;
		matRot.makeScaling(1.0f/m_sclWorld.x, 1.0f/m_sclWorld.y, 1.0f/m_sclLocal.z);
		inverseMat = inverseMat * matRot;
		return inverseMat;
	}

	void Node::convertWorldToLocalPosition(Vector3& posLocal, const Vector3& posWorld)
	{
		Quaternion ortWorldInv = m_ortWorld;
		ortWorldInv.inverse();
		posLocal = ortWorldInv * (posWorld - m_posWorld) / m_sclWorld;
	}

	void Node::convertWorldToLocalOrientation(Quaternion& ortLocal, const Quaternion& ortWorld)
	{
		Quaternion ortWorldInv = m_ortWorld;
		ortWorldInv.inverse();

		ortLocal = ortWorldInv * ortWorld;
	}

	void Node::needUpdate()
	{
		if (m_bModify)
			return;

		m_bModify = true;
		m_bMatrixDirty = true;

		ChildNodeSet::iterator it = m_children.begin();
		for(; it!=m_children.end(); ++it)
		{
			(*it)->needUpdate();
		}
	}

	void Node::update(float delta, bool bUpdateChildren)
	{
		if (m_bModify)
		{
			if (m_pParent)
			{
				m_ortWorld = m_pParent->m_ortWorld * m_ortLocal;

				// Update scale
				m_sclWorld = m_pParent->m_sclWorld * m_sclLocal;

				// Change position vector based on parent's orientation & scale
				m_posWorld = m_pParent->m_ortWorld * (m_pParent->m_sclWorld * m_posLocal);
				m_posWorld += m_pParent->m_posWorld;
			}
			else
			{
				m_ortWorld = m_ortLocal;
				m_posWorld = m_posLocal;
				m_sclWorld = m_sclLocal;
			}

			m_bModify = false;
		}

		update();

		if (bUpdateChildren)
		{
			for (Node* node : m_children)
			{
				node->update(delta, bUpdateChildren);
			}
		}
	}
}