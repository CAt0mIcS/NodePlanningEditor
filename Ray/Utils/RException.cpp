#include "Rpch.h"
#include "RException.h"

#include "RString.h"


namespace At0::Ray
{
	Exception::Exception(uint16_t line, const char* file) : m_File(file), m_Line(line) {}

	std::string Exception::GetDefaultString() const
	{
		std::ostringstream oss;
		oss << "[File] " << m_File << '\n' << "[Line] " << m_Line;

		return oss.str();
	}

	const char* Exception::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << '\n' << GetDefaultString();

		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}

	RuntimeException::RuntimeException(const char* message, uint16_t line, const char* file)
		: Exception(line, file), m_Message(message)
	{
	}

	const char* RuntimeException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << '\n' << "[Description] " << m_Message << '\n' << GetDefaultString();

		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}

	RenderException::RenderException(
		const char* message, uint16_t line, const char* file, int result)
		: RuntimeException(message, line, file), m_Error(result)
	{
	}
	RenderException::RenderException(
		const char* message, uint16_t line, const char* file, VkResult result)
		: RuntimeException(message, line, file), m_Error2(result)
	{
	}
	const char* RenderException::what() const noexcept
	{
		if (m_Error)
		{
			std::ostringstream oss;
			oss << GetType() << '\n'
				<< "[Error Code] " << String::Construct(*m_Error) << "(" << (int32_t)*m_Error << ")"
				<< '\n'
				<< "[Description] " << m_Message << '\n'
				<< GetDefaultString();

			m_WhatBuffer = oss.str();
		}
		else
		{
			std::ostringstream oss;
			oss << GetType() << '\n'
				<< "[Error Code] " << String::Construct(*m_Error2) << "(" << (int32_t)*m_Error2
				<< ")" << '\n'
				<< "[Description] " << m_Message << '\n'
				<< GetDefaultString();

			m_WhatBuffer = oss.str();
		}


		return m_WhatBuffer.c_str();
	}
}  // namespace At0::Ray
