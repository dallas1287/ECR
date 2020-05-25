#pragma once
#include <QOpenGLExtraFunctions>

namespace Shader
{
	enum Attributes
	{
		Position = 0,
		Color,
		TexCoords,
		Normal,
		Bone0,
		Bone1,
		Bone2,
		Bone3,
		Model,
		View,
		Projection,
		SceneData
	};
};

struct ShaderAttributes
{
	GLuint m_posAttr = -1;
	GLuint m_colAttr = -1;
	GLuint m_texCoordAttr = -1;
	GLuint m_normalAttr = -1;
	GLuint m_boneTransform0 = -1;
	GLuint m_boneTransform1 = -1;
	GLuint m_boneTransform2 = -1;
	GLuint m_boneTransform3 = -1;
	GLuint m_modelUniform = -1;
	GLuint m_viewUniform = -1;
	GLuint m_projectionUniform = -1;
};
