#pragma once

#include <mothbus/mothbus.h>
#include "base.h"
#include "../checked_return.h"

namespace mothbus
{
	namespace pdu
	{
		//template<std::ptrdiff_t Extent>
		class write_multiple_reg_pdu_resp : public pdu_base<function_code::write_multiple_registers>
		{
		public:
			write_multiple_reg_pdu_resp(uint16_t starting_address, uint16_t quantity_of_registers)
				: starting_address(starting_address)
				, quantity_of_registers(quantity_of_registers)
			{
			}

			uint16_t starting_address;
			uint16_t quantity_of_registers;
		};


		template <class Reader>//, std::ptrdiff_t Extent>
		error_code read(Reader& reader, write_multiple_reg_pdu_resp& resp)
		{
			MOTH_CHECKED_RETURN(read(reader, resp.starting_address));
			MOTH_CHECKED_RETURN(read(reader, resp.quantity_of_registers));
			return{};
		}

		template <class Writer>//, std::ptrdiff_t Extent>
		void write(Writer& writer, const write_multiple_reg_pdu_resp& v)
		{
			write(writer, write_multiple_reg_pdu_resp::fc);
			write(writer, v.starting_address);
			write(writer, v.quantity_of_registers);
		}
	}
}
