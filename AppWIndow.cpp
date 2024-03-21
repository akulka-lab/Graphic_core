#include "AppWindow.h"
#include "Windows.h"
#include "Vector3D.h"
#include "Vector2D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "Triangles.cpp"

#include <thread>
#include <vector>
#include <mutex>

std::mutex mtx;

struct vertex
{
	Vector3D position;
	Vector3D color;
	Vector3D color1;
};


__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	unsigned int m_time;
};


AppWindow::AppWindow()
{
}


void AppWindow::update()
{
	constant cc;
	cc.m_time = ::GetTickCount();

	m_delta_pos += m_delta_time / 10.0f;
	if (m_delta_pos > 1.0f)
		m_delta_pos = 0;


	Matrix4x4 temp;

	m_delta_scale += m_delta_time / 0.55f;


	cc.m_world.setIdentity();

	Matrix4x4 world_cam;
	world_cam.setIdentity();

	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	world_cam *= temp;


	Vector3D new_pos = m_world_cam.getTranslation() + world_cam.getZDirection()*(m_forward*0.1f);

	new_pos = new_pos + world_cam.getXDirection()*(m_rightward*0.1f);

	world_cam.setTranslation(new_pos);

	m_world_cam = world_cam;


	world_cam.inverse();

	cc.m_view = world_cam;

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);


	cc.m_proj.setPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 100.0f);


	m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);
}


AppWindow::~AppWindow()
{
}

void AppWindow::processTriangle(const std::vector<unsigned int>& indexes, int k) {
	
	/*index_list_2.push_back(index1);
	index_list_2.push_back(index2);
	index_list_2.push_back(index3);

	UINT size_index_list = index_list_2.size();*/

	std::vector<unsigned int> index_data = indexes; //  опируем данные из вектора во временный массив
	m_ib[k]->load(index_data.data(), index_data.size());
}

void AppWindow::onCreate()
{
	Window::onCreate();

	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(false);

	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	m_world_cam.setTranslation(Vector3D(0, 0, -2));

	vertex vertex_list[] =
	{
		//X - Y - Z
		//передн€€ часть
		{Vector3D(-0.5f,-0.5f,-0.5f),    Vector3D(1,1,1),  Vector3D(0.2f,0,0) },
		{Vector3D(-0.5f,0.5f,-0.5f),    Vector3D(1,0,0), Vector3D(0.2f,0.2f,0) },
		{ Vector3D(0.5f,0.5f,-0.5f),   Vector3D(1,0,0),  Vector3D(0.2f,0.2f,0) },
		{ Vector3D(0.5f,-0.5f,-0.5f),     Vector3D(1,0,0), Vector3D(0.2f,0,0) },

		//задн€€ часть
		{ Vector3D(0.5f,-0.5f,0.5f),     Vector3D(1,0,0),  Vector3D(0,0.2f,0) },
		{ Vector3D(0.5f,0.5f,0.5f),    Vector3D(1,0,0),  Vector3D(0,0.2f,0.2f) },
		{ Vector3D(-0.5f,0.5f,0.5f),   Vector3D(1,0,0),  Vector3D(0,0.2f,0.2f) },
		{ Vector3D(-0.5f,-0.5f,0.5f),    Vector3D(0,0,0),  Vector3D(0,0.2f,0) }

	};

	/*vertex vertex_list_line[] =
	{
		//X - Y - Z
		//передн€€ часть
		{Vector2D(-0.5f,-0.5f,-0.5f),    Vector2D(1,1,1),  Vector2D(0.2f,0,0) },
		{Vector3D(-0.5f,0.5f,-0.5f),    Vector3D(1,0,0), Vector3D(0.2f,0.2f,0) },
		{ Vector3D(0.5f,0.5f,-0.5f),   Vector3D(1,0,0),  Vector3D(0.2f,0.2f,0) },
		{ Vector3D(0.5f,-0.5f,-0.5f),     Vector3D(1,0,0), Vector3D(0.2f,0,0) },

		//задн€€ часть
		{ Vector3D(0.5f,-0.5f,0.5f),     Vector3D(1,0,0),  Vector3D(0,0.2f,0) },
		{ Vector3D(0.5f,0.5f,0.5f),    Vector3D(1,0,0),  Vector3D(0,0.2f,0.2f) },
		{ Vector3D(-0.5f,0.5f,0.5f),   Vector3D(1,0,0),  Vector3D(0,0.2f,0.2f) },
		{ Vector3D(-0.5f,-0.5f,0.5f),    Vector3D(0,0,0),  Vector3D(0,0.2f,0) }

	};*/


	m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(vertex_list);


	std::vector<unsigned int> index_list_2;

	unsigned int index_list[] =
	{
		//FRONT SIDE
		0,1,2,  //FIRST TRIANGLE
		2,3,0,  //SECOND TRIANGLE
		//BACK SIDE
		4,5,6,
		6,7,4,
		//TOP SIDE
		1,6,5,
		5,2,1,
		//BOTTOM SIDE
		7,0,3,
		3,4,7,
		//RIGHT SIDE
		3,2,5,
		5,4,3,
		//LEFT SIDE
		7,6,1,
		1,0,7
	};


	std::vector<std::thread> threads;
	std::mutex m;
	
	std::vector<unsigned int> indexes;
	int k = 0;
	

	for (int i = 0; i < ARRAYSIZE(index_list); i += 3) {
		unsigned int index1 = index_list[i];
		unsigned int index2 = index_list[i + 1];
		unsigned int index3 = index_list[i + 2];
		//threads.push_back(std::thread(&AppWindow::processTriangle, this, index1, index2, index3));

		m_ib[k] = GraphicsEngine::get()->createIndexBuffer();
		std::vector<unsigned int> indexes = { index1, index2, index3 };

		threads.push_back(std::thread([&]() {
			// Ѕлокируем мьютекс перед началом критической секции
			std::lock_guard<std::mutex> lock(mtx);

			// ѕроцесс отрисовки треугольника
			processTriangle(indexes, k);
			}));
		k++;
	}

		/*std::vector<unsigned int> indexes = { index1, index2, index3 };
		threads.push_back(std::thread(&AppWindow::processTriangle, this, indexes));*/

	// ƒаем команду всем потокам начать одновременную отрисовку
	{
	std::lock_guard<std::mutex> lock(m);
	}


	for (auto& thread : threads) {
		thread.join();
	}

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vb->load(vertex_list, sizeof(vertex), size_list, shader_byte_code, size_shader);

	GraphicsEngine::get()->releaseCompiledShader();

	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	constant cc;
	cc.m_time = 0;

	m_cb = GraphicsEngine::get()->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));

}



void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputSystem::get()->update();

	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, m_r, m_g, m_b, m_a);
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);


	update();


	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	//базовый шейдер
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);

	//индексы треугольников
	for (int i = 0; i < 12; i++)
	{
		GraphicsEngine::get()->getImmediateDeviceContext()->setIndexBuffer(m_ib[i]);
	}
	
	// вывод треугольников
	for (int i = 0; i < 12; i++)
	{
		GraphicsEngine::get()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib[i]->getSizeIndexList(), 0, 0);
	}

	m_swap_chain->present(true);


	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount();

	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_vb->release();
	for (int i = 0; i < 12; i++)
	{
		m_ib[i]->release();
	}
	m_cb->release();
	m_swap_chain->release();
	m_vs->release();
	m_ps->release();
	GraphicsEngine::get()->release();
}

void AppWindow::onFocus()
{
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

void AppWindow::onKeyDown(int key)
{
	if (key == 'W')
	{
		m_forward = 1.0f;
	}
	else if (key == 'S')
	{
		m_forward = -1.0f;
	}
	else if (key == 'A')
	{
		m_rightward = -1.0f;
	}
	else if (key == 'D')
	{
		m_rightward = 1.0f;
	}
}

void AppWindow::onKeyUp(int key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;
}

void AppWindow::onMouseMove(const Point & mouse_pos)
{
	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);


	m_rot_x += (mouse_pos.m_y - (height / 2.0f))*m_delta_time*0.1f;
	m_rot_y += (mouse_pos.m_x - (width / 2.0f))*m_delta_time*0.1f;

	InputSystem::get()->setCursorPosition(Point((int)(width / 2.0f), (int)(height / 2.0f)));
}

void AppWindow::onLeftMouseDown(const Point & mouse_pos)
{
	m_scale_cube = 0.5f;
}

void AppWindow::onLeftMouseUp(const Point & mouse_pos)
{
	m_scale_cube = 1.0f;
}

void AppWindow::onRightMouseDown(const Point & mouse_pos)
{
	m_scale_cube = 2.0f;
}

void AppWindow::onRightMouseUp(const Point & mouse_pos)
{
	m_scale_cube = 1.0f;
}