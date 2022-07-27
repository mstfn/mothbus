#pragma once

#include <vector>
#include <mothbus/mothbus.h>
#include "base.h"
#include "function_code.h"
#include "../checked_return.h"

namespace mothbus
{
	namespace pdu
	{
		class write_multiple_reg_pdu_req : public pdu_base<function_code::write_multiple_registers>
		{
		public:
			uint16_t starting_address;
			uint16_t quantity_of_registers;
			uint8_t byte_count;
			std::vector<uint8_t> registers_value;
		};


		template <class Reader>
		error_code read(Reader& reader, write_multiple_reg_pdu_req& req)
		{
			MOTH_CHECKED_RETURN(read(reader, req.starting_address));
			MOTH_CHECKED_RETURN(read(reader, req.quantity_of_registers));
			MOTH_CHECKED_RETURN(read(reader, req.byte_count));
			req.registers_value = std::vector<uint8_t>(req.byte_count);
			MOTH_CHECKED_RETURN(read(reader, req.registers_value));
			return{};
		}


		template <class Writer>
		void write(Writer& writer, const write_multiple_reg_pdu_req& v)
		{
			write(writer, write_multiple_reg_pdu_req::fc);
			writer.write(v.starting_address);
			writer.write(v.quantity_of_registers);
			writer.write(v.byte_count);
			write(writer, v.registers_value);
		}
	}
}
