#pragma once

#include "function_code.h"

namespace mothbus
{
	namespace pdu
	{
		template <function_code FunctionCode>
		class pdu_base
		{
		public:
			constexpr static function_code fc = FunctionCode;
		};

		template<function_code FunctionCode>
		constexpr function_code pdu_base<FunctionCode>::fc;
	}
}
