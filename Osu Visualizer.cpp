// Osu Visualizer.cpp : Définit le point d'entrée de l'application.
//

#include "framework.h"
#include "Osu Visualizer.h"
#include <commdlg.h> // Nécessaire pour la boîte de dialogue de sélection de fichier

#define MAX_LOADSTRING 100


// Variables globales :
HINSTANCE hInst;                                // instance actuelle
WCHAR szTitle[MAX_LOADSTRING];                  // Texte de la barre de titre
WCHAR szWindowClass[MAX_LOADSTRING];            // nom de la classe de fenêtre principale
HWND hwndLabelGlobal;
HWND hwndButtonGo;

// Déclarations anticipées des fonctions incluses dans ce module de code :
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Placez le code ici.

    // Initialise les chaînes globales
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_OSUVISUALIZER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Effectue l'initialisation de l'application :
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OSUVISUALIZER));

    MSG msg;

    // Boucle de messages principale :
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FONCTION : MyRegisterClass()
//
//  OBJECTIF : Inscrit la classe de fenêtre.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OSUVISUALIZER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_OSUVISUALIZER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FONCTION : InitInstance(HINSTANCE, int)
//
//   OBJECTIF : enregistre le handle d'instance et crée une fenêtre principale
//
//   COMMENTAIRES :
//
//        Dans cette fonction, nous enregistrons le handle de l'instance dans une variable globale, puis
//        nous créons et affichons la fenêtre principale du programme.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

   hInst = hInstance; // Stocke le handle d'instance dans la variable globale
   int width = 400;
   int height = 110;

   int screenWidth = GetSystemMetrics(SM_CXSCREEN);
   int screenHeight = GetSystemMetrics(SM_CYSCREEN);

   int x = (screenWidth - width) / 2;
   int y = (screenHeight - height) / 2;

   int btnGoX = (width - 150) / 2;
   int btnGoY = 40; // Positionner le bouton vers le bas

   HWND hWnd = CreateWindowW(szWindowClass, szTitle,
       WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME, // Modifier les styles ici
       x, y, width, height, nullptr, nullptr, hInstance, nullptr);

   HWND hwndButton = CreateWindow(
       L"BUTTON",  // Pré-défini 'BUTTON' dans Windows API
       L"Ouvrir fichier",  // Texte du bouton
       WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles du bouton
       10,         // Position X
       10,         // Position Y
       150,        // Largeur
       24,         // Hauteur
       hWnd,       // Fenêtre parent
       (HMENU)IDM_OPEN_FILE, // ID du bouton pour le reconnaître dans WndProc
       hInstance,
       NULL);

   HWND hwndLabel = CreateWindow(
       L"STATIC",
       L"Aucun fichier sélectionné",
       WS_VISIBLE | WS_CHILD | SS_PATHELLIPSIS,
       170, 10, 220, 24, // Vous pouvez augmenter la largeur ici
       hWnd,
       NULL,
       hInstance,
       NULL);

   hwndButtonGo = CreateWindow(
       L"BUTTON",  // Pré-défini 'BUTTON' dans Windows API
       L"Go !",    // Texte du bouton
       WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON, // Ne pas inclure WS_VISIBLE pour l'instant
       btnGoX, btnGoY, 150, 24,  // Position et dimensions
       hWnd, (HMENU)IDM_GO, // Identifier pour le bouton "Go !"
       hInstance, NULL);

   if (hwndButtonGo == NULL) {
       MessageBox(hWnd, L"La création du bouton 'Go !' a échoué.", L"Erreur", MB_OK | MB_ICONERROR);
   }

   hwndLabelGlobal = hwndLabel;

   SetFocus(hwndButton);

   if (!hWnd)
   {
      return FALSE;
   }
   // Créez une police standard
   HFONT hFont = CreateFont(
       -MulDiv(10, GetDeviceCaps(GetDC(hWnd), LOGPIXELSY), 72), // Taille 10
       0, 0, 0, FW_NORMAL, // FW_NORMAL pour non-gras
       FALSE, FALSE, FALSE, ANSI_CHARSET,
       OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
       DEFAULT_PITCH | FF_DONTCARE, TEXT("Segoe UI"));

   // Appliquez cette police au bouton
   SendMessage(hwndButton, WM_SETFONT, (WPARAM)hFont, TRUE);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


void OpenFile(HWND hWnd, HWND hwndLabel) {
    OPENFILENAME ofn;
    WCHAR szFile[260];

    ZeroMemory(&ofn, sizeof(ofn));
    ZeroMemory(szFile, sizeof(szFile));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"Fichiers OSZ (*.osz)\0*.osz\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE) {
        // Met à jour le label avec le chemin du fichier sélectionné
        SetWindowText(hwndLabel, szFile);
        // Rendre le bouton "Go !" visible
        ShowWindow(hwndButtonGo, SW_SHOW);
        UpdateWindow(hwndButtonGo);
    }
}

//
//  FONCTION : WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  OBJECTIF : Traite les messages pour la fenêtre principale.
//
//  WM_COMMAND  - traite le menu de l'application
//  WM_PAINT    - Dessine la fenêtre principale
//  WM_DESTROY  - génère un message d'arrêt et retourne
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case IDM_OPEN_FILE: 
            OpenFile(hWnd, hwndLabelGlobal);
            break;
        case IDM_GO:
            MessageBox(hWnd, L"Le bouton 'Go !' a été cliqué.", L"Action", MB_OK);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // Code de dessin ici
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


// Gestionnaire de messages pour la boîte de dialogue À propos de.
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
