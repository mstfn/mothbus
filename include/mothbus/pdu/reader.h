#pragma once

#include <cstdint>
#include "../error.h"
#include "../checked_return.h"
#include <mothbus/mothbus.h>

namespace mothbus
{
	namespace pdu
	{
		template <class Reader, class C>
		error_code read(Reader& reader, C& v);

		template <class Reader>
		error_code read(Reader& reader, uint8_t& v)
		{
			reader.get(v);
			return{};
		}

		template <class Reader>
		error_code read(Reader& reader, uint16_t& v)
		{
			uint8_t b = 0;
			MOTH_CHECKED_RETURN(read(reader, b));
			v = b << 8;
			MOTH_CHECKED_RETURN(read(reader, b));
			v |= b;
			return{};
		}


		template <class Reader>
		error_code read(Reader& reader, span<byte>& v)
		{
			for (auto& byte : v)
			{
				uint8_t temp = 0;
				MOTH_CHECKED_RETURN(read(reader, temp));
				byte = gsl::to_byte(temp);
			}
			return{};
		}

		template <class Reader>
		error_code read(Reader& reader, std::vector<byte>& v)
		{
			for (auto& byte : v)
			{
				uint8_t temp = 0;
				MOTH_CHECKED_RETURN(read(reader, temp));
				byte = gsl::to_byte(temp);
			}
			return{};
		}

		template <class Reader>
		error_code read(Reader& reader, span<uint8_t>& v)
		{
			for (auto& byte : v)
			{
				MOTH_CHECKED_RETURN(read(reader, byte));
			}
			return{};
		}

		template <class Reader>
		error_code read(Reader& reader, std::vector<uint8_t>& v)
		{
			for (auto& byte : v)
			{
				MOTH_CHECKED_RETURN(read(reader, byte));
			}
			return{};
		}

		template <class Reader>
		error_code read(Reader& reader, modbus_exception_code& v)
		{
			uint8_t h;
			MOTH_CHECKED_RETURN(read(reader, h));
			v = static_cast<modbus_exception_code>(h);
			return{};
		}

	}
}
