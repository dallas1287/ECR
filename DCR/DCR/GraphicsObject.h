#pragma once
#include <QOpenGLWidget>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLExtraFunctions>
#include "primitive_defs.h"
#include "shader_defs.h"

typedef std::vector<VertexData> VertexDataPool;
typedef std::vector<GLushort> IndexPool;

class GraphicsObject : protected QOpenGLExtraFunctions
{
public:
	GraphicsObject(QOpenGLWidget* parent);
	GraphicsObject(const GraphicsObject& other);
	virtual ~GraphicsObject();
	QOpenGLShaderProgram* ShaderProgram() const { return m_program.get(); }
	QOpenGLTexture* Texture() const { return m_texture.get(); }
	QOpenGLVertexArrayObject& Vao();
	QOpenGLBuffer Vbo();
	QOpenGLBuffer Ebo();
	GLuint PosAttr() { return m_shaderAttributes.m_posAttr; }
	GLuint ColorAttr() { return m_shaderAttributes.m_colAttr; }
	GLuint TextureAttr() { return m_shaderAttributes.m_texCoordAttr; }
	GLuint NormAttr() { return m_shaderAttributes.m_normalAttr; }
	void initialize();
	void initialize(const QString& path);
	void initBuffers() { initBuffers(m_vertexData, m_indices); }
	void initBuffers(VertexDataPool& data, IndexPool& indices);
	void setupBuffers() { setupBuffers(m_vertexData, m_indices); }
	void setupBuffers(VertexDataPool& data, IndexPool& indices);
	void initShaders(const QString& vertexPath, const QString& fragmentPath, const QString& geometryPath = QString(), bool setup = true);
	void initTexture(const QString& path);
	bool resetTexture(const int& width, const int& height, void* data);
	void setupAttributes();
	QMatrix4x4& getModelMatrix() { return m_modelMatrix; }
	virtual void Draw();
	void bindAll();
	void releaseAll();
	void bindToDraw();
	void releaseFromDraw();
	std::vector<VertexData>& getVertexData() { return m_vertexData; }
	void setVertexData(std::vector<VertexData>& vdata) { m_vertexData.clear();  m_vertexData.assign(vdata.begin(), vdata.end()); }
	std::vector<GLushort>& getIndices() { return m_indices; }
	void setIndices(std::vector<GLushort>& idata) { m_indices.clear(); m_indices.assign(idata.begin(), idata.end()); }
	bool isInverted() const { return m_inverted; }
	void setInverted(bool state) { m_inverted = state; }

protected:
	std::vector<VertexData> m_vertexData;
	std::vector<GLushort> m_indices;

private:
	QOpenGLWidget* m_parent;
	std::unique_ptr<QOpenGLShaderProgram> m_program;
	QOpenGLVertexArrayObject m_vao;
	QOpenGLBuffer m_vbo, m_ebo;
	std::unique_ptr<QOpenGLTexture> m_texture;
	ShaderAttributes m_shaderAttributes;
	QMatrix4x4 m_modelMatrix;
	bool m_inverted = true;
};

