#include "GraphicsObject.h"

GraphicsObject::GraphicsObject() : m_ebo(QOpenGLBuffer(QOpenGLBuffer::IndexBuffer))
{
	initializeOpenGLFunctions();
	//initialize arrays and attributes
	Vao();
	Vbo();
	Ebo();
}

GraphicsObject::~GraphicsObject()
{
}

QOpenGLVertexArrayObject& GraphicsObject::Vao()
{
	if (!m_vao.isCreated())
		m_vao.create();
	return m_vao;
}

QOpenGLBuffer GraphicsObject::Vbo()
{
	if (!m_vbo.isCreated())
		m_vbo.create();
	return m_vbo;
}

QOpenGLBuffer GraphicsObject::Ebo()
{
	if (!m_ebo.isCreated())
		m_ebo.create();
	return m_ebo;
}

void GraphicsObject::initialize()
{
	initShaders("base_vs.glsl", "base_frag.glsl");
	//initTexture("../harley.jpg");
	initTexture("../scene5.png");
	//createQuad(m_vertexData, m_indices);
	createTexInvertedQuad(m_vertexData, m_indices);
	setupBuffers();
}

void GraphicsObject::initShaders(const QString& vertexPath, const QString& fragmentPath, const QString& geometryPath, bool setup)
{
	m_program.reset(new QOpenGLShaderProgram);
	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexPath))
		qDebug() << m_program->log();
	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentPath))
		qDebug() << m_program->log();
	if (!geometryPath.isEmpty())
	{
		if (!m_program->addShaderFromSourceFile(QOpenGLShader::Geometry, geometryPath))
			qDebug() << m_program->log();
	}

	if (!m_program->link())
	{
		qDebug() << m_program->log();
		return;
	}
	if (setup)
		setupAttributes();
}

void GraphicsObject::initTexture(const QString& path)
{
	//OpenGL flips the image so it needs to be mirrored when created
	m_texture.reset(new QOpenGLTexture(QImage(path).mirrored()));

	// Set nearest filtering mode for texture minification
	m_texture->setMinificationFilter(QOpenGLTexture::Nearest);

	// Set bilinear filtering mode for texture magnification
	m_texture->setMagnificationFilter(QOpenGLTexture::Linear);

	// Wrap texture coordinates by repeating
	m_texture->setWrapMode(QOpenGLTexture::Repeat);
}

bool GraphicsObject::resetTexture(const int& width, const int& height, void* data)
{
	m_texture.reset(new QOpenGLTexture(QOpenGLTexture::Target2D));

	m_texture->setSize(width, height);
	m_texture->setFormat(QOpenGLTexture::RGBA8_UNorm);
	m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
	m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
	m_texture->setWrapMode(QOpenGLTexture::Repeat);

	m_texture->allocateStorage(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8);
	if (!m_texture->isStorageAllocated())
		return false;

	m_texture->setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, data);

	return true;
}

void GraphicsObject::setupAttributes()
{
	m_shaderAttributes.m_posAttr = m_program->attributeLocation("posAttr");
	m_shaderAttributes.m_texCoordAttr = m_program->attributeLocation("texCoordAttr");
	m_shaderAttributes.m_colAttr = m_program->attributeLocation("colAttr");
	m_shaderAttributes.m_normalAttr = m_program->attributeLocation("normAttr");
	m_shaderAttributes.m_modelUniform = m_program->uniformLocation("model");
	m_shaderAttributes.m_viewUniform = m_program->uniformLocation("view");
	m_shaderAttributes.m_projectionUniform = m_program->uniformLocation("projection");
}

void GraphicsObject::bindAll()
{
	m_program->bind();
	m_vao.bind();
	m_vbo.bind();
	m_ebo.bind();
}

void GraphicsObject::releaseAll()
{
	m_vao.release();
	m_ebo.release();
	m_vbo.release();
	m_program->release();
}

void GraphicsObject::bindToDraw()
{
	if (!m_program)
		return;
	m_program->bind();

	if (m_texture)
		m_texture->bind();

	m_vao.bind();
}

void GraphicsObject::releaseFromDraw()
{
	m_vao.release();
	if (m_texture && m_texture->isBound())
		m_texture->release();
	if (m_program)
		m_program->release();
}

void GraphicsObject::initBuffers(VertexDataPool& data, IndexPool& indices)
{
	if (data.empty() || indices.empty())
		return;

	bindAll();
	Vbo().allocate(&data[0], data.size() * sizeof(VertexData));

	int offset = 0;

	ShaderProgram()->enableAttributeArray(PosAttr());
	ShaderProgram()->setAttributeBuffer(PosAttr(), GL_FLOAT, offset, 3, sizeof(VertexData));

	offset += sizeof(decltype(data[0].position));

	ShaderProgram()->enableAttributeArray(TextureAttr());
	ShaderProgram()->setAttributeBuffer(TextureAttr(), GL_FLOAT, offset, 3, sizeof(VertexData));

	offset += sizeof(decltype(data[0].texCoord));

	ShaderProgram()->enableAttributeArray(NormAttr());
	ShaderProgram()->setAttributeBuffer(NormAttr(), GL_FLOAT, offset, 3, sizeof(VertexData));

	offset += sizeof(decltype(data[0].normal));

	ShaderProgram()->enableAttributeArray(ColorAttr());
	ShaderProgram()->setAttributeBuffer(ColorAttr(), GL_FLOAT, offset, 4, sizeof(VertexData));

	Ebo().allocate(&indices[0], indices.size() * sizeof(GLushort));
	releaseAll();
}

void GraphicsObject::setupBuffers(VertexDataPool& data, IndexPool& indices)
{
	if (data.empty() || indices.empty())
		return;

	m_vao.bind();
	m_vbo.bind();
	m_ebo.bind();
	m_vbo.allocate(&data[0], data.size() * sizeof(VertexData));

	int offset = 0;

	glEnableVertexAttribArray(Shader::Position);
	glVertexAttribPointer(Shader::Position, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offset);

	offset += sizeof(decltype(data[0].position));

	glEnableVertexAttribArray(Shader::TexCoords);
	glVertexAttribPointer(Shader::TexCoords, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offset);

	offset += sizeof(decltype(data[0].texCoord));

	glEnableVertexAttribArray(Shader::Normal);
	glVertexAttribPointer(Shader::Normal, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offset);

	offset += sizeof(decltype(data[0].normal));

	glEnableVertexAttribArray(Shader::Color);
	glVertexAttribPointer(Shader::Color, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offset);

	offset += sizeof(decltype(data[0].color));

	glEnableVertexAttribArray(Shader::Bone0);
	glVertexAttribPointer(Shader::Bone0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offset);

	offset += sizeof(QVector4D);

	glEnableVertexAttribArray(Shader::Bone1);
	glVertexAttribPointer(Shader::Bone1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offset);

	offset += sizeof(QVector4D);

	glEnableVertexAttribArray(Shader::Bone2);
	glVertexAttribPointer(Shader::Bone2, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offset);

	offset += sizeof(QVector4D);

	glEnableVertexAttribArray(Shader::Bone3);
	glVertexAttribPointer(Shader::Bone3, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offset);

	Ebo().allocate(&indices[0], indices.size() * sizeof(GLushort));
	m_vao.release();
	m_ebo.release();
	m_vbo.release();
}

void GraphicsObject::Draw()
{
	if (getIndices().empty())
		return;
	bindToDraw();
	glDrawElements(GL_TRIANGLES, getIndices().size(), GL_UNSIGNED_SHORT, 0);
	releaseFromDraw();
}