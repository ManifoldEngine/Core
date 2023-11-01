#pragma once

#include "OpenGL.h"
#include <memory>
#include <vector>

namespace ECSEngine
{
	class OpenGLVertexBuffer;
	class OpenGLIndexBuffer;

	class OpenGLVertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		// can't implicetely copy a VAO.
		OpenGLVertexArray(const OpenGLVertexArray&) = delete;

		void bind() const;

		void addVertexBuffer(std::shared_ptr<OpenGLVertexBuffer> pBuffer);
		void setIndexBuffer(std::shared_ptr<OpenGLIndexBuffer> pBuffer);
	
		std::shared_ptr<OpenGLIndexBuffer> getIndexBuffer() const;
	private:
		unsigned int m_vertexArrayObjectId;
		int m_vertexArrayLastIndex = 0;

		std::vector<std::shared_ptr<OpenGLVertexBuffer>> m_vertexBuffers;
		std::shared_ptr<OpenGLIndexBuffer> m_pIndexBuffer;
	};
}