#pragma once

#include <cstdint>
#include "function_code.h"

namespace mothbus
{
	namespace pdu
	{

		class pdu_exception_resp
		{
		public:
			function_code fc;
			modbus_exception_code exceptionCode;
		};

		template <class Writer>
		void write(Writer& writer, const pdu_exception_resp& v)
		{
			uint8_t error_function_code = static_cast<uint8_t>(v.fc);
			error_function_code |= 0x80;
			write(writer, error_function_code);
			write(writer, v.exceptionCode);
		}

	}
}
