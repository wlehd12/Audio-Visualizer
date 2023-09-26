
// ChildView.h: CChildView 클래스의 인터페이스
//


#pragma once

#include <mmsystem.h>

#define	MAX_NUMBER		3       // 버퍼의 개수
#define	MAX_SIZE  		8000    // 버퍼의 크기, 0.3초간 녹음

// CChildView 창

class CChildView : public CWnd
{
// 생성입니다.
public:
	CChildView();

// 특성입니다.
public:
	// 녹음과 재생에 사용할 공통 설정 정보 
	WAVEFORMATEX m_format;
	// 녹음 장치
	HWAVEIN m_hWaveIn;
	// 녹음에 사용할 데이터 블럭에 대한 정보
	WAVEHDR *m_waveInHdr[MAX_NUMBER];
	// 녹음 파일 헤더
	WAVEHDR *p_wave_header;
	// 재생 장치
	HWAVEOUT m_hWaveOut;
	// 재생에 사용할 데이터 블럭에 대한 정보
	WAVEHDR *m_waveOutHdr[MAX_NUMBER];
	// 재생할 데이터를 저장할 위치
	int m_play_index;
	int _size;
	int p;					//진동수 변화를 정수로 저장			
	int tunnel;				//파형 선택
	short *p_data;			//녹음 데이터값을 저장할 변수
	int p_length;			//녹음 길이		
	CPoint _pos;			//위치
// 작업입니다.
public:

// 재정의입니다.
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
// 구현입니다.
public:
	virtual ~CChildView();

	// 생성된 메시지 맵 함수
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnAudiovisualizerCircle();
	afx_msg void OnAudiovisualizerGraph();
	afx_msg void OnAudiovisualizerWave();
};

