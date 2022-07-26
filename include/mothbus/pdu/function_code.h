#pragma once

#include "../error.h"
#include "../checked_return.h"

namespace mothbus
{
	namespace pdu
	{
		enum class function_code : uint8_t
		{
			read_holding_registers = 0x03
		};

		template <class Reader>
		error_code read(Reader& reader, function_code& v)
		{
			uint8_t h;
			MOTH_CHECKED_RETURN(read(reader, h));
			v = static_cast<function_code>(h);
			return{};
		}

		template <class Writer>
		void write(Writer& writer, const function_code& functionCode)
		{
			writer.write(static_cast<uint8_t>(functionCode));
		}
	}
}
