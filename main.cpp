// Client.cpp : 애플리케이션에 대한 진입점을 정의합니다.

#include "global.h"
#include "framework.h"
#include "Client.h"

#include "CCore.h"


#define MAX_LOADSTRING 100
#define ADD(a,b) a + b

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
HWND g_hWnd;                                    // 메인 윈도우 핸들
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// SAL (Standard Annotation Language) 소스 코드 주석 언어

// 지역 변수
// 전역 변수
// 정적 변수(데이터 영역)
// 1. 함수 안에 선언시 해당 함수에서만 접근 가능
// 2. 파일 안에 선언시 해당 파일에서 접근 가능(다른 파일에 이름이 같은 정적변수가 있어도 중뵥 규칙이 발생하지않음)
// 3. 클래스 안에 선언
// 외부 변수

class CClass
{
private:
    int m_i;

    // 정적 멤버(데이터 영역), 클래스 안에서 접근 가능하지만 데이터 영역에 딱 1개만 존재함.
    // 다른 클래스를 만들어도 정적 멤버는 추가로 생성되지 않음
    static int m_iStatic;
    //public:
    //    static int m_iStatic;
public:
    void func()
    {
        this->m_i;
    }

    // 정적 멤버함수, 객체 없이 호출 가능
    // 멤버에 접근 불가능
    static void FUNC()
    {
        // this 키워드가 없음(멤버 접근 불가)
        //this->m_i;

        // 정적 멤버는 접근 가능
        m_iStatic = 1;

    }
};

// 정적 멤버는 반드시 클래스 선언 밖에서 초기화 해야함
int CClass::m_iStatic = 0;

int Add(int a, int b)
{
    return a + b;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, // 실행 된 프로세스의 시작 가상메모리 주소
    _In_opt_ HINSTANCE hPrevInstance, // 초창기 윈도우에서 이전에 실행 된 프로세스의 시작 실제메모리 주소, 현재는 가상메모리의 사용으로 인해 사용 안함
    _In_ LPWSTR    lpCmdLine, // 프롬포트에서 실행할 때 추가로 받아오는 문자열
    _In_ int       nCmdShow)
{
    // 메모리 누수 체크
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(221); // 메모리 누수 발생한곳에서 중단점 걸기
    //new int; // 메모리 누수 발생

    UNREFERENCED_PARAMETER(hPrevInstance); //UNREFERENCED_PARAMETER는 입력값을 그대로 출력함. 즉 변하는게 없음 -> 주석과 같이 아무 일을 하지않음
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.
    //int a = 100;
    //switch (a)
    //{
    //case 100:
    //// switch문 안에서 변수 선언을 하고싶을때는 반드시 {}로 묶어줘야함
    //{
    //    int b = 200;
    //}
    //break;
    //case 200:
    //    break;
    //default:
    //    break;
    //}

    CClass a;
    a.func();
    a.FUNC(); // 객체를 통해 호출도 가능하긴 하지만 의미가 없음
    CClass::FUNC();
    //CClass::m_iStatic = 0; // public인경우 접근 가능

    //CCore* pCore = CCore::GetInstance();

    //int iCal = Add(10, 20);
    int iCal1 = 10 * Add(10, 20); // 300 함수이기 때문에 10*(10+20)
    int iCal2 = 10 * ADD(10, 20); // 120 전처리기로 치환하는것이기 때문에 10*10+20

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING); // 프로젝트 제목, 직접 지정하고 싶으면 szTitle을 L"제목"으로 바꿔주면 됨
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);

    // 윈도우 정보 등록
    MyRegisterClass(hInstance);

    // 윈도우 생성 (프로세스와 윈도우는 다름, 윈도우는 눈에 보이는 창)
    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    // core 초기화
    if (FAILED(CCore::GetInst()->init(g_hWnd, POINT{1280,768})))
    {
        MessageBox(nullptr, L"Core 객체 초기화 실패",L"ERROR",MB_OK);

        return FALSE;
    }



    // 단축키 테이블 정보 로딩
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

    MSG msg;

    // 타이머 설정
    //SetTimer(g_hWnd, 0, 0, nullptr); // 1000 == 1초 프레임 초당 1 0 == 프레임 제한없음

    // 기본 메시지 루프입니다:
    // GetMessage는 메세지 큐에서 메세지가 확인될 때 까지 0대기
    // msg.message == WM_QUIT 인 경우 false를 반환 -> 프로그램 종료

    //while (GetMessage(&msg, nullptr, 0, 0))
    //{

    //    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
    //    {
    //        TranslateMessage(&msg);
    //        DispatchMessage(&msg); 
    //    }
    //}

    //KillTimer(g_hWnd, 0); // 타이머도 커널오브젝트여서 프로그램 종료시 제거시켜줘야함
            // peek : 몰래 보다

    // PeekMessage
    // 메세지 유무와 관계없이 반환
    // 메세지 큐에서 메세지를 확인한 경우 true, 메세지 큐에 메세지가 없는경우 false를 반환한다.

    // 메세지가 없을때가 훨씬 많기 때문에 동작을 메세지가 없는곳에 할당해주는게 효율적임

    //DWORD dwPrevCount = GetTickCount64();
    //DWORD dwAccCount = 0;

    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            //int iTime = GetTickCou nt64();

            if (WM_QUIT == msg.message)
            {
                break;
            }

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

            //dwAccCount += (GetTickCount64() - iTime);

        }
        // 메세지가 발생하지 않는 대부분의 시간
        else
        {
            //DWORD dwCurCount = GetTickCount64();
            //if (dwCurCount - dwPrevCount > 1000)
            //{
            //    float fRatio = (float)dwAccCount / 1000.f;

            //    wchar_t szBuff[50] = {};
            //    swprintf_s(szBuff, L" 비율 : %f", fRatio);
            //    SetWindowText(g_hWnd,szBuff);
            //    dwPrevCount = dwCurCount;
            //}

            // Game 코드 수행
            // 디자인 패턴(설계 유형)
            // 싱글톤 패턴

            CCore::GetInst()->progress();
        }
    }


    return (int)msg.wParam;
}





//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr; // MAKEINTRESOURCEW(IDC_CLIENT); // nullptr로 설정하면 메뉴를 쓰지 않음
    wcex.lpszClassName = szWindowClass; // 윈도우 정보를 설정하는 key 값
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex); // 윈도우에서 제공하는 함수
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    // hwnd(윈도우 핸들) : 윈도우를 구별하는 ID역할을 함

    g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!g_hWnd)
    {
        return FALSE;
    }

    ShowWindow(g_hWnd, nCmdShow); // nCmdShow를 false로 하면 윈도우 창이 숨김처리됨
    UpdateWindow(g_hWnd);

    return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

struct tObjInfo
{
    POINT g_ptObjPos = { 500,300 };
    POINT g_ptObjScale = { 100,100 };
};

INT_PTR CALLBACK TileCountProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case ID_MENU_TITLE:
        {
            INT_PTR iRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_TILE_COUNT), hWnd, TileCountProc);

            if (iRet == IDOK)
            {

            }
        }

            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT: // 무효화 영역(invalidate Rect)이 발생한 경우
    {
        PAINTSTRUCT ps;

        // Device Context를 만들어서 ID를 반환
        HDC hdc = BeginPaint(hWnd, &ps); // Device Context 핸들역할을 함 (그리기)
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

        //Rectangle(hdc
        //    , 1180
        //    , 668
        //    , 1280
        //    , 768
        //);

        // 그리기 종료
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_KEYDOWN:
        break;

    case WM_LBUTTONDOWN:
        break;

    case WM_MOUSEMOVE:
        break;

    case WM_LBUTTONUP:
        break;


    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}