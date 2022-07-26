#pragma once

#include "../error.h"
#include "../checked_return.h"

namespace mothbus
{
	namespace pdu
	{
		//using pdu_resp = variant<pdu_exception_resp, read_holding_pdu_resp, not_implemented>;

		template <class Resp>
		class pdu_resp
		{
		public:
			Resp& resp;
		};

		template <class Reader, class Response>
		error_code read(Reader& reader, pdu_resp<Response>& resp)
		{
			uint8_t fC;
			MOTH_CHECKED_RETURN(read(reader, fC));
			// 0x80 marks an modbus exception
			if (fC & 0x80)
			{
				pdu_exception_resp exc;
				exc.fc = static_cast<function_code>(fC & 0x7f);
				MOTH_CHECKED_RETURN(read(reader, exc.exceptionCode));
				return make_error_code(exc.exceptionCode);
			}
			function_code function_code_value = static_cast<function_code>(fC);
			if (function_code_value != Response::fc)
				return make_error_code(modbus_exception_code::invalid_response);
			return read(reader, resp.resp);
		}
	}
}
