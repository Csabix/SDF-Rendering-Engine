#pragma once
#include <glm/glm.hpp>
#include <array>
#include <vector>

namespace FUN
{
	//inline float to01(float start_time, float arg_min, float arg_max){return glm::clamp<float>((start_time - arg_min) / (arg_max - arg_min), 0, 1);}
	//inline float from01(float start_time, const float ret_min, const float ret_max){return ret_min + glm::clamp<float>(start_time, 0, 1) * (ret_max - ret_min);}
	std::array<float, 4> hermite3_coeffs(glm::vec2 p0, glm::vec2 p1, float d0, float d1);
	inline float eval_poly3(const std::array<float, 4> &p, float t)
	{
		return ((p[3]*t + p[2])*t + p[1])*t + p[0]; //3 mad op
	}
	inline float eval_lines(std::vector<glm::vec2> points, float t)
	{
		if (points.size() <= 0) return 0;
		if (points.front().x >= t) return points.front().y;
		if (points.back().x <= t) return points.back().y;
		int i = 0;
		for (; i < points.size() && points[i + 1].x < t; ++i)
			; //slow, but general
		float ratio = (t - points[i].x) / (points[i + 1].x - points[i].x);
		return glm::mix<float>(points[i].y, points[i + 1].y, ratio);
	}
}

namespace GUI
{
	class FunctionChooser
	{
		glm::vec2 p0 = glm::vec2(0), p1 = glm::vec2(1), d_in = glm::vec2(1), d_out = glm::vec2(1);
		std::array<float, 4> hermite3_coeffs = FUN::hermite3_coeffs(glm::vec2(0), glm::vec2(1), 0,0);
		float constant_value = 1.0;
		bool change = true, integers_only = true;
		const char* header_name;
		int current_funtion_type = 0;
		bool relative_derivatives = true;
		std::vector<glm::vec2> points = { {0,0}, {0,1} };

		inline float function(float t) const
		{
			switch(current_funtion_type)
			{
			case 0:	return FUN::eval_poly3(hermite3_coeffs, t); break;
			case 1: return constant_value; break;
			case 2: return FUN::eval_lines(points, t); break;
			default: return constant_value; break;
			}
		}
		void ConstantFunctionChooser();
		void HermiteFunctionChooser();
		void LinesFunctionChooser();
	public:
		FunctionChooser(const char *header_name, glm::vec2 p0 = glm::vec2(0), glm::vec2 p1 = glm::vec2(1),
						float d0 = 1, float d1 = 1) :
			header_name(header_name), p0(p0), p1(p1), d_in(d0, d1), current_funtion_type(0)
		{
			d_out = (p1.y - p0.y) / (p1.x - p0.x) * glm::vec2(d0, d1);
			hermite3_coeffs = FUN::hermite3_coeffs(p0, p1, d_out.x, d_out.y);
		}
		FunctionChooser(const char *header_name, float constant_value) :
			header_name(header_name), constant_value(constant_value), current_funtion_type(1)
		{}

		inline float operator ()(float t) const { return function(glm::clamp<float>(t, p0.x, p1.x)); }

		void ChangeFunction(); //!< For debug view
	};


	class LinesFunction
	{
	public:
		LinesFunction(const char*header_name , const std::vector<glm::vec2> &points) : header_name(header_name), points(points) {}
		inline float operator()(float t) const
		{
			if (points.size() <= 0) return 0;
			if (points.front().x >= t) return points.front().y;
			if (points.back().x <= t) return points.back().y;
			int i = 0;
			for (; i < points.size() && points[i + 1].x < t; ++i)
				; //slow, but general
			float ratio = (t - points[i].x) / (points[i + 1].x - points[i].x);
			return glm::mix<float>(points[i].y, points[i + 1].y, ratio);
		}
		void ChangeFunction();
	private:
		std::vector<glm::vec2> points = { {0,0}, {0,1} };
		const char* header_name;
	};
}