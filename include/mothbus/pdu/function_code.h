#pragma once

#include "../error.h"
#include "../checked_return.h"

namespace mothbus
{
	namespace pdu
	{
		enum class function_code : uint8_t
		{
			// read_coils = 0x01,
			// read_discrete_inputs = 0x02,
			read_holding_registers = 0x03,
			// read_input_registers = 0x04,
			// write_single_coil = 0x05,
			// write_single_register = 0x06,
			// read_exception_status = 0x07,
			// write_multiple_coils = 0xf,
			write_multiple_registers = 0x10 //,
			// report_slave_id = 0x11,
			// mask_write_register = 0x16,
			// write_and_read_registers = 0x17
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
