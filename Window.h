#pragma once
#include "Windows.h"
#pragma comment(lib,"d3d11.lib") 
#pragma comment(lib,"d3dcompiler.lib") 

class Window
{
public:
	Window();
	//�������������� ����
	bool init();
	bool broadcast();
	//������� ����
	bool release();
	bool isRun();

	RECT getClientWindowRect();
	void setHWND(HWND hwnd);


	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();

	~Window();
protected:
	HWND m_hwnd;
	bool m_is_run;
};