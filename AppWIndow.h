#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputListener.h"
#include "Matrix4x4.h"

#include <vector>
#include <thread>

class AppWindow : public Window, public InputListener
{
public:
	AppWindow();

	void update();

	~AppWindow();

	

	// Inherited via Window
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
	virtual void onFocus() override;
	virtual void onKillFocus() override;

	// Inherited via InputListener
	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;
	virtual void onMouseMove(const Point& mouse_pos) override;

	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;
	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;

	//��������������� ��� ��������� �������������
	void processTriangle(const std::vector<unsigned int>& indexes);


private:
	SwapChain * m_swap_chain;
	VertexBuffer* m_vb;
	VertexShader* m_vs;
	PixelShader* m_ps;
	ConstantBuffer* m_cb;

	IndexBuffer* m_ib;
	IndexBuffer* m_ib2;
	IndexBuffer* m_ib3;
	IndexBuffer* m_ib4;
	IndexBuffer* m_ib5;
	IndexBuffer* m_ib6;
	IndexBuffer* m_ib7;
	IndexBuffer* m_ib8;
	IndexBuffer* m_ib9;
	IndexBuffer* m_ib10;
	IndexBuffer* m_ib11;
	IndexBuffer* m_ib12;

	long m_old_delta;
	long m_new_delta;
	float m_delta_time;

	float m_delta_pos;
	float m_delta_scale;
	float m_delta_rot;

	float m_rot_x = 0.0f;
	float m_rot_y = 0.0f;

	float m_r = 0.0f;
	float m_g = 0.3f;
	float m_b = 0.6f;
	float m_a = 0.0f;


	float m_scale_cube = 1;
	float m_forward = 0.0f;
	float m_rightward = 0.0f;
	Matrix4x4 m_world_cam;

};