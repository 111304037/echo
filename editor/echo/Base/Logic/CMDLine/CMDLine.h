#pragma once

class QSplashScreen;

namespace Echo
{
	/**
	 * �����н��� 2012-8-16 ����
	 */
	class CMDLine
	{
	public:
		// ���������
		static bool Parser(int argc, char* argv[], QSplashScreen* splash);

		// �ͷ�
		static void Release();
	};
}