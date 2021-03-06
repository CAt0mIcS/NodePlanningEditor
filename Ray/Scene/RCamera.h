#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Ray/RBase.h"
#include "Ray/Events/REventListener.h"
#include "Ray/Events/REventDispatcher.h"
#include "Ray/Events/RMouseEvents.h"
#include "Ray/Events/RKeyboardEvents.h"
#include "../Core/RTime.h"
#include "Ray/Core/RMath.h"
#include "Ray/Core/RFrustum.h"


namespace At0::Ray
{
	class BufferUniform;

	class RAY_EXPORT CameraChangedEvent
	{
	public:
		CameraChangedEvent() = default;
	};

	class RAY_EXPORT Camera :
		public EventDispatcher<CameraChangedEvent>,
		EventListener<MouseMovedEvent>,
		EventListener<KeyPressedEvent>,
		EventListener<KeyReleasedEvent>
	{
	public:
		enum CameraType
		{
			LookAt,
			FirstPerson
		};

	public:
		Float3 Rotation{};
		Float3 Position{};

		float RotationSpeed = 1.0f, MovementSpeed = 1.0f;

		bool Updated = false;
		bool FlipY = true;
		bool FreezeFrustum = false;
		CameraType Type = CameraType::FirstPerson;

		struct Data
		{
			Matrix View = MatrixIdentity();
			Matrix Projection = MatrixIdentity();
			Float3 ViewPos{};
		} ShaderData{};

		struct
		{
			bool Left = false;
			bool Right = false;
			bool Forward = false;
			bool Backward = false;
			bool Up = false;
			bool Down = false;
		} Keys{};

	public:
		Camera();
		~Camera();
		bool IsMoving() const;
		float GetNearClip() const { return m_NearZ; }
		float GetFarClip() const { return m_FarZ; }
		void SetPerspective(float fov, float aspect, float nearZ, float farZ);
		void UpdateAspectRatio(float aspect);
		void SetPosition(Float3 pos);
		void SetRotation(Float3 rotation);
		void Rotate(Float3 delta);
		void Translate(Float3 delta);
		void SetRotationSpeed(float speed) { RotationSpeed = speed; }
		void SetMovementSpeed(float speed) { MovementSpeed = speed; }
		void Update(Delta dt);

		const Frustum& GetFrustum() const { return m_Frustum; }

	private:
		void UpdateViewMatrix();
		void DispatchCameraChanged(CameraChangedEvent e);

		void OnEvent(MouseMovedEvent& e) override;
		void OnEvent(KeyPressedEvent& e) override;
		void OnEvent(KeyReleasedEvent& e) override;

	private:
		float m_FoV;
		float m_NearZ, m_FarZ;
		Frustum m_Frustum;
	};
}  // namespace At0::Ray
