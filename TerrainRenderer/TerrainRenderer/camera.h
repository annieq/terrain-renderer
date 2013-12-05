#pragma once

#include"common.h"

class Camera
{
public:
    Camera();

    void Move(D3DXVECTOR3);   //move camera
    void Rotate(D3DXVECTOR3); //rotate view
	
	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();
	
	void GetViewMatrix(D3DXMATRIX&);


private:
    D3DXVECTOR3     m_position, m_rotation;

    D3DXMATRIX      m_viewMatrix;    //view matrix
};
