#ifndef CAMERA_H
#define CAMERA_H

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>


class CameraBase
{
	enum ProjectionMode
	{
		Perspective,
		Orthogonal,
	};

	struct ViewPort
	{
		float TopLeftX;
		float TopLeftY;
		float Width;
		float Height;
		float MinDepth;
		float MaxDepth;
	};

public:
	CameraBase();
	virtual ~CameraBase();

	void                   SetEyeAndDir(glm::vec3& eye, glm::vec3& dir);
	void                   SetEye(glm::vec3& eye);
	void                   SetDir(glm::vec3& dir);

	void                   SetPerspectiveMode();
	void                   SetOrthogonalMode();

	void                   SetFov(float fov);
	void                   SetAspectRatio(float aspect);
	void                   SetOrthgonalMode();
	void                   SetOrthgonalData(const glm::vec3 &minimum, const glm::vec3 &maximum);

	void                   SetViewPort(float topLeftX, float topLeftY, float width, float height, float minDepth = 0.0f, float maxDepth = 1.0f);
					       
	inline glm::vec3	   GetEye()	                 const { return m_Eye; };
	inline glm::vec3	   GetDir()	                 const { return m_Dir; };

	inline float           GetFov()                  const { return m_Fov; };
	inline float           GetAspectRatio()          const { return m_Aspect; };

	inline ViewPort        GetViewPort()             const { return m_ViewPort; };

	inline glm::mat4x4	   GetViewMatrix()           const { return m_ViewMatrix; };
	inline glm::mat4x4	   GetProjectionMatrix()     const { return m_ProjectionMatrix; };
	inline glm::mat4x4     GetProjectionViewMatrix() const { return GetProjectionMatrix() * GetViewMatrix(); };
					       
protected:
	void                   UpdateMatrix();
	void                   UpdateViewMatrix();
	void                   UpdateProjectionMatrix();

	glm::vec3	           m_Eye;
	glm::vec3	           m_Dir;
	ViewPort               m_ViewPort;
	glm::mat4x4            m_ViewMatrix;
	glm::mat4x4            m_ProjectionMatrix;

	ProjectionMode         m_ProjectionMode;

	/*perspective projection data*/
	float                  m_Fov;
	float                  m_Aspect;
	float                  m_NearZ;
	float                  m_FarZ;

	/*orthgonal projection data*/
	float                  m_Left;
	float                  m_Right;
	float                  m_Bottom;
	float                  m_Top;
	float                  m_NearPlane;
	float                  m_FarPlane;
};


class CameraFTP : public CameraBase
{
public:
	CameraFTP();
	virtual ~CameraFTP();

	void SetEyeAndTarget(glm::vec3& eye, glm::vec3& target);

	inline float GetYaw() const { return m_Yaw; };
	inline float GetPitch() const { return m_Pitch; };
	inline float GetRotateRadius() const { return m_RotateRadius; };
					       
protected:
	void MoveForward(float delta);
	void MoveRight(float delta);
	void ZoomByFov(float delta);
	void ZoomByMove(float delta);
	void EulerRotate(float deltaX, float deltaY);
	void OrbitRotate(float deltaX, float deltaY);

private:
	float m_Yaw;
	float m_Pitch;
	float m_RotateRadius;

	glm::vec3 rotatePointByVector(const glm::vec3& p, const glm::vec3& v, float theta);
};


class Camera : public CameraFTP
{

public:
	Camera();
	virtual ~Camera();

	virtual void HandleMouseBtnPress(Button button, int x, int y, int pressed);
	virtual void HandleMouseBtnMove(Button button, int x, int y);
	virtual void HandleMouseBtnDoubleClick(Button button, int x, int y);
	virtual void HandleMouseScroll(float delta);
	virtual void HandleKey(KeyCode key, int pressed);

	inline void SetKeyMoveSpeed(float speed) { m_KeyMoveSpeed = speed; };
	inline void SetMouseLeftSpeed(float speed) { m_MouseLeftSpeed = speed; };
	inline void SetMouseRightSpeed(float speed) { m_MouseRightSpeed = speed; };
	inline void SetMouseScrollSpeed(float speed) { m_MouseScrollSpeed = speed; };

private:	
	int		m_MouseX;
	int		m_MouseY;
	bool    m_MouseLeftBtnFirstPress;
	bool    m_MouseRightBtnFirstPress;
	float   m_KeyMoveSpeed;
	float   m_MouseLeftSpeed;
	float   m_MouseRightSpeed;
	float   m_MouseScrollSpeed;
};

#endif //CAMERA_H
