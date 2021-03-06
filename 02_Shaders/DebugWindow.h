#pragma once
#include <vector>
#include <set>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "FunctionChooser.h"
#include "gCamera.h"

class GPUState; //fwd decl
class Uniforms; //...

struct PerfData
{
	std::string name;
	int alg;
	std::vector<float>	resolutions;
	std::vector<int>	iters;
	float render_time_ms;
	float other_time_ms;
	double error;
};

namespace GUI
{
	class DebugWindow
	{
	public:
		bool pause = false;
		bool show_currentstate = true;
		bool original_size = false;

		struct	//times
		{
			bool optimize = true;
			int num_of_updates = 0;

			struct	//consts
			{
				const float fps60ms = 16.0;
				const float start_value = fps60ms;
				const int N = 512;					//data exists until this point
				double target = fps60ms;			//node framerate is 30 fps
				const float learning_rate = 0.2;	//0-1: smaller value means more recent weights more
			} consts;
			/// last value holds the average of the rest:		update[min(N-1,i)] ...
			std::vector<float> update = std::vector<float>(consts.N, consts.start_value*0.2);
			float render = consts.start_value*0.2;
			float total = consts.start_value;
			float sum_of_updates = 0.0;
			float cpu_measured_time = 16.0;
			float cpu_gpu_time_err = 0.0;
			int learned_update_diff = 0;
		} times;

		int &iternum;
		Uniforms &uniforms;
		gCamera &camera;
		struct //ambient
		{
			int algorithm = 1;
			int start_iternum = 15;
		} ambient;

		struct	//functions
		{
			LinesFunction resolution_multipier = LinesFunction("Iteration -> Resolution multipier",
					{ {0.f, 0.5f}, {1.f, 1.f} });
					//{ {0.f, 0.4f},{1.f,0.7f},{2.f,0.9f}, {3.f, 1.f} });
			LinesFunction spheretrace_stepcount = LinesFunction("Iteration -> Sphere-trace stepcount",
					{ {0.f, 25.f }, {3.f, 25.f}, {7.f, 10.f}, {15.f, 2.f} });
			LinesFunction shadow_stepcount = LinesFunction("Iteration -> Shadow stepcount",
					{ { 0.f, 0.f },{ 4.f, 0.f }, { 10.f, 30.f }, { 40.f, 2.f }});
			LinesFunction user_itercount = LinesFunction("Iteration -> User Itercount",
					{ {0, 8}, {5, 25} });
		} functions;

		struct	//gpu_states
		{
			GPUState *&source_state;
			GPUState *&target_state; //** //*& //const
		} gpu_states;

		struct //view
		{
			float fow_mult = 1.0;				//1.0 = 45�
			GLuint type = 0;					//0 = standard, 1: one screen adjustable, 2: quadrants adjustable
			GLint samplers[4] = {2, 0, 0, 0};	//0 for depth, 1 for normal 2 for color
			/*uniform*/ glm::mat4 color_multipiers[4] = {glm::mat4(1), glm::mat4(1) , glm::mat4(1) , glm::mat4(1) };
			/*uniform*/ glm::vec4 color_offsets[4] = {glm::vec4(0), glm::vec4(0), glm::vec4(0) , glm::vec4(0) };
			struct //deatil
			{
				glm::mat4 channel_map[4] = { glm::mat4(1), glm::mat4(1) , glm::mat4(1) , glm::mat4(1) };
				glm::vec4 multipiers_diag[4] = { glm::vec4(1), glm::vec4(1) , glm::vec4(1) , glm::vec4(1) };
				glm::vec4 offsets[4] = { glm::vec4(0), glm::vec4(0), glm::vec4(0) , glm::vec4(0) };
				int color_mode[4] = {0,2,2,2};
				int channel_id[4] = { 0,0,2,1 };
				float ch_multipiers[4] = {1,0.015f,0.01f,4.0};
				float ch_offsets[4] = { 0,0,0,0 };
			}detail;
		}view;

		struct //stats
		{
			std::vector<glm::vec4> pixeldata;
			glm::dvec4 max, min, avg, sd;
			std::vector<float> histograms[4];
			glm::ivec4 hist_Ns = glm::ivec4(30);
			std::string latex_hist[4];
			std::string latex_table[4];
		}stats;

		DebugWindow(int &iternum, Uniforms &uniforms, GPUState *&source_state, GPUState *&target_state, gCamera &camera);

		void SetSize();
		void Render();
		void Resize(int w, int h);
	public: //private
		bool turn_pause_to_false = false;
		bool turn_pause_to_true = false;
		bool measure_performance = false;
		std::vector<PerfData> perfdata;
		std::string text_runtimes;
		std::vector<float> reference_image;
		struct
		{
			float full_width = 350;	float full_height = 450;
			float win_width = 350;	float win_height = 450;
		} sizes;
		struct
		{
			float scale[2] = { 32.f, 32.f };
			bool b[2] = { false, false };
			bool cnd[2][3] = { {false, true, false},{false, false, false} };
		}stateview;
		bool p_open = true;
		const char* this_winname = "Debug Window";

		int tab_sub_wind_id = 0;
		int radio_visual_gpu_state_show_num = 0;

		void ShowIterationOptions();
		void ShowFunctionOptions();
		void GenPerfStrings();
		void ShowSwichOptions();
		void ShowGPUState(int i, const GPUState & gpu, float m);
		void ColorMappingSettings(int i);
		void UpdateHistogramCH(int channel);
		void DispHistDataCH(int channel);
		void ShowVisualisationOptions();
	};
}

//divisable by 2
inline int test_iterfun(int i)
{
	return 2 << i;
}