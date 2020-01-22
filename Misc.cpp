#include "Misc.h"

cMisc* Misc = new cMisc();

#define Assert( _exp ) ((void)0)

struct vMatrix
{
	vMatrix() {}
	vMatrix(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23)
	{
		m_flMatVal[0][0] = m00;	m_flMatVal[0][1] = m01; m_flMatVal[0][2] = m02; m_flMatVal[0][3] = m03;
		m_flMatVal[1][0] = m10;	m_flMatVal[1][1] = m11; m_flMatVal[1][2] = m12; m_flMatVal[1][3] = m13;
		m_flMatVal[2][0] = m20;	m_flMatVal[2][1] = m21; m_flMatVal[2][2] = m22; m_flMatVal[2][3] = m23;
	}

	float *operator[](int i) { Assert((i >= 0) && (i < 3)); return m_flMatVal[i]; }
	const float *operator[](int i) const { Assert((i >= 0) && (i < 3)); return m_flMatVal[i]; }
	float *Base() { return &m_flMatVal[0][0]; }
	const float *Base() const { return &m_flMatVal[0][0]; }

	float m_flMatVal[3][4];
};

vMatrix Matrix(Vector3 rot, Vector3 origin)
{
	origin = Vector3(0, 0, 0);
	float radPitch = (rot.x * float(PI) / 180.f);
	float radYaw = (rot.y * float(PI) / 180.f);
	float radRoll = (rot.z * float(PI) / 180.f);

	float SP = sinf(radPitch);
	float CP = cosf(radPitch);
	float SY = sinf(radYaw);
	float CY = cosf(radYaw);
	float SR = sinf(radRoll);
	float CR = cosf(radRoll);

	vMatrix matrix;
	matrix[0][0] = CP * CY;
	matrix[0][1] = CP * SY;
	matrix[0][2] = SP;
	matrix[0][3] = 0.f;

	matrix[1][0] = SR * SP * CY - CR * SY;
	matrix[1][1] = SR * SP * SY + CR * CY;
	matrix[1][2] = -SR * CP;
	matrix[1][3] = 0.f;

	matrix[2][0] = -(CR * SP * CY + SR * SY);
	matrix[2][1] = CY * SR - CR * SP * SY;
	matrix[2][2] = CR * CP;
	matrix[2][3] = 0.f;

	return matrix;
}

bool cMisc::WorldToScreen(Vector3 world , Vector2 * screen) {
	Vector3 Screenlocation = Vector3(0, 0, 0);
	Vector3 Rotation = SOT->localCamera.angles;	

	vMatrix tempMatrix = Matrix(Rotation, Vector3(0, 0, 0)); 

	Vector3 vAxisX, vAxisY, vAxisZ;

	vAxisX = Vector3(tempMatrix[0][0], tempMatrix[0][1], tempMatrix[0][2]);
	vAxisY = Vector3(tempMatrix[1][0], tempMatrix[1][1], tempMatrix[1][2]);
	vAxisZ = Vector3(tempMatrix[2][0], tempMatrix[2][1], tempMatrix[2][2]);

	Vector3 vDelta = world - SOT->localCamera.position;
	Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (vTransformed.z < 1.f)
		vTransformed.z = 1.f;

	

	float FovAngle = SOT->localCamera.fov;
	float ScreenCenterX = Process->Size[0] / 2.0f;
	float ScreenCenterY = Process->Size[1] / 2.0f;
	
	auto tmpFOV = tanf(FovAngle * (float)PI / 360.f);

	screen->x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tmpFOV )/ vTransformed.z;
	screen->y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)PI / 360.f)) / vTransformed.z;
	
	
	/*
	auto Ratio = Process->Size[0] / Process->Size[1];
	if (Ratio < 4.0f / 3.0f)
		Ratio = 4.0f / 3.0f;

	auto FOV = Ratio / (16.0f / 9.0f) * tanf(SOT->localCamera.fov * PI / 360.0f);

	int debug = FOV;

	//screen->x = ScreenCenterX + vTransformed.x * ScreenCenterX / FOV / vTransformed.z;
	//screen->y = ScreenCenterY - vTransformed.y * ScreenCenterX / FOV / vTransformed.z;
	*/


	return true;
}

std::string cMisc::wstringToString(std::wstring wstring)
{
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;
	return converter.to_bytes(wstring);
}



