#pragma once
#include <vector>
#include <QMatrix4x4>
#include <QOpenGLExtraFunctions>
#include <QtMath>

const QVector3D X(1.0, 0.0, 0.0);
const QVector3D Y(0.0, 1.0, 0.0);
const QVector3D Z(0.0, 0.0, 1.0);

const QVector4D X4(1.0f, 0.0f, 0.0f, 0.0f);
const QVector4D Y4(0.0f, 1.0f, 0.0f, 0.0f);
const QVector4D Z4(0.0f, 0.0f, 1.0f, 0.0f);
const QVector4D W4(0.0f, 0.0f, 0.0f, 1.0f);

struct VertexData
{
	VertexData(QVector3D position = QVector3D(), QVector3D texCoord = QVector3D(), QVector3D norm = QVector3D(),
		QVector4D col = QVector4D(), QMatrix4x4 trans = QMatrix4x4()) :
		position(position), texCoord(texCoord), normal(norm), color(col), transform(trans) {};
	~VertexData() {};
	VertexData& operator=(const VertexData& other)
	{
		position = other.position;
		texCoord = other.texCoord;
		normal = other.normal;
		color = other.color;
		transform = other.transform;
		return *this;
	}

	QVector3D position;
	QVector3D texCoord;
	QVector3D normal;
	QVector4D color;
	QMatrix4x4 transform;
};


//opengl textures map (0,0) bottom left -> (1, 1) topr right
const std::vector<VertexData> QuadTemplate =
{
	{ QVector3D(X + Y), QVector3D(1.0, 1.0, 0.0), QVector3D(0.0, 1.0, 0.0) }, // top right
	{ QVector3D(X - Y), QVector3D(1.0, 0.0, 0.0), QVector3D(0.0, 1.0, 0.0) }, // bottom right
	{ QVector3D(-X - Y), QVector3D(0.0, 0.0, 0.0), QVector3D(0.0, 1.0, 0.0) }, // bottom left
	{ QVector3D(-X + Y), QVector3D(0.0, 1.0, 0.0), QVector3D(0.0, 1.0, 0.0) }  // top left 
};

const std::vector<VertexData> QuadTemplateTexInverted =
{
	{ QVector3D(X + Y), QVector3D(1.0, 0.0, 0.0), QVector3D(0.0, 1.0, 0.0) }, // top right
	{ QVector3D(X - Y), QVector3D(1.0, 1.0, 0.0), QVector3D(0.0, 1.0, 0.0) }, // bottom right
	{ QVector3D(-X - Y), QVector3D(0.0, 1.0, 0.0), QVector3D(0.0, 1.0, 0.0) }, // bottom left
	{ QVector3D(-X + Y), QVector3D(0.0, 0.0, 0.0), QVector3D(0.0, 1.0, 0.0) }  // top left 
};

const std::vector<GLushort> QuadIndices =
{
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};

const std::vector<GLushort> QuadOutlineIndices =
{
	0, 1,
	1, 2,
	2, 3,
	3, 0
};

static void createQuad(std::vector<VertexData>& vertices, std::vector<GLushort>& indices)
{
	vertices.insert(vertices.end(), QuadTemplate.begin(), QuadTemplate.end());
	indices.insert(indices.end(), QuadIndices.begin(), QuadIndices.end());
}

static void createTexInvertedQuad(std::vector<VertexData>& vertices, std::vector<GLushort>& indices)
{
	vertices.clear();
	indices.clear();
	vertices.insert(vertices.end(), QuadTemplateTexInverted.begin(), QuadTemplateTexInverted.end());
	indices.insert(indices.end(), QuadIndices.begin(), QuadIndices.end());
}

static QVector4D gammaCorrected(const QVector4D& input)
{
	return QVector4D(qPow(input.x(), (1 / 2.2)), qPow(input.y(), (1 / 2.2)), qPow(input.z(), (1 / 2.2)), 1.0);
}