#pragma once

#include <mothbus/mothbus.h>
#include "req_reading_register.h"
#include "../checked_return.h"

namespace mothbus
{
	namespace pdu
	{
		class not_implemented
		{
		public:
			uint8_t fc = 0;
		};

		using pdu_req = variant<read_holding_pdu_req, not_implemented>;


		namespace detail
		{
			template <class Head, class ...Tail, class Reader>
			typename std::enable_if<std::is_same<Head, not_implemented>::value, error_code>::type
				read_pdu_variant(Reader& reader, pdu_req& resp, function_code functionCode)
			{
				resp = not_implemented{};
				return make_error_code(modbus_exception_code::illegal_function);
			}

			template <class Head, class ...Tail, class Reader>
			typename std::enable_if<!std::is_same<Head, not_implemented>::value, error_code>::type
				read_pdu_variant(Reader& reader, pdu_req& resp, function_code functionCode)
			{
				if (Head::fc == functionCode)
				{
					Head real{};
					MOTH_CHECKED_RETURN(read(reader, real));
					resp = real;
					return{};
				}
				return read_pdu_variant<Tail...>(reader, resp, functionCode);
			}

			template <class Reader, class ...t>
			error_code read_pdu_req(Reader& reader, variant<t...>& resp, function_code functionCode)
			{
				return read_pdu_variant<t...>(reader, resp, functionCode);
			}
		}

		template <class Reader>
		error_code read(Reader& reader, pdu_req& req)
		{
			function_code functionCode;
			MOTH_CHECKED_RETURN(read(reader, functionCode));
			return detail::read_pdu_req(reader, req, functionCode);
		}
	}
}
