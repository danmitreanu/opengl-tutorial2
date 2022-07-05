#include "VertexLayout.h"
#include <OpenGL/gl3.h>
#include <iostream>

void VertexLayout::AddVertexAttribute(const char* name, const std::size_t number_of_floats)
{
	VertexAttribute attrib;
	attrib.name = name;
	attrib.number_of_floats = number_of_floats;

	m_Attributes.push_back(attrib);
}

std::size_t VertexLayout::size() const
{
	return this->float_count() * sizeof(float);
}

std::size_t VertexLayout::float_count() const
{
	int total = 0;
	for (auto a : m_Attributes)
		total += a.number_of_floats;

	return total;
}

VertexBuffer::VertexBuffer()
{
	// generate the buffer
	glGenVertexArrays(1, &m_Vao);
}

VertexBuffer::~VertexBuffer()
{
	if (m_Vao)
		glDeleteVertexArrays(1, &m_Vao);

	if (m_BufferCount)
		glDeleteBuffers(m_BufferCount, m_Buffers);
}

const std::vector<VertexAttribute> VertexLayout::get_attributes() const
{
	return m_Attributes;
}

void VertexBuffer::create(const float* data, const VertexLayout& vertex_layout, std::size_t vertex_count)
{
	int attrib_count = vertex_layout.get_attributes().size();

	glBindVertexArray(m_Vao);

	m_Buffers = new GLuint[vertex_count];
	// only one buffer per vertex including all attributes
	m_BufferCount = vertex_count;
	glGenBuffers(vertex_count, m_Buffers);

	int offset = 0;

	// Create buffers for vertices attributes
	for (int i = 0; i < vertex_count; i++)
	{
		int data_pos = i * vertex_layout.float_count();

		int vertex_layout_size = vertex_layout.size();
		glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[i]);
		glBufferData(GL_ARRAY_BUFFER, vertex_layout.size(), (void*)(data + data_pos), GL_STATIC_DRAW);
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, vertex_layout.float_count(), GL_FLOAT, GL_FALSE, 0, (void*)(offset * vertex_layout.size()));

		offset++;
	}
}
