#include "std_include.hpp"

#define	PITCH	0
#define	YAW		1
#define	ROLL	2

namespace utils
{
	bool starts_with(std::string haystack, std::string needle)
	{
		return (haystack.size() >= needle.size() && !strncmp(needle.data(), haystack.data(), needle.size()));
	}

	bool string_contains(const std::string& s1, const std::string s2)
	{
		const auto it = s1.find(s2);
		if (it != std::string::npos)
		{
			return true;
		}

		return false;
	}

	bool erase_substring(std::string& base, const std::string& replace)
	{
		if (const auto it = base.find(replace);
			it != std::string::npos)
		{
			base.erase(it, replace.size());
			return true;
		}

		return false;
	}

	std::string str_to_lower(std::string input)
	{
		std::ranges::transform(input.begin(), input.end(), input.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
		return input;
	}

	std::string convert_wstring(const std::wstring& wstr)
	{
		std::string result;
		result.reserve(wstr.size());

		for (const auto& chr : wstr)
		{
			result.push_back(static_cast<char>(chr));
		}

		return result;
	}


	// ---------------------------------
	// math

	game::vec3_t vec3_origin = { 0,0,0 };
	game::vec3_t axisDefault[3] = { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } };

	float rad_to_deg(const float radians)
	{
		return radians * (180.0f / M_PI);
	}

	float deg_to_rad(const float degrees)
	{
		return degrees * M_PI / 180.0f;
	}

	void vectosignedangles(const float* in, float* out)
	{
		auto signed_vec = [](const float v, const float less_zero, const float other) -> float
		{
			float val = less_zero;

			if (v < 0.0f)
			{
				val = other;
			}

			return val;
		};

		float yaw = 0.0f;
		float pitch = 0.0f;

		if (in[1] == 0.0f && in[0] == 0.0f)
		{
			pitch = signed_vec(-in[2], 90.0f, 270.0f);
		}
		else
		{
			yaw = atan2(in[1], in[0]) * -180.0f / M_PI;
			yaw = signed_vec(yaw, 0.0f, 360.0f) + yaw;

			pitch = atan2(in[2], sqrt(in[0] * in[0] + in[1] * in[1])) * -180.0f / M_PI;
			pitch = signed_vec(pitch, 0.0f, 360.0f) + pitch;
		}

		out[0] = pitch;
		out[1] = yaw;
		out[2] = 0.0f;
	}

	void vectoangles(const game::vec3_t value1, game::vec3_t angles)
	{
		float forward;
		float yaw, pitch;

		if (value1[1] == 0.0f && value1[0] == 0.0f)
		{
			yaw = 0.0f;
			if (value1[2] > 0.0f)
			{
				pitch = 90.0f;
			}
			else 
			{
				pitch = 270.0f;
			}
		}
		else 
		{
			if (value1[0]) 
			{
				yaw = (atan2(value1[1], value1[0]) * 180.0f / M_PI);
			}
			else if (value1[1] > 0.0f)
			{
				yaw = 90.0f;
			}
			else 
			{
				yaw = 270.0f;
			}
			if (yaw < 0.0f)
			{
				yaw += 360.0f;
			}

			forward = sqrt(value1[0] * value1[0] + value1[1] * value1[1]);
			pitch = (atan2(value1[2], forward) * 180.0f / M_PI);
			if (pitch < 0.0f)
			{
				pitch += 360.0f;
			}
		}

		angles[PITCH] = -pitch;
		angles[YAW] = yaw;
		angles[ROLL] = 0.0f;
	}

	float vectosignedpitch(float* vec)
	{
		auto signed_vec = [](const float v, const float less_zero, const float other) -> float
		{
			float val = less_zero;

			if (v < 0.0f)
			{
				val = other;
			}

			return val;
		};

		float pitch = 0.0f;

		if (vec[1] == 0.0f && vec[0] == 0.0f)
		{
			pitch = signed_vec(-vec[2], 90.0f, -90.0f);
		}
		else
		{
			pitch = atan2(vec[2], sqrt(vec[0] * vec[0] + vec[1] * vec[1]));
			pitch = pitch * -180.0f / M_PI;
		}

		return pitch;
	}

	void vec3_copy(const game::vec3_t in, game::vec3_t out)
	{
		out[0] = in[0];
		out[1] = in[1];
		out[2] = in[2];
	}

	game::vec_t dot_product(const game::vec3_t v1, const game::vec3_t v2)
	{
		return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
	}

	float angle_normalize360(float angle)
	{
		return (360.0f / 65536) * ((int)(angle * (65536 / 360.0f)) & 65535);
	}

	float angle_normalize180(float angle)
	{
		angle = angle_normalize360(angle);
		if (angle > 180.0f) 
		{
			angle -= 360.0f;
		}
		return angle;
	}

	float vector_normalize(game::vec3_t v)
	{
		float length, ilength;

		length = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];

		if (length) 
		{
			ilength = 1.0f / sqrtf(length);
			length *= ilength;
			v[0] *= ilength;
			v[1] *= ilength;
			v[2] *= ilength;
		}

		return length;
	}

	void cross_product(const game::vec3_t v1, const game::vec3_t v2, game::vec3_t cross)
	{
		cross[0] = v1[1] * v2[2] - v1[2] * v2[1];
		cross[1] = v1[2] * v2[0] - v1[0] * v2[2];
		cross[2] = v1[0] * v2[1] - v1[1] * v2[0];
	}

	void project_point_on_plane(game::vec3_t dst, const game::vec3_t p, const game::vec3_t normal)
	{
		float d;
		game::vec3_t n;
		float inv_denom;

		inv_denom = 1.0f / dot_product(normal, normal);

		d = dot_product(normal, p) * inv_denom;

		n[0] = normal[0] * inv_denom;
		n[1] = normal[1] * inv_denom;
		n[2] = normal[2] * inv_denom;

		dst[0] = p[0] - d * n[0];
		dst[1] = p[1] - d * n[1];
		dst[2] = p[2] - d * n[2];
	}

	void perpendicular_vector(game::vec3_t dst, const game::vec3_t src)
	{
		int pos, i;
		float minelem = 1.0f;
		game::vec3_t tempvec;

		for (pos = 0, i = 0; i < 3; i++)
		{
			if (fabs(src[i]) < minelem) 
			{
				pos = i;
				minelem = fabs(src[i]);
			}
		}

		tempvec[0] = tempvec[1] = tempvec[2] = 0.0f;
		tempvec[pos] = 1.0f;

		project_point_on_plane(dst, tempvec, src);
		vector_normalize(dst);
	}

	void matrix_multiply(float in1[3][3], float in2[3][3], float out[3][3])
	{
		out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] + in1[0][2] * in2[2][0];
		out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] + in1[0][2] * in2[2][1];
		out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] + in1[0][2] * in2[2][2];
		out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] + in1[1][2] * in2[2][0];
		out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] + in1[1][2] * in2[2][1];
		out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] + in1[1][2] * in2[2][2];
		out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] + in1[2][2] * in2[2][0];
		out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] + in1[2][2] * in2[2][1];
		out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] + in1[2][2] * in2[2][2];
	}

	void rotate_point_around_vector(game::vec3_t dst, const game::vec3_t dir, const game::vec3_t point, float degrees)
	{
		float m[3][3];
		float im[3][3];
		float zrot[3][3];
		float tmpmat[3][3];
		float rot[3][3];
		int i;
		game::vec3_t vr, vup, vf;
		float rad;

		vf[0] = dir[0];
		vf[1] = dir[1];
		vf[2] = dir[2];

		perpendicular_vector(vr, dir);
		cross_product(vr, vf, vup);

		m[0][0] = vr[0];
		m[1][0] = vr[1];
		m[2][0] = vr[2];

		m[0][1] = vup[0];
		m[1][1] = vup[1];
		m[2][1] = vup[2];

		m[0][2] = vf[0];
		m[1][2] = vf[1];
		m[2][2] = vf[2];

		memcpy(im, m, sizeof(im));

		im[0][1] = m[1][0];
		im[0][2] = m[2][0];
		im[1][0] = m[0][1];
		im[1][2] = m[2][1];
		im[2][0] = m[0][2];
		im[2][1] = m[1][2];

		memset(zrot, 0, sizeof(zrot));
		zrot[0][0] = zrot[1][1] = zrot[2][2] = 1.0F;

		rad = deg_to_rad(degrees);
		zrot[0][0] = cos(rad);
		zrot[0][1] = sin(rad);
		zrot[1][0] = -sin(rad);
		zrot[1][1] = cos(rad);

		matrix_multiply(m, zrot, tmpmat);
		matrix_multiply(tmpmat, im, rot);

		for (i = 0; i < 3; i++) 
		{
			dst[i] = rot[i][0] * point[0] + rot[i][1] * point[1] + rot[i][2] * point[2];
		}
	}

	void axis_to_angles(game::vec3_t axis[3], game::vec3_t angles)
	{
		game::vec3_t right, roll_angles, tvec;


		vectoangles(axis[0], angles);
		vec3_copy(axis[1], right);

		rotate_point_around_vector(tvec, axisDefault[2], right, -angles[YAW]);
		rotate_point_around_vector(right, axisDefault[1], tvec, -angles[PITCH]);

		vectoangles(right, roll_angles);
		roll_angles[PITCH] = angle_normalize180(roll_angles[PITCH]);

		if (dot_product(right, axisDefault[1]) < 0) 
		{
			if (roll_angles[PITCH] < 0) 
			{
				roll_angles[PITCH] = -90 + (-90 - roll_angles[PITCH]);
			}
			else 
			{
				roll_angles[PITCH] = 90 + (90 - roll_angles[PITCH]);
			}
		}

		angles[ROLL] = -roll_angles[PITCH];
	}

	void axis_transpose(const float(*in)[3], float(*out)[3])
	{
		(*out)[0] = (*in)[0];
		(*out)[1] = (*in)[3];
		(*out)[2] = (*in)[6];
		(*out)[3] = (*in)[1];
		(*out)[4] = (*in)[4];
		(*out)[5] = (*in)[7];
		(*out)[6] = (*in)[2];
		(*out)[7] = (*in)[5];
		(*out)[8] = (*in)[8];
	}
}
