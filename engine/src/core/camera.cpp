#include <algorithm>
#include <string>

#include "camera.hpp"


CameraBase::CameraBase():
	//m_ProjectionMode(ProjectionMode::Orthogonal),
	m_ProjectionMode(ProjectionMode::Perspective),

	m_Fov(45.0f),
	m_Aspect(1.0f),
	m_NearZ(0.1f),
	m_FarZ(100000.0f),
	
	m_Left(-100.0f),
	m_Right(100.0f),
	m_Bottom(-100.0f),
	m_Top(100.0f),
	m_NearPlane(0.1f),
	m_FarPlane(100000.0f),

	m_Eye(glm::vec3(0.0f, 0.0f, 3.0f)),
	m_Dir(-glm::normalize(m_Eye)),

	m_ViewMatrix(glm::mat4x4()),
	m_ProjectionMatrix(glm::mat4x4())
{
	UpdateMatrix();
}

CameraBase::~CameraBase()
{
}


void CameraBase::SetEyeAndDir(glm::vec3& eye, glm::vec3& dir)
{
	m_Eye = eye;
	m_Dir = dir;

	UpdateViewMatrix();
}

void CameraBase::SetEye(glm::vec3& eye)
{
	m_Eye = eye;
	UpdateViewMatrix();
}

void CameraBase::SetDir(glm::vec3& dir)
{
	m_Dir = dir;
	UpdateViewMatrix();
}


void CameraBase::SetPerspectiveMode()
{
	m_ProjectionMode = ProjectionMode::Perspective;
}

void CameraBase::SetOrthogonalMode()
{
	m_ProjectionMode = ProjectionMode::Orthogonal;
}


void CameraBase::SetFov(float fov)
{
	m_Fov = fov;
	UpdateProjectionMatrix();
}

void CameraBase::SetAspectRatio(float aspect)
{
	m_Aspect = aspect;
	UpdateProjectionMatrix();
}

void CameraBase::SetOrthgonalMode()
{
	m_ProjectionMode = ProjectionMode::Orthogonal;
}

void CameraBase::SetOrthgonalData(const glm::vec3 &minimum, const glm::vec3 &maximum)
{
	m_Left = -(maximum.x - minimum.x)*2;
	m_Right = (maximum.x - minimum.x) * 2;
	m_Bottom = -(maximum.y - minimum.y) * 2;
	m_Top = (maximum.y - minimum.y) * 2;
	UpdateProjectionMatrix();
}


void CameraBase::SetViewPort(float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth)
{
	m_ViewPort.TopLeftX = topLeftX;
	m_ViewPort.TopLeftY = topLeftY;
	m_ViewPort.Width = width;
	m_ViewPort.Height = height;
	m_ViewPort.MinDepth = minDepth;
	m_ViewPort.MaxDepth = maxDepth;
}


void CameraBase::UpdateMatrix()
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void CameraBase::UpdateViewMatrix()
{
	glm::vec3 right = glm::normalize(glm::cross(m_Dir, glm::vec3(0, 1, 0)));
	glm::vec3 up = glm::normalize(glm::cross(right, m_Dir));
	m_ViewMatrix = glm::lookAt(m_Eye, m_Eye + m_Dir, up);
}

void CameraBase::UpdateProjectionMatrix()
{
	if (m_ProjectionMode == ProjectionMode::Perspective)
	{
		m_ProjectionMatrix = glm::perspective(m_Fov, m_Aspect, m_NearZ, m_FarZ);
	}
	else
	{
		m_ProjectionMatrix = glm::ortho(m_Left, m_Right, m_Bottom, m_Top, m_NearPlane, m_FarPlane);
	}
}


CameraFTP::CameraFTP():
	m_Yaw(0.0f),
	m_Pitch(0.0f),
	m_RotateRadius(0.0f)
{
}

CameraFTP::~CameraFTP()
{
}


void CameraFTP::SetEyeAndTarget(glm::vec3& eye, glm::vec3& target)
{
	auto dir = glm::normalize(target - eye);
	m_RotateRadius = glm::distance(eye, target);
	m_Yaw = 0.0f;
	m_Pitch = 0.0f;
	SetEyeAndDir(eye, dir);
}


void CameraFTP::MoveForward(float delta)
{
	auto eye = GetEye();
	auto dir = GetDir();

	eye = eye + delta * dir;

	SetEye(eye);
}

void CameraFTP::MoveRight(float delta)
{
	auto eye = GetEye();
	auto dir = GetDir();

	auto right = glm::normalize(glm::cross(dir, glm::vec3(0, 1, 0)));
	eye = eye + delta * right;

	SetEye(eye);
}

void CameraFTP::ZoomByFov(float delta)
{
	float fov = GetFov();

	fov += delta;
	if (fov < 1.0f)
	{
		fov = 1.0f;
	}
	else if (fov > 150.0f)
	{
		fov = 150.0f;
	}

	SetFov(fov);
}

void CameraFTP::ZoomByMove(float delta)
{
	auto eye = GetEye();
	auto dir = GetDir();

	glm::vec3 center = eye + m_RotateRadius * dir;
	eye += (float)delta * dir;
	m_RotateRadius = glm::distance(center, eye);

	SetEye(eye);
}

void CameraFTP::EulerRotate(float deltaX, float deltaY)
{
	auto dir = GetDir();

	glm::vec3 right = glm::normalize(glm::cross(dir, glm::vec3(0, 1, 0)));
	glm::vec3 up = glm::normalize(glm::cross(right, dir));

	float yaw = glm::radians(deltaX);
	float pitch = glm::radians(deltaY);

	auto qx = glm::quat(yaw, up);
	auto qy = glm::quat(pitch, right);
	dir = glm::normalize(qy * qx * dir);

	SetDir(dir);
}

void CameraFTP::OrbitRotate(float deltaX, float deltaY)
{
	auto eye = GetEye();
	auto dir = GetDir();

	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::vec3 center = eye + m_RotateRadius * dir;
	glm::vec3 right = glm::normalize(glm::cross(dir, up));

	glm::vec3 tmpEye = eye - center;

	// left-right rotate
	m_Yaw += deltaX;
	tmpEye = rotatePointByVector(tmpEye, up, deltaX);

	// up-down rotate
	m_Pitch += deltaY;
	if (-80.0f < m_Pitch && m_Pitch < 80.0f)
	{
		tmpEye = rotatePointByVector(tmpEye, right, deltaY);
	}
	else
	{
		m_Pitch = std::min(m_Pitch, 80.0f);
		m_Pitch = std::max(m_Pitch, -80.0f);
	}

	eye = tmpEye + center;
	dir = glm::normalize(center - eye);

	SetEyeAndDir(eye, dir);
}


glm::vec3 CameraFTP::rotatePointByVector(const glm::vec3& p, const glm::vec3& v, float theta)
{
	float r = glm::radians(theta);
	float c = cos(r);
	float s = sin(r);
	float newX = (v.x*v.x*(1 - c) + c) * p.x + (v.x*v.y*(1 - c) - v.z * s) * p.y + (v.x*v.z*(1 - c) + v.y * s) * p.z;
	float newY = (v.y*v.x*(1 - c) + v.z * s) * p.x + (v.y*v.y*(1 - c) + c) * p.y + (v.y*v.z*(1 - c) - v.x * s) * p.z;
	float newZ = (v.x*v.z*(1 - c) - v.y * s) * p.x + (v.y*v.z*(1 - c) + v.x * s) * p.y + (v.z*v.z*(1 - c) + c) * p.z;
	return glm::vec3(newX, newY, newZ);
}


Camera::Camera():
	m_MouseX(0),
	m_MouseY(0),
	m_MouseLeftBtnFirstPress(false),
	m_MouseRightBtnFirstPress(false),
	m_KeyMoveSpeed(200.0f),
	m_MouseLeftSpeed(200.0f),
	m_MouseRightSpeed(100.0f),
	m_MouseScrollSpeed(40.0f)
{
}

Camera::~Camera()
{
}


void Camera::HandleMouseLeftBtnPress(int x, int y)
{
	(void)(x);
	(void)(y);
	m_MouseLeftBtnFirstPress = true;
}

void Camera::HandleMouseRightBtnPress(int x, int y)
{
	(void)(x);
	(void)(y);
	m_MouseRightBtnFirstPress = true;
}

void Camera::HandleMouseLeftBtnMove(int x, int y)
{
	if (m_MouseLeftBtnFirstPress)
	{
		m_MouseX = x;
		m_MouseY = y;
		m_MouseLeftBtnFirstPress = false;
		return;
	}

	int dx = m_MouseX - x;
	int dy = y - m_MouseY;
	m_MouseX = x;
	m_MouseY = y;

	auto speed = m_MouseRightSpeed * 0.001f;
	OrbitRotate(dx*speed, -dy* speed);
}

void Camera::HandleMouseRightBtnMove(int x, int y)
{
	if (m_MouseRightBtnFirstPress)
	{
		m_MouseX = x;
		m_MouseY = y;
		m_MouseRightBtnFirstPress = false;
		return;
	}

	int dx = m_MouseX - x;
	int dy = y - m_MouseY;
	m_MouseX = x;
	m_MouseY = y;

	auto speed = m_MouseLeftSpeed * 0.0001f;
	EulerRotate(dx*speed, dy*speed);
}

void Camera::HandleMouseLeftBtnRelease(int x, int y)
{
	(void)(x);
	(void)(y);
	m_MouseLeftBtnFirstPress = false;
}

void Camera::HandleMouseRightBtnRelease(int x, int y)
{
	(void)(x);
	(void)(y);
	m_MouseRightBtnFirstPress = false;
}

void Camera::HandleMouseLeftBtnDoubleClick(int x, int y)
{
	(void)(x);
	(void)(y);
}

void Camera::HandleMouseScroll(int delta)
{
	auto speed = m_MouseScrollSpeed * 0.1f;
	ZoomByMove(delta*speed);
}

void Camera::HandleKey(unsigned char key)
{
	auto speed = m_KeyMoveSpeed * 0.1f;
	switch (toupper(key))
	{
	case 'W':	MoveForward(speed);		break;
	case 'S':	MoveForward(-speed);	break;
	case 'A':	MoveRight(-speed);		break;
	case 'D':	MoveRight(speed);		break;
	}
}
