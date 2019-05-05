// dui-demo.cpp : main source file
//

#include "stdafx.h"
#include <helper/MenuWndHook.h>
#include <helper/mybuffer.h>
#include "httpsvr/HTTPServer.h"
#include "MemFlash.h"
#include "../controls.extend/propgrid/SPropertyGrid.h"
#include "../controls.extend/SFadeFrame.h"
#include "../controls.extend/sradiobox2.h"
#include "../controls.extend/SVscrollbar.h"
#include "../controls.extend/SSkinNewScrollBar.h"
#include "../controls.extend/SChromeTabCtrl.h"
#include "../controls.extend/siectrl.h"
#include "../controls.extend/schatedit.h"
#include "../controls.extend/SScrollText.h"
#include "../controls.extend/SCalendar2.h"

#include "../controls.extend/SMcListViewEx\SHeaderCtrlEx.h"
#include "../controls.extend/SMcListViewEx\SMcListViewEx.h"

#include "uianimation/UiAnimationWnd.h"
#include "appledock/SDesktopDock.h"
#include "SMatrixWindow.h"
#include "SmileyCreateHook.h"
#include "clock/sclock.h"

//<--��һ��filter="demo"��slog���
#undef LOG_FILTER
#define LOG_FILTER "demo"
#include <helper/slog-def.h>
//-->

#include "MainDlg.h"

#ifdef _DEBUG
#define RES_TYPE 0      //���ļ��м�����Դ������ʧ���ٴ�PE����
#else
#define RES_TYPE 1		//��PE��Դ�м���UI��Դ
#endif
// #define RES_TYPE 1   //��PE��Դ�м���UI��Դ
// #define RES_TYPE 2   //��zip���м�����Դ

#include "../components/resprovider-zip/zipresprovider-param.h"

#ifdef _DEBUG
#define SYS_NAMED_RESOURCE _T("soui-sys-resourced.dll")
#pragma comment(lib,"lua-52d")
#pragma comment(lib,"scriptmodule-luad")
#else
#define SYS_NAMED_RESOURCE _T("soui-sys-resource.dll")
#pragma comment(lib,"lua-52")
#pragma comment(lib,"scriptmodule-lua")
#endif

#include "../controls.extend/smiley/ssmileyctrl.h"
#include "skin/SDemoSkin.h"
#include "skin/SSkinLoader.h"
#include "trayicon/SShellNotifyIcon.h"
#include "qrcode/SQrCtrl.h"

#include <interface/STaskLoop-i.h>
#include <helper/SFunctor.hpp>
#include <string>

ROBJ_IN_CPP

//��ʾ�첽����
class CAsyncTaskObj
{
public:
	void task1(int a)
	{
		SLOG_INFO("task1,a:" << a);
	}

	void task2(int a, const std::string & b)
	{
		SLOG_INFO("task2,a:" << a<<" b:"<<b.c_str());
	}
};

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpstrCmdLine*/, int /*nCmdShow*/)
{
    
    //����Ҫ����OleInitialize����ʼ�����л���
    HRESULT hRes = OleInitialize(NULL);
    SASSERT(SUCCEEDED(hRes));


    //LoadLibrary(L"E:\\soui.taobao\\richedit\\Debug\\riched20.dll");
    
    /* XMLԤ����ǰ�����Ч�ʱȽ�
    pugi::xml_document doc;
    
    LARGE_INTEGER perf;
    QueryPerformanceFrequency(&perf);
    LARGE_INTEGER t1,t2;
    QueryPerformanceCounter(&t1);
    doc.load_file(L"e:\\play.xml",pugi::parse_default,pugi::encoding_utf8);
    QueryPerformanceCounter(&t2);
    doc.save_file(L"e:\\play1.xml");
    
    
    doc.save_bin(L"e:\\paly.xml.bin");
    pugi::xml_document doc2;
    LARGE_INTEGER t21,t22;
    QueryPerformanceCounter(&t21);
    doc2.load_bin_file(L"e:\\paly.xml.bin");
    QueryPerformanceCounter(&t22);
    doc2.save_file(L"e:\\play2.xml");
    
    LONGLONG SP1 = (t2.QuadPart-t1.QuadPart)*1000000/perf.QuadPart;        
    LONGLONG SP2 = (t22.QuadPart-t21.QuadPart)*1000000/perf.QuadPart;
    
    SStringW str;
    str.Format(L"!!!!!sp2=%d,sp1=%d\n",(int)SP2,(int)SP1);
    MessageBoxW(GetActiveWindow(),str,L"span",MB_OK);
    return 0;    
    */
    int nRet = 0; 

    //ʹ��imgdecoder-pngͼƬ����ģ����ʾapng����
    SComMgr2 *pComMgr = new SComMgr2(_T("imgdecoder-png"));
    


    {

        int nType=MessageBox(GetActiveWindow(),_T("ѡ����Ⱦ���ͣ�\n[yes]: Skia\n[no]:GDI\n[cancel]:Quit"),_T("select a render"),MB_ICONQUESTION|MB_YESNOCANCEL);
        if(nType == IDCANCEL)
        {
            nRet = -1;
            goto exit;
        }


        //����һ����SOUIϵͳ��ʹ�õ���COM���
        //CAutoRefPtr��һ��SOUIϵͳ��ʹ�õ�����ָ����
        CAutoRefPtr<IImgDecoderFactory> pImgDecoderFactory; //ͼƬ����������imagedecoder-wid.dllģ���ṩ
        CAutoRefPtr<IRenderFactory> pRenderFactory;         //UI��Ⱦģ�飬��render-gdi.dll����render-skia.dll�ṩ
        CAutoRefPtr<ITranslatorMgr> trans;                  //�����Է���ģ�飬��translator.dll�ṩ
        CAutoRefPtr<IScriptFactory> pScriptLua;              //lua�ű�ģ�飬��scriptmodule-lua.dll�ṩ
        CAutoRefPtr<ILog4zManager>  pLogMgr;                //log4z����
        
		//��ʾ�첽����
		CAutoRefPtr<ITaskLoop>  pTaskLoop;
		if (pComMgr->CreateTaskLoop((IObjRef**)&pTaskLoop))
		{
			CAsyncTaskObj obj;
			pTaskLoop->start("test", ITaskLoop::Low);
			STaskHelper::post(pTaskLoop, &obj, &CAsyncTaskObj::task1, 100,true);
			STaskHelper::post(pTaskLoop, &obj, &CAsyncTaskObj::task2, 100,"abc", true);
			pTaskLoop->stop();
			pTaskLoop = NULL;
		}

		BOOL bLoaded = FALSE;
		//�Ӹ��������ʽ���������������
		if (nType == IDYES)
		{
			bLoaded = pComMgr->CreateRender_Skia((IObjRef**)&pRenderFactory);			
		}
        else
            bLoaded = pComMgr->CreateRender_GDI((IObjRef**)&pRenderFactory);
        SASSERT_FMT(bLoaded,_T("load interface [%s] failed!"),nType==IDYES?_T("render_skia"):_T("render_gdi"));
        bLoaded=pComMgr->CreateImgDecoder((IObjRef**)&pImgDecoderFactory);
        SASSERT_FMT(bLoaded,_T("load interface [%s] failed!"),_T("imgdecoder"));
        bLoaded=pComMgr->CreateTranslator((IObjRef**)&trans);
        SASSERT_FMT(bLoaded,_T("load interface [%s] failed!"),_T("translator"));

        if(pComMgr->CreateLog4z((IObjRef**)&pLogMgr))
        if(pLogMgr){
            pLogMgr->createLogger("soui");//support output soui trace infomation to log
            pLogMgr->start();
        }
        
        //Ϊ��Ⱦģ����������Ҫ���õ�ͼƬ����ģ��
        pRenderFactory->SetImgDecoderFactory(pImgDecoderFactory);

        //����һ��Ψһ��SApplication����SApplication��������Ӧ�ó������Դ
        SApplication *theApp=new SApplication(pRenderFactory,hInstance);
        
        theApp->SetLogManager(pLogMgr);
        SLOG_INFO("test="<<200);
        SLOGFMTE("log output using ansi format,str=%s, tick=%u","test",GetTickCount());
        SLOGFMTE(L"log output using unicode format,str=%s, tick=%u",L"����",GetTickCount());
        
        //�ؼ�ע��Ҫ�ŵ�AddResProviderǰ�� 2016��3��8��
        
        //��SApplicationϵͳ��ע�����ⲿ��չ�Ŀؼ���SkinObj��
        SWkeLoader wkeLoader;
        if(wkeLoader.Init(_T("wke.dll")))        
        {
            theApp->RegisterWindowClass<SWkeWebkit>();//ע��WKE�����
        }
        theApp->RegisterWindowClass<SMatrixWindow>();//
        theApp->RegisterWindowClass<SFreeMoveWindow>();//
        theApp->RegisterWindowClass<SClock>();//
        theApp->RegisterWindowClass<SGifPlayer>();//theApp�����ӷ�����RegisterWindowClass,�滻RegisterWndFactory(TplSWindowFactory<SGifPlayer>())
        theApp->RegisterSkinClass<SSkinGif>();//ע��SkinGif
        theApp->RegisterSkinClass<SSkinAPNG>();//ע��SSkinAPNG
        theApp->RegisterSkinClass<SSkinVScrollbar>();//ע�����������Ƥ��
        theApp->RegisterSkinClass<SSkinNewScrollbar>();//ע�����������Ƥ��
		theApp->RegisterSkinClass<SDemoSkin>();

		theApp->RegisterWindowClass<SMCListViewEx>();
		theApp->RegisterWindowClass<SHeaderCtrlEx>();

        theApp->RegisterWindowClass<SIPAddressCtrl>();//ע��IP�ؼ�
        theApp->RegisterWindowClass<SPropertyGrid>();//ע�����Ա�ؼ�
        theApp->RegisterWindowClass<SChromeTabCtrl>();//ע��ChromeTabCtrl
        theApp->RegisterWindowClass<SIECtrl>();//ע��IECtrl
        theApp->RegisterWindowClass<SChatEdit>();//ע��ChatEdit
        theApp->RegisterWindowClass<SScrollText>();//ע��SScrollText
        theApp->RegisterWindowClass<SDesktopDock>();//ע��SDesktopDock
        theApp->RegisterWindowClass<SImageMaskWnd>();//ע��SImageMaskWnd
        theApp->RegisterWindowClass<SRatingBar>();//ע��SRatingBar
		theApp->RegisterWindowClass<SInterpolatorView>();
		theApp->RegisterWindowClass<SPathView>();
		theApp->RegisterWindowClass<SQrCtrl>();
		theApp->RegisterWindowClass<SProgressRing>();
		theApp->RegisterWindowClass<SCheckBox2>();
		theApp->RegisterWindowClass<SAniWindow>();
		theApp->RegisterWindowClass<SGroupList>();

        if(SUCCEEDED(CUiAnimation::Init()))
        {
            theApp->RegisterWindowClass<SUiAnimationWnd>();//ע�ᶯ���ؼ�
        }
        theApp->RegisterWindowClass<SFadeFrame>();//ע�ὥ���������ؼ�
        theApp->RegisterWindowClass<SRadioBox2>();//ע��RadioBox2
        theApp->RegisterWindowClass<SCalendar2>();//ע��SCalendar2

		theApp->RegisterWindowClass<SShellNotifyIcon>();

        SSkinGif::Gdiplus_Startup();
        
        //�����Ҫ�ڴ�����ʹ��R::id::namedid���ַ�ʽ��ʹ�ÿؼ�����Ҫ��һ�д��룺2016��2��2�գ�R::id::namedXmlID����uiresbuilder ����-h .\res\resource.h idtable ��3�����������ɵġ�
        theApp->InitXmlNamedID(namedXmlID,ARRAYSIZE(namedXmlID),TRUE);
        
        //�����������·���޸ĵ�demo���ڵ�Ŀ¼
#ifdef _DEBUG
#   ifdef DEMO_DEBUG_UI_RESOURCES_SEARCH_PATH
        SStringT strResDir = _T(DEMO_DEBUG_UI_RESOURCES_SEARCH_PATH);
#   else
        SStringT strResDir = theApp->GetAppDir();
        strResDir += _T("\\..\\demo");
#   endif
        SetCurrentDirectory(strResDir);
        //SOUIϵͳ���Ǵ�appdirȥ������Դ
        theApp->SetAppDir(strResDir);
#endif

        //����һ�˸���Դ�ṩ����,SOUIϵͳ��ʵ����3����Դ���ط�ʽ���ֱ��Ǵ��ļ����أ���EXE����Դ���ؼ���ZIPѹ��������
        CAutoRefPtr<IResProvider>   pResProvider;
#if (RES_TYPE == 0)//���ļ�����
        CreateResProvider(RES_FILE,(IObjRef**)&pResProvider);
        if(!pResProvider->Init((LPARAM)_T("uires"),0))
        {
            CreateResProvider(RES_PE,(IObjRef**)&pResProvider);
            if(!pResProvider->Init((WPARAM)hInstance,0))
            {
                SASSERT(0);
                delete theApp;
                nRet = 1;
                goto exit;
            }
        }
#elif (RES_TYPE==1)//��EXE��Դ����
        CreateResProvider(RES_PE,(IObjRef**)&pResProvider);
        pResProvider->Init((WPARAM)hInstance,0);
#elif (RES_TYPE==2)//��ZIP������
        bLoaded=pComMgr->CreateResProvider_ZIP((IObjRef**)&pResProvider);
        SASSERT_FMT(bLoaded,_T("load interface [%s] failed!"),_T("resprovider_zip"));

        ZIPRES_PARAM param;
        param.ZipFile(pRenderFactory, _T("uires.zip"),"souizip");
        bLoaded = pResProvider->Init((WPARAM)&param,0);
        SASSERT(bLoaded);
#endif
        //��������IResProvider����SApplication����
        theApp->AddResProvider(pResProvider);

		SSkinLoader *SkinLoader = new SSkinLoader(theApp);
		SkinLoader->LoadSkin(_T("themes\\skin1"));

        //����һ��http����������������Դ�м���flash
        CMemFlash   memFlash;

        CHTTPServer flashSvr(&memFlash);
        flashSvr.Start(CMemFlash::HomeDir(),"",82,0);

        if(trans)
        {//�������Է����
            theApp->SetTranslator(trans);
            pugi::xml_document xmlLang;
            if(theApp->LoadXmlDocment(xmlLang,_T("lang_cn"),_T("translator")))
            {
                CAutoRefPtr<ITranslator> langCN;
                trans->CreateTranslator(&langCN);
                langCN->Load(&xmlLang.child(L"language"),1);//1=LD_XML
                trans->InstallTranslator(langCN);
            }
        }
#if (defined(DLL_CORE) || defined(LIB_ALL)) && !defined(_WIN64)
        //����LUA�ű�ģ�飬ע�⣬�ű�ģ��ֻ����SOUI�ں�����DLL��ʽ����ʱ����ʹ�á�
        bLoaded=pComMgr->CreateScrpit_Lua((IObjRef**)&pScriptLua);
        SASSERT_FMT(bLoaded,_T("load interface [%s] failed!"),_T("scirpt_lua"));
        theApp->SetScriptFactory(pScriptLua);
#endif//DLL_CORE

        
        //����ϵͳ��Դ
        HMODULE hSysResource=LoadLibrary(SYS_NAMED_RESOURCE);
        if(hSysResource)
        {
            CAutoRefPtr<IResProvider> sysSesProvider;
            CreateResProvider(RES_PE,(IObjRef**)&sysSesProvider);
            sysSesProvider->Init((WPARAM)hSysResource,0);
            theApp->LoadSystemNamedResource(sysSesProvider);
        }
        //����hook���Ʋ˵��ı߿�
        CMenuWndHook::InstallHook(hInstance,L"_skin.sys.menu.border");
        
        //����ȫ����Դ����XML
        //theApp->Init(_T("xml_init")); ������Ҫ��䣬��AddResProviderʱ�Զ�ִ�г�ʼ��
        
        //��ʾR.color.xxx,R.string.xxx�ڴ����е�ʹ�á�
        COLORREF crRed = GETCOLOR(R.color.red);
        SStringW strTitle = GETSTRING(R.string.title);
        
		SNotifyCenter *pNotifyCenter = new SNotifyCenter;
        {
			//������ʾ���ڲ���
			CTipWnd::SetLayout(_T("layout:dlg_tip"));

            //��������ʾʹ��SOUI����Ӧ�ó��򴰿�,Ϊ�˱��洰�ڶ�������������������󣬰���������һ�㡣
            CMainDlg dlgMain;  
            dlgMain.Create(GetActiveWindow(), 0,0,888,650);

            dlgMain.GetNative()->SendMessage(WM_INITDIALOG);
            dlgMain.CenterWindow();
            dlgMain.ShowWindow(SW_SHOWNORMAL);

            SmileyCreateHook  smileyHook; //��֪��MainDlg���Ŀ��mhook��ͻ�ˣ���win10�У����hook�ŵ�dlgmain.createǰ�ᵼ��hookʧ�ܡ�
            nRet=theApp->Run(dlgMain.m_hWnd);
        }
		delete pNotifyCenter;

        theApp->UnregisterWindowClass<SGifPlayer>();
        //Ӧ�ó����˳�
        delete theApp; 
		delete SkinLoader;
        if(pLogMgr)
        {
            pLogMgr->stop();
        }
        
        flashSvr.Shutdown();

        //ж�ز˵��߿����hook
        CMenuWndHook::UnInstallHook();
        CUiAnimation::Free();
                
        SSkinGif::Gdiplus_Shutdown();
    }
exit:
    delete pComMgr;
    

    OleUninitialize();

    return nRet;
}


