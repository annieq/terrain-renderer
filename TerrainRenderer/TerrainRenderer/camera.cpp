#include "camera.h"

Camera::Camera() 
{
	m_position.x = 0.0f;
	m_position.y = 30.0f;
	m_position.z = -5.0f;
			  
	m_rotation.x = 15.0f;
	m_rotation.y = 0.0f;
	m_rotation.z = 0.0f;
}

void Camera::Move(D3DXVECTOR3 move)
{
	m_position.x += move.x*CAM_MOVE;
	m_position.y += move.y*CAM_MOVE;
	m_position.z += move.z*CAM_MOVE;
	return;
}

void Camera::Rotate(D3DXVECTOR3 rotate)
{
	m_rotation.x += rotate.x*CAM_ROTATE;
	m_rotation.y += rotate.y*CAM_ROTATE;
	m_rotation.z += rotate.z*CAM_ROTATE;
	return;
}

D3DXVECTOR3 Camera::GetPosition()
{
	return m_position;
}


D3DXVECTOR3 Camera::GetRotation()
{
	return m_rotation;
}
	
void Camera::GetViewMatrix(D3DXMATRIX& viewMx) {
	D3DXVECTOR3 up(0.0f,1.0f,0.0f);	// wektor wskazuj¹cy w górê
	D3DXVECTOR3 lookAt(0.0f,0.0f,1.0f);	// wektor domyœlnego kierunku patrzenia (w kierunku Z)

	// rotacja w radianach (pitch, yaw i roll). Potrzebne do macierzy rotacji
	float pitch = m_rotation.x * RADIANS_PER_DEGREES;
	float yaw = m_rotation.y * RADIANS_PER_DEGREES;
	float roll = m_rotation.z * RADIANS_PER_DEGREES;

	D3DXMATRIX rotationMatrix;

	D3DXMatrixRotationYawPitchRoll(&rotationMatrix,yaw,pitch,roll);

	// Transformacja wektorów up i lookAt przez macierz rotacji
	// (¿eby widok by³ zwrócony w odpowiednim kierunku)
	
	D3DXVec3TransformCoord(&lookAt,&lookAt,&rotationMatrix);
	D3DXVec3TransformCoord(&up,&up,&rotationMatrix);

	// przesuniêcie punktu "lookAt" na pozycjê obserwatora
	lookAt = m_position + lookAt;

	// Obliczenie macierzy widoku
	D3DXMatrixLookAtLH(&m_viewMatrix, &m_position, &lookAt, &up);

	viewMx = m_viewMatrix;
}