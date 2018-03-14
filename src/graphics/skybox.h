#pragma once

#include "Renderable3D.h"

// 3D world skybox
class Skybox : public Renderable3D
{
public:
	Skybox(const std::string& skyboxName);

	// Overrides base class GetModelMatrix() function
	// The skybox must not calculate position or scale matrices, therefore it must have its own version
	inline mat4 GetModelMatrix() const override
	{
		return rotationMatrix;
	};

	virtual void Draw(Camera &camera, mat4 &vpMatrix) override;

private:
	float scale;
};