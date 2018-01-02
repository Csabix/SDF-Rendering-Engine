#include "DebugWindow.h"
#include "imgui\imgui.h"
#include "imgui\auto.h"
#include "FileIO.h"
//#include "Uniforms.h"

void GUI::DebugWindow::SetSize()
{
	sizes.win_width = sqrtf((float)sizes.full_width) * 9.5f + 100.f;
	sizes.win_height = sizes.full_height - 50;
	ImGui::SetWindowSize(this_winname, ImVec2(sizes.win_width + 30, sizes.full_height));
	ImGui::SetWindowPos(this_winname, ImVec2(0, 0));
}

GUI::DebugWindow::DebugWindow(int &iternum, Uniforms & uniforms, GPUState *& source_state, GPUState *& target_state, gCamera &camera)
	:iternum(iternum), uniforms(uniforms), gpu_states({ source_state, target_state }), camera(camera)
{}

void GUI::DebugWindow::Render()
{
	if(p_open) SetSize();
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
	if(!ImGui::Begin(this_winname, &p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
	{
		ImGui::End();	ImGui::PopFont(); return;
	}
	ImGui::PopFont();	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
	ShowIterationOptions();
	ImGui::Separator();

	const int N = 3;
	const char * tmp_selectables[N] = {"Functions", "Swiches", "Data Visualisation"};
	ImVec2 tab_button_size = ImVec2(ImGui::GetContentRegionAvailWidth() / 3 - 5, 15);

	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
	for(int i = 0; i < N; ++i)
	{
		if(ImGui::Selectable(tmp_selectables[i], i == tab_sub_wind_id, 0, tab_button_size))
			tab_sub_wind_id = i;
		if(i != N-1) ImGui::SameLine();
	}ImGui::PopFont();

	switch(tab_sub_wind_id)
	{
	case 0:	ShowFunctionOptions();		break;
	case 1:	ShowSwichOptions();			break;
	case 2:	ShowVisualisationOptions();	break;
	default:							break;
	}
	ImGui::End();
	ImGui::PopFont();
}

void GUI::DebugWindow::Resize(int w, int h)
{
	sizes.full_width = (float)w; sizes.full_height = (float)h;
	SetSize();
}

void GUI::DebugWindow::ShowFunctionOptions()
{
	if (ImGui::Button("Measure times"))	this->measure_performance = true;
	static std::string ref_image_path = "C:/Users/Csabix/Documents/Grafika/RawData/csg.bin";
	ImGui::Auto(ref_image_path, "Ref Img Path");
	if (ImGui::Button("Load Ref image"))
	{
		FileIO::LoadUnformattedData(ref_image_path.data(), reference_image);
		assert(reference_image.size() % 4 == 0);
		for (int i = 0; i < reference_image.size() / 4; ++i)
		{
			reference_image[i] = reference_image[4 * i]; //first channel only
		}
		reference_image.resize(reference_image.size() / 4);
		reference_image.shrink_to_fit();
	}
	ImGui::Auto(this->perfdata, "PerfTimes");

	ImGui::InputTextMultiline("Runtimes", this->text_runtimes);

	ImGui::Separator();
	
	functions.resolution_multipier.ChangeFunction();
	functions.spheretrace_stepcount.ChangeFunction();
	functions.shadow_stepcount.ChangeFunction();
	functions.user_itercount.ChangeFunction();
}

void GUI::DebugWindow::GenPerfStrings()
{
	const int test_cnt = 15;
	text_runtimes = "Rendertimes";
	for (int i = 0; i < 10; ++i)
		text_runtimes += "\t" + std::to_string(test_iterfun(i));
	for (int j = 0; j < test_cnt; ++j)
	{
		text_runtimes += '\n';
		text_runtimes += this->perfdata[j*10].name;
		for (int i = 0; i < 10; ++i)
			text_runtimes += "\t" + std::to_string(this->perfdata[j*10 + i].render_time_ms);
	}

	text_runtimes += "\n\nErrors";
	for (int i = 0; i < 10; ++i)
		text_runtimes += "\t" + std::to_string(test_iterfun(i));
	for (int j = 0; j < test_cnt; ++j)
	{
		text_runtimes += '\n';
		text_runtimes += this->perfdata[j*10].name;
		for (int i = 0; i < 10; ++i)
			text_runtimes += "\t" + std::to_string(this->perfdata[j*10 + i].error);
	}

	/*text_runtimes += "\n\nOther times";
	for (int i = 0; i < 10; ++i)
		text_runtimes += "\t" + std::to_string(test_iterfun(i));
	for (int j = 0; j < test_cnt; ++j)
	{
		text_runtimes += '\n';
		text_runtimes += this->perfdata[j*10].name;
		for (int i = 0; i < 10; ++i)
			text_runtimes += "\t" + std::to_string(this->perfdata[j*10 + i].other_time_ms);
	}*/
}