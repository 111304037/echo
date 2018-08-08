#include "UIManager.h"
#include <Engine/Object/Root.h>

namespace Examples
{
	// ��Ϣ�ص�
	void HandleGoToFunction(const int &mapid, const int &xPos, const int &yPos, const int& zPos, const int& npcid, const int& chefu, int gotodirectly, const int64_t npckey, const int fubenid, const int taskid, const int messageid, const int tasklineid)
	{
		//LuaUtil::LargeInteger key = {};
		//key.quad = npckey;
		// ת��lua�д���
		//HeroGameApplication->FireTaskFindPathEvent(mapid, xPos, yPos, zPos, npcid, chefu, gotodirectly, key.low, key.high, fubenid, taskid, messageid, tasklineid);
	}

	// ��ʾ��ʾ
	void HandleShowRewardItemTips(const int& itemid, float xPos, float yPos, int equipid)
	{

	}

	void OnEmotionFrameChange(const CEGUI::EventArgs& e)
	{
// 		const CEGUI::RichEditBoxCoponentEventArgs& Args = static_cast<const CEGUI::RichEditBoxCoponentEventArgs&>(e);
// 		CEGUI::RichEditboxEmotionComponent* pEmtionCpn = static_cast<CEGUI::RichEditboxEmotionComponent*>(Args.d_Component);
// 
// 		if (pEmtionCpn != NULL)
// 		{
// 
// 			int aniID = pEmtionCpn->GetAniID();
// 			float fElapse = pEmtionCpn->GetElapseTime();
// 			int frame = GetEmotionManager()->GetFrameByElapseTime(aniID, fElapse);
// 			const CEGUI::Image* pImage = GetEmotionManager()->GetEmotionImageByFrame(aniID, frame);
// 			if (pImage != NULL)
// 			{
// 				pEmtionCpn->setImage(pImage);
// 			}
// 
// 		}

		return;
	}

	// ���Tips����
	void HandleTipsLinkClick(const CEGUI::CEGUIString& name, int64_t roleID, int type, int key, int baseid, int64_t shopID, int counterID, const CEGUI::colour& nameColor, int bind, int64_t loseeffecttime)
	{

	}

	//...
	void HandleTeamRecruitClick(int64_t teamid, int recruittype, int param1, int param2)
	{

	}

	void HandleCharNameClick(int64_t roleID, const CEGUI::CEGUIString& roleName, int level, int64_t familyid)//,int recoredID) ��֪��Ϊʲô�ص�������Ӧ���ϣ�����һ��recoreid����ȥ�����������ټӻ���
	{

	}

	void HandleChangelImageClick(const int& changelID)
	{

	}

	void HandleShowCompnentTips(const CEGUI::RichEditboxComponent* pCompnent)
	{

	}

	LORD::Dword HandleUIPlaySound(const CEGUI::CEGUIString& soundRes)
	{
		return 0;
	}

	void HandleLinkTextClick(int keyid)
	{

	}

	// ����ɱ༭����
	void ClickEditArea(const char* str)
	{
	}

	// ʲô��
	void RestoreXPRenderState(void)
	{

	}

	// ��ô���߼���...
	bool HandleCheckShied(CEGUI::CEGUIString& inText)
	{
		return false;
	}

	// ����ʲô��
	void GetEmotionParseInf(int id, bool& bExist, bool& bExitstNextChar)
	{

	}

	// ���캯��
	UIManager::UIManager()
		: m_rootWindow( NULL)
	{
	}

	// ��������
	UIManager::~UIManager()
	{
		CEGUI::CEGUIRenderer::destroySystem();
	}

	// ��ʼ��
	void UIManager::initialize()
	{
		m_uiRenderer = &CEGUI::CEGUIRenderer::bootstrapSystem();

		// ����������
		LORD::Viewport* viewPort = LordRender->getFrameBuffer()->getViewport();
		m_adapter.Initialize(viewPort->getWidth(), viewPort->getHeight());
		CEGUI::System::getSingletonPtr()->SetAdapter(&m_adapter);

		// ������ʾ���ߴ�
		m_uiRenderer->setDisplaySize(CEGUI::Size( (float)m_adapter.GetLogicWidth(), (float)m_adapter.GetLogicHeight()));
		m_uiResourceProvider = CEGUI::System::getSingleton().getResourceProvider<CEGUI::ResourceProvider>();

		// ����������Ⱦ����
		LORD::RenderQueue* pUIRenderQuene = LordSceneManager->getRenderQueue("UI");
		pUIRenderQuene->setManalRenderEnd((LORD::ManualRenderListener*)m_uiRenderer);

		// ������־����
		CEGUI::LoggingLevel loggingMap[] = {
			CEGUI::Insane,
			CEGUI::Informative,
			CEGUI::Warnings,
			CEGUI::Errors,
			CEGUI::Errors,
		};
		CEGUI::Logger::getSingleton().setLoggingLevel(loggingMap[LordLogManager->getLogLevel()]);

		// ��ʼ����Դ��
		initResourceDirectoriesAndGroups();

		// ����Scheme
		CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
		CEGUI::SchemeManager::getSingleton().create("TaharezLook2.scheme");

		// ����UI����
		CEGUI::AnimationManager::getSingleton().loadAnimationsFromXML("example.xml", "animations");

		// �������ͼƬ��
		CEGUI::System::getSingleton().setDefaultMouseCursor("component", "Mouse");
		CEGUI::System::getSingleton().setDefaultTooltip("TaharezLook/Tooltip");
		CEGUI::System::getSingleton().setDefaultCompnenttip("TaharezLook/CompnentTip");

		// �����»�������ˢ��������������ˢ����Ĭ�������
		CEGUI::Image* pUnderLineImage = (CEGUI::Image*)&CEGUI::ImagesetManager::getSingleton().get("component").getImage("UnderLine");
		CEGUI::Image* pTextBrushImage = (CEGUI::Image*)&CEGUI::ImagesetManager::getSingleton().get("component").getImage("TextBrush");
		CEGUI::FontManager::getSingleton().SetUnderLineImage(pUnderLineImage);
		CEGUI::System::getSingleton().SetTextBrushImage(pTextBrushImage);
		CEGUI::System::getSingleton().setDefaultFont("simhei-12");

		// �ص���
		CEGUI::System::getSingleton().SetDefaultGoToFunction((CEGUI::GoToFunction*)(&HandleGoToFunction));
		CEGUI::System::getSingleton().SetDefaultShowItemTips((CEGUI::ShowItemTips*)(&HandleShowRewardItemTips));
		CEGUI::System::getSingleton().SetEmotionFrameChangeFunction((CEGUI::EmotionChangeFrameFunction*)(&OnEmotionFrameChange));
		CEGUI::System::getSingleton().SetTipsLinkClickFunc((CEGUI::TipsLinkClick*)(&HandleTipsLinkClick));
		CEGUI::System::getSingleton().SetDefaultTeamRecruitFunction((CEGUI::OnTeamRecruitClick*)(&HandleTeamRecruitClick));
		CEGUI::System::getSingleton().SetNameClickFuction((CEGUI::OnNameLinkClick*)(&HandleCharNameClick));
		CEGUI::System::getSingleton().SetChangelClickFuction((CEGUI::OnChangelImageClick*)(&HandleChangelImageClick));
		CEGUI::System::getSingleton().SetCompnentTips((CEGUI::ShowCompnentTips*)(&HandleShowCompnentTips));
		CEGUI::System::getSingleton().SetUIPlaySoundFunc((CEGUI::OnUIPlaySound*)(&HandleUIPlaySound));
		CEGUI::System::getSingleton().SetLinkTextClickFuction((CEGUI::OnLinkTextClick*)(&HandleLinkTextClick));
		CEGUI::System::getSingleton().SetCheckShiedFunc((CEGUI::OnCheckShied*)(&HandleCheckShied));
		CEGUI::System::getSingleton().SetRestoreXPStateFunc((CEGUI::OnRestoreXPRenderState*)(&RestoreXPRenderState));
		CEGUI::System::getSingleton().SetOnClickEditAreaFunc((CEGUI::OnClickEditArea*)(&ClickEditArea));
		
		CEGUI::System::getSingleton().SetEmotionParseInfFunc((CEGUI::EmotionParseInfFunc*)(&GetEmotionParseInf));
		CEGUI::System::getSingleton().SetEmotionNum(49);

		// ����������
		m_rootWindow = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "root_wnd");
		m_rootWindow->setMousePassThroughEnabled(true);

		// ����������
		CEGUI::System::getSingleton().setGUISheet(m_rootWindow);
		m_rootWindow->setMousePassThroughEnabled(true);
		m_rootWindow->setDistributesCapturedInputs(true);

		// ģ̬
		CEGUI::System::getSingleton().subscribeEvent(CEGUI::System::EventStartModalState, CEGUI::Event::Subscriber(&UIManager::onStartModalState, this));

		// ����layoutǰ����
		CEGUI::WindowManager::getSingleton().subscribeEvent(CEGUI::WindowManager::EventBeforeLoadlayout,CEGUI::Event::Subscriber(&UIManager::handleBeforeLoadlayout, this));

		// ��ʼ��Lua�ű�ģ��
		//CEGUI::System::getSingletonPtr()->setScriptingModule( &CEGUI::LuaScriptModule::create(HeroCCLuaEngine->getLuaState()));
	}

	// ��ʼ����ԴĿ¼
	void UIManager::initResourceDirectoriesAndGroups()
	{
// 		std::string pathPrefix = "/";
// 		m_uiResourceProvider->setResourceGroupDirectory("animations", pathPrefix + "ui/animations");
// 		m_uiResourceProvider->setResourceGroupDirectory("fonts", pathPrefix + "ui/fonts");
// 		m_uiResourceProvider->setResourceGroupDirectory("imagesets", pathPrefix + "ui/imagesets");
// 		m_uiResourceProvider->setResourceGroupDirectory("layouts", pathPrefix + "ui/layouts");
// 		m_uiResourceProvider->setResourceGroupDirectory("looknfeels", pathPrefix + "ui/looknfeel");
// 		m_uiResourceProvider->setResourceGroupDirectory("lua_scripts", pathPrefix + "ui/lua_scripts");
// 		m_uiResourceProvider->setResourceGroupDirectory("schemes", pathPrefix + "ui/schemes");
// 		m_uiResourceProvider->setResourceGroupDirectory("schemas", pathPrefix + "ui/xml_schemas");

		CEGUI::Imageset::setDefaultResourceGroup("imagesets");
		CEGUI::Font::setDefaultResourceGroup("fonts");
		CEGUI::Scheme::setDefaultResourceGroup("schemes");
		CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
		CEGUI::WindowManager::setDefaultResourceGroup("layouts");
		CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
		CEGUI::AnimationManager::setDefaultResourceGroup("animations");
	}

	// ���ڴ�С�ı�
	void UIManager::onSize(int screenW, int screenH)
	{
		UIAdapter* adapter = LORD_DOWN_CAST<UIAdapter*>(CEGUI::System::getSingleton().GetAdapter());
		if (adapter)
		{
			adapter->Initialize(screenW, screenH);
		}
	}

	// ����
	void UIManager::tick(int delta)
	{
		CEGUI::System::getSingleton().injectTimePulse(delta * 0.001f);
	}

	// �¼�
	bool UIManager::onStartModalState(const CEGUI::EventArgs& e)
	{
		return true;
	}

	// �¼�
	bool UIManager::handleBeforeLoadlayout(const CEGUI::EventArgs &e)
	{
		return true;
	}
}