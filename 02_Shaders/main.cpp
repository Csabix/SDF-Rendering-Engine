#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>
#include <sstream>
#include "imgui\imgui.h"
#include "imgui\imgui_impl_sdl_gl3.h"
#include "MyApp.h"
#include "gTimer.h"

int main( int argc, char* args[] )
{
	//atexit([]() { system("pause"); });

	glassert(SDL_Init(SDL_INIT_VIDEO) != -1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,         32);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,            8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,          8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,           8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,          8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,		1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,          24);
	
	SDL_Window *win = SDL_CreateWindow( "Hello SDL&OpenGL!", 100, 100, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);			// megjelenítési tulajdonságok
	glassert(win != nullptr);

	ImGui_ImplSdlGL3_Init(win);

	SDL_GLContext	context	= SDL_GL_CreateContext(win);
	glassert(context != nullptr);

	SDL_GL_SetSwapInterval(-1);
	
	glassert(glewInit() == GLEW_OK);

	int glVersion[2] = {-1, -1}; 
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]); glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]); 
	std::cout << "Running OpenGL " << glVersion[0] << "." << glVersion[1] << std::endl;
	glassert(glVersion[0] != -1 || glVersion[1] != -1);

	std::stringstream window_title;
	window_title << "SDL Renderer (OpenGL " << glVersion[0] << "." << glVersion[1] << ')';
	SDL_SetWindowTitle(win, window_title.str().c_str());
	
	//ImGui::GetIO().Fonts->AddFontDefault();
	{// app
		for(int i = 0; i < 8; ++i)
		{
			ImGui::GetIO().Fonts->AddFontFromFileTTF("Consolas.ttf", i*2+11);
		}
		CMyApp app;						glcheck("Main Error");

		// PERFORMANCE TEST DATA
		for (int i = 0; i < 10; ++i)
			app.debug.perfdata.push_back({ "1 res, basic", Uniforms::ALGORITHM::ST_1 , {1.f}, {test_iterfun(i)} });
		for (int i = 0; i < 10; ++i)
			app.debug.perfdata.push_back({ "1 res, relaxed", Uniforms::ALGORITHM::ST_2 , {1.f}, {test_iterfun(i)} });
		for (int i = 0; i < 10; ++i)
			app.debug.perfdata.push_back({ "1 res, enhanced", Uniforms::ALGORITHM::ST_3 , {1.f}, {test_iterfun(i)} });
		for (int i = 0; i < 10; ++i)
			app.debug.perfdata.push_back({ "2 res, basic", Uniforms::ALGORITHM::ST_1 , {0.5f,1.f}, {test_iterfun(i)/2,test_iterfun(i)/2} });
		for (int i = 0; i < 10; ++i)
			app.debug.perfdata.push_back({ "2 res, relaxed", Uniforms::ALGORITHM::ST_2 , {0.5f,1.f}, {test_iterfun(i)/2,test_iterfun(i)/2} });
		for (int i = 0; i < 10; ++i)
			app.debug.perfdata.push_back({ "2 res, enhanced", Uniforms::ALGORITHM::ST_3 , {0.5f,1.f}, {test_iterfun(i)/2,test_iterfun(i)/2} });
		for (int i = 0; i < 10; ++i)
			app.debug.perfdata.push_back({ "3 res, basic", Uniforms::ALGORITHM::ST_1 , {0.33f, 0.66f, 1.f}, {test_iterfun(i)/3,test_iterfun(i)/3, test_iterfun(i) - 2*(test_iterfun(i)/3)} });
		for (int i = 0; i < 10; ++i)
			app.debug.perfdata.push_back({ "3 res, relaxed", Uniforms::ALGORITHM::ST_2 , {0.33f, 0.66f, 1.f},{test_iterfun(i)/3,test_iterfun(i)/3, test_iterfun(i) - 2*(test_iterfun(i)/3)} });
		for (int i = 0; i < 10; ++i)
			app.debug.perfdata.push_back({ "3 res, enhanced", Uniforms::ALGORITHM::ST_3 , {0.33f, 0.66f, 1.f}, {test_iterfun(i)/3,test_iterfun(i)/3, test_iterfun(i) - 2*(test_iterfun(i)/3)} });
		for (int i = 0; i < 10; ++i)
			app.debug.perfdata.push_back({ "4 res, basic", Uniforms::ALGORITHM::ST_1 , {0.25f,0.5f,0.75f, 1.f}, {test_iterfun(i)/4,test_iterfun(i)/4,test_iterfun(i)/4,test_iterfun(i)/4}, });
		for (int i = 0; i < 10; ++i)
			app.debug.perfdata.push_back({ "4 res, relaxed", Uniforms::ALGORITHM::ST_2 , {0.25f,0.5f,0.75f, 1.f}, {test_iterfun(i)/4,test_iterfun(i)/4,test_iterfun(i)/4,test_iterfun(i)/4} });
		for (int i = 0; i < 10; ++i)
			app.debug.perfdata.push_back({ "4 res, enhanced", Uniforms::ALGORITHM::ST_3 , {0.25f,0.5f,0.75f, 1.f}, {test_iterfun(i)/4,test_iterfun(i)/4,test_iterfun(i)/4,test_iterfun(i)/4} });
		for (int i = 0; i < 10; ++i)
			app.debug.perfdata.push_back({ "4 heu, basic", Uniforms::ALGORITHM::ST_1 , {0.40f,0.65f,0.85f, 1.f}, {test_iterfun(i)/4,test_iterfun(i)/4,test_iterfun(i)/4,test_iterfun(i)/4}, });
		for (int i = 0; i < 10; ++i)
			app.debug.perfdata.push_back({ "4 heu, relaxed", Uniforms::ALGORITHM::ST_2 , {0.40f,0.65f,0.85f, 1.f}, {test_iterfun(i)/4,test_iterfun(i)/4,test_iterfun(i)/4,test_iterfun(i)/4} });
		for (int i = 0; i < 10; ++i)
			app.debug.perfdata.push_back({ "4 heu, enhanced", Uniforms::ALGORITHM::ST_3 , {0.40f,0.65f,0.85f, 1.f}, {test_iterfun(i)/4,test_iterfun(i)/4,test_iterfun(i)/4,test_iterfun(i)/4} });
		// =====================

		glassert(app.Init());			glcheck("Init Error");
		//Test Render (the very first frame)
		ImGui_ImplSdlGL3_NewFrame(win);	glcheck("ImGui Error");
		app.Update();					glcheck("Update Error");
		app.Render();					glcheck("Render Error");
		ImGui::Render();				glcheck("ImGui Error");
		SDL_GL_SwapWindow(win);			glcheck("SDL Swap Error");
		app.PresentationUpdate();

		GPU_Timer gpu_first_timer;			glcheck("GPU_Timer Error");
		GPU_Timer gpu_update_timer;			glcheck("GPU_Timer Error");
		GPU_Timer gpu_render_timer;			glcheck("GPU_Timer Error");

		CPU_Timer cpu_timer; cpu_timer.Start();	size_t frame_cnt = 0;

		bool quit = false, pause = false, autopause = false;
		while(!quit)
		{
			SDL_Event ev;
			while(SDL_PollEvent(&ev))
			{
				ImGui_ImplSdlGL3_ProcessEvent(&ev);
				bool is_mouse_captured = ImGui::GetIO().WantCaptureMouse, is_keyboard_captured = ImGui::GetIO().WantCaptureKeyboard;
				switch(ev.type)
				{
				case SDL_QUIT: quit = true; break;
				case SDL_KEYDOWN:
					if(!is_keyboard_captured)
					{
						app.KeyboardDown(ev.key); break;
						quit = ev.key.keysym.sym == SDLK_ESCAPE;
					}
					break;
				case SDL_KEYUP:	if(!is_keyboard_captured)	app.KeyboardUp(ev.key);	break;
				case SDL_MOUSEBUTTONDOWN:	if(!is_mouse_captured)	app.MouseDown(ev.button);	break;
				case SDL_MOUSEBUTTONUP:	if(!is_mouse_captured)	app.MouseUp(ev.button);	break;
				case SDL_MOUSEWHEEL:	if(!is_mouse_captured)	app.MouseWheel(ev.wheel);	break;
				case SDL_MOUSEMOTION:	if(!is_mouse_captured)	app.MouseMove(ev.motion);	break;
				case SDL_WINDOWEVENT:	if(ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
											app.Resize(ev.window.data1, ev.window.data2);	break;
				}
			}
			ImGui_ImplSdlGL3_NewFrame(win);

			auto &times = app.debug.times;
			if (app.debug.measure_performance)
			{
				app.measure_error();
				app.warmup_run();				//warmup
				app.measure_performance();		//start measurement
				app.debug.GenPerfStrings();
				app.debug.measure_performance = false;
			}
			else if(!app.debug.pause)
			{
				gpu_first_timer.Start();
				app.Update();//UPDATE #1
				
				gpu_first_timer.Stop();			
				gpu_first_timer.Swap();
				float first_frame_time = gpu_first_timer.QuerryMillisecs();

				times.sum_of_updates = first_frame_time;
				auto &u = times.update[std::min(times.consts.N - 1, app.iternum - 1)];
				u = glm::mix<float>(first_frame_time, u, times.consts.learning_rate);			//moving avg
				times.num_of_updates = 1;

				if(times.optimize)
				{			//the very first iterations has to keep exactly the render times, later it is OK to be bit late, even 2x slower
					int to_update = 0, const start_iternum = app.iternum;
					for (float sum = first_frame_time + times.render;
						 sum < times.consts.target;
						 sum += times.update[std::min(times.consts.N - 1, to_update + start_iternum)])
						++to_update;
					to_update += times.learned_update_diff;

					gpu_update_timer.Start();
					for (int i=0; i < to_update ; ++i)
						app.Update();

					gpu_update_timer.Stop();
					gpu_update_timer.Swap();
					float sum_time = gpu_update_timer.QuerryMillisecs();

					float avg_updated = sum_time / (float) to_update;
					for (int i = start_iternum; i < start_iternum + to_update; ++i)
					{
						auto &u = times.update[std::min(times.consts.N - 1, i)];
						u = glm::mix<float>(avg_updated, u, times.consts.learning_rate/3.f);
					}
					times.sum_of_updates += sum_time;
					times.num_of_updates += to_update;

				}
			}//end of event processing

			gpu_render_timer.Start();
			app.Render();
			//ImGui::ShowTestWindow();
			ImGui::Render();

			gpu_render_timer.Stop();
			gpu_render_timer.Swap();

			SDL_GL_SwapWindow(win);

			times.render = glm::mix<float>((float)gpu_render_timer.QuerryMillisecs(), times.render, times.consts.learning_rate);
			times.total = times.sum_of_updates + times.render;

			const size_t N = 25;
			if(++frame_cnt % N == 0)
			{
				cpu_timer.Finish();
				float avg_cpu_measured_time = cpu_timer.GetMilliseconds() / (double) N;
				cpu_timer.Start();
				times.cpu_measured_time = glm::mix<float>(avg_cpu_measured_time, times.cpu_measured_time, times.consts.learning_rate / sqrtf(N));

				if (times.cpu_measured_time > times.consts.target*1.1f)
				{
					times.learned_update_diff -= 1;
				}
				if (times.total < times.consts.target*0.5f)
				{
					times.learned_update_diff += 1;
				}
				times.learned_update_diff = glm::clamp<int>(times.learned_update_diff, -2, 0);
			}

			times.cpu_gpu_time_err = glm::mix<float>(times.cpu_measured_time - times.total, times.cpu_gpu_time_err, times.consts.learning_rate*0);
		}	//end of rendering loop
		glcheck("Before Cleanup");
		app.Clean();
	}	//app, gpu_timer deletes itself
	glcheck("Cleanup");
	ImGui_ImplSdlGL3_Shutdown();
	glcheck("Cleanup");
	SDL_GL_DeleteContext(context);
	//glcheck("Cleanup");
	SDL_DestroyWindow( win );
	//glcheck("Cleanup");
	SDL_Quit();
	//glcheck("After Cleanup");
	return 0;
}