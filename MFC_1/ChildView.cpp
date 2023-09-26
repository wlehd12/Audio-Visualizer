
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "MFC_1.h"
#include "ChildView.h"
#include "windows.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView
#pragma comment (lib, "winmm.lib")

CChildView::CChildView()
{
	_pos.SetPoint(600, 400);
	_size = 20;
	tunnel = 1;
	p_data = NULL;
	m_hWaveIn = NULL;
	m_hWaveOut = NULL;
	// 녹음 또는 재생에 사용할 소리 정보를 설정한다.

	m_format.wFormatTag = WAVE_FORMAT_PCM;   // PCM
	m_format.nChannels = 2;                  // 마이크 녹음방식 : 스테레오
	m_format.nSamplesPerSec = 8000;          // 샘플링 주기 : 8 KHz
	m_format.wBitsPerSample = 16;            // 샘플링 단위 : 16 Bits = 2 bytes
	// 샘플당 비트수/8(샘플당 바이트수) * 채널 수
	m_format.nBlockAlign = m_format.nChannels*m_format.wBitsPerSample / 8;
	// 1초당 데이터를 입출력하는데에 필요한 바이트 수를 구한다.
	// 샘플링 * 샘플당 비트수/8 * 채널수
	m_format.nAvgBytesPerSec = m_format.nSamplesPerSec*m_format.nBlockAlign;
	m_format.cbSize = 0; // WAVEFORMATEX 구조체 정보외에 추가적인 정보가 없다.
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_WM_KEYUP()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	
	ON_COMMAND(ID_AUDIOVISUALIZER_CIRCLE, &CChildView::OnAudiovisualizerCircle)
	ON_COMMAND(ID_AUDIOVISUALIZER_GRAPH, &CChildView::OnAudiovisualizerGraph)
	ON_COMMAND(ID_AUDIOVISUALIZER_WAVE, &CChildView::OnAudiovisualizerWave)
END_MESSAGE_MAP()



// CChildView 메시지 처리기

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this);
	CRect rect;
	GetClientRect(&rect);

	CDC memDc;
	memDc.CreateCompatibleDC(&dc);

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());		//화면에 맞게  bitmap 생성
	memDc.SelectObject(&bitmap);
	
	CBrush brush1(RGB(0, 100, 100));
	CBrush brush2(RGB(0, 0, 200));
	CPen graph_pen(PS_SOLID, 1, RGB(0, 255, 0));	
	CPen *p_old_pen = memDc.SelectObject(&graph_pen);
	switch (tunnel)
	{
		case 1:					// 원으로 표현
			memDc.SelectObject(&brush1);
			memDc.Ellipse(600 - p, 400 - p, 600 + p, 400 + p);
			break;
		case 2:					// 그래프로 표현
			memDc.SelectObject(&brush2);
			if (p_data != NULL) {
				memDc.Rectangle(505 - _size / 5, 400, 505 + _size / 5, 320 + _pos.y / 5);
				memDc.Rectangle(525 - _size / 4, 400, 525 + _size / 4, 300 + _pos.y / 4);
				memDc.Rectangle(550 - _size / 2, 400, 550 + _size / 2, 200 + _pos.y / 2);
				memDc.Rectangle(_pos.x - _size, 400, _pos.x + _size, _pos.y);
				memDc.Rectangle(650 - _size / 2, 400, 650 + _size / 2, 200 + _pos.y / 2);
				memDc.Rectangle(675 - _size / 4, 400, 675 + _size / 4, 300 + _pos.y / 4);
				memDc.Rectangle(695 - _size / 5, 400, 695 + _size / 5, 320 + _pos.y / 5);
			}
			break;
		case 3:					// 파형으로 표현
			memDc.SelectObject(&graph_pen);
			memDc.MoveTo(0, 400);
			memDc.LineTo(426, 400);
			if (p_data != NULL) {
				memDc.LineTo(438, 480 - _pos.y / 5);
				memDc.LineTo(450, 400);
				memDc.LineTo(465, 300 + _pos.y / 4);
				memDc.LineTo(480, 400);
				memDc.LineTo(510, 600 - _pos.y / 2);
				memDc.LineTo(540, 400);
				memDc.LineTo(_pos.x, _pos.y);
				memDc.LineTo(660, 400);
				memDc.LineTo(690, 600 - _pos.y / 2);
				memDc.LineTo(720, 400);
				memDc.LineTo(735, 300 + _pos.y / 4);
				memDc.LineTo(750, 400);
				memDc.LineTo(762, 480 - _pos.y / 5);
			}
			memDc.LineTo(774, 400);
			memDc.LineTo(rect.Width(), 400);
			memDc.SelectObject(p_old_pen);
			graph_pen.DeleteObject();
			break;
	}

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDc, 0, 0, SRCCOPY);
	// 그리기 메시지에 대해서는 CWnd::OnPaint()를 호출하지 마십시오.
}



void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//스페이스바를 눌렀을 때
	if (nChar == VK_SPACE) {
		if (m_hWaveIn == NULL) {
			// 재생 버퍼의 기준 위치를 0으로 설정
			m_play_index = 0;
			for (int i = 0; i < MAX_NUMBER; i++) {
				// 녹음 버퍼용 메모리
				m_waveInHdr[i] = (WAVEHDR *)::HeapAlloc(GetProcessHeap(), 0, sizeof(WAVEHDR));
				m_waveInHdr[i]->lpData = (char *)::HeapAlloc(GetProcessHeap(), 0, MAX_SIZE);
				m_waveInHdr[i]->dwBufferLength = MAX_SIZE;
				m_waveInHdr[i]->dwFlags = 0;
				m_waveInHdr[i]->dwLoops = 0;

				// 재생 버퍼용 메모리
				m_waveOutHdr[i] = (WAVEHDR *)::HeapAlloc(GetProcessHeap(), 0, sizeof(WAVEHDR));
				m_waveOutHdr[i]->lpData = (char *)::HeapAlloc(GetProcessHeap(), 0, MAX_SIZE);
				m_waveOutHdr[i]->dwBufferLength = MAX_SIZE;
				m_waveOutHdr[i]->dwFlags = 0;
				m_waveOutHdr[i]->dwLoops = 0;
			}

			// 사운드 입력 장치
			waveInOpen(&m_hWaveIn, WAVE_MAPPER, &m_format, (DWORD)m_hWnd, NULL, CALLBACK_WINDOW);
			// 사운드 출력 장치
			waveOutOpen(&m_hWaveOut, NULL, &m_format, (DWORD)m_hWnd, NULL, CALLBACK_WINDOW | WAVE_ALLOWSYNC);

				// 버퍼를 모두 녹음 장치에게 전송
			for (int i = 0; i < MAX_NUMBER; i++) {
				// 녹음할 준비가 되었음을 녹음 장치에게 알림
				waveInPrepareHeader(m_hWaveIn, m_waveInHdr[i], sizeof(WAVEHDR));
				// 녹음을 위해 할당된 버퍼를 녹음 장치에게 전송
				waveInAddBuffer(m_hWaveIn, m_waveInHdr[i], sizeof(WAVEHDR));
			}
			waveInStart(m_hWaveIn);			// 녹음 시작
		}
	}
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

LRESULT CChildView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == MM_WIM_DATA && m_hWaveIn != NULL) {
		// lParam 에 녹음된 데이터 존재
		p_wave_header = (WAVEHDR *)lParam;

		// 녹음 데이터를 OnPaint()에 사용할 변수에 적용
		p_data = (short *)p_wave_header->lpData;
		p_length = p_wave_header->dwBufferLength;

		// 녹음 버퍼에 저장된 데이터를 재생 버퍼에 복사
		memcpy(m_waveOutHdr[m_play_index]->lpData, p_wave_header->lpData, p_wave_header->dwBytesRecorded);
		m_waveOutHdr[m_play_index]->dwBufferLength = p_wave_header->dwBytesRecorded;

		// 재생 버퍼를 출력 장치로 전달
		waveOutPrepareHeader(m_hWaveOut, m_waveOutHdr[m_play_index], sizeof(WAVEHDR));
		// 재생
		waveOutWrite(m_hWaveOut, m_waveOutHdr[m_play_index], sizeof(WAVEHDR));
		// 재생 버퍼의 다음 위치 계산
		m_play_index = (m_play_index + 1) % MAX_NUMBER;
		// 녹음할 준비가 되었음을 녹음 장치에 알림
		waveInPrepareHeader(m_hWaveIn, p_wave_header, sizeof(WAVEHDR));
		// 녹음이 끝나서 사용 가능해진 버퍼를 녹음 장치에게 전송
		waveInAddBuffer(m_hWaveIn, p_wave_header, sizeof(WAVEHDR));
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	// 원 위치 변화
	p = 0;
	if (p_data != NULL) {
		for (int i = 0; i < p_length; i++) {
			//65535은 16비트 정수의 저장범위
			//변화량만을 알고싶기 때문에 abs함수를 이용하여 절대값을 구함
			if (p < abs(1200 * p_data[i] / 65535)) {		
				while (p != abs(1200 * p_data[i] / 65535)) {
					_pos.y = 400 - p;
					p++;
				}
			}
			else if (p > abs(1200 * p_data[i] / 65535)) {
				while (p != abs(1200 * p_data[i] / 65535)) {
					_pos.y = 400 - p;
					p--;
				}
			}
		}
		Invalidate();
	}

	CWnd::OnTimer(nIDEvent);
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 1초당 데이터를 입출력하는데에 필요한 바이트 수 = nAvgBytesPerSec = 32000 
	SetTimer(0, 1/32, NULL);
	return 0;
}

void CChildView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nChar == VK_SPACE)
	{
		if (m_hWaveIn != NULL) {
			// 녹음 중단
			waveInStop(m_hWaveIn);
			// 녹음 중이던 버퍼 비움
			waveInReset(m_hWaveIn);

			// 일시 정지
			waveOutPause(m_hWaveOut);
			// 재생 중이던 버퍼 비움
			waveOutReset(m_hWaveOut);

			// 버퍼 모두 반환
			for (int i = 0; i < MAX_NUMBER; i++) {
				waveInUnprepareHeader(m_hWaveIn, m_waveInHdr[i], sizeof(WAVEHDR));
				waveOutUnprepareHeader(m_hWaveOut, m_waveInHdr[i], sizeof(WAVEHDR));
			}

			// 메모리 제거
			for (int i = 0; i < MAX_NUMBER; i++) {
				::HeapFree(GetProcessHeap(), 0, m_waveInHdr[i]->lpData);
				::HeapFree(GetProcessHeap(), 0, m_waveInHdr[i]);

				::HeapFree(GetProcessHeap(), 0, m_waveOutHdr[i]->lpData);
				::HeapFree(GetProcessHeap(), 0, m_waveOutHdr[i]);
			}

			waveInClose(m_hWaveIn);				// 입력 장치 닫음
			waveOutClose(m_hWaveOut);			// 재생 장치 닫음

			// 초기화
			m_hWaveIn = NULL;
			m_hWaveOut = NULL;
			p_data = NULL;
			_pos.SetPoint(600, 400);
			Invalidate();
		}
	}
	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return true;
	//return CWnd::OnEraseBkgnd(pDC);
}

	//원으로 그리기
void CChildView::OnAudiovisualizerCircle()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	tunnel = 1;
	Invalidate();
}
	//그래프로 그리기
void CChildView::OnAudiovisualizerGraph()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	tunnel = 2;
	Invalidate();
}

	//파형으로 그리기
void CChildView::OnAudiovisualizerWave()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	tunnel = 3;
	Invalidate();
}
