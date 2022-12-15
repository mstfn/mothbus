#pragma once

#include <mothbus/mothbus.h>

namespace mothbus
{
	namespace pdu
	{
		template <class Writer, class C>
		void write(Writer& writer, const C& v);

		template <class Sink>
		class writer
		{
		public:
			writer(Sink& sink)
				: _sink(sink)
			{
			};

			inline void write(uint16_t v)
			{
				_sink.put((v >> 8) & 0xff);
				_sink.put(v & 0xff);
			}

			inline void write(uint8_t v)
			{
				_sink.put(v);
			}


		private:
			Sink& _sink;
		};

		template <class Writer>
		void write(Writer& writer, uint8_t v)
		{
			writer.write(v);
		}

		template <class Writer>
		void write(Writer& writer, uint16_t v)
		{
			writer.write(v);
		}

		template <class Writer>//, std::ptrdiff_t Extent>
		inline void write(Writer& writer, const span<byte>& v)
		{
			for (auto& byte : v)
			{
				writer.write(mothbus::to_integer<uint8_t>(byte));
			}
		}

		template <class Writer>//, std::ptrdiff_t Extent>
		inline void write(Writer& writer, const std::vector<byte>& v)
		{
			for (auto& byte : v)
			{
				writer.write(gsl::to_integer<uint8_t>(byte));
			}
		}

		template <class Writer>//, std::ptrdiff_t Extent>
		inline void write(Writer& writer, const span<uint8_t>& v)
		{
			for (auto& byte : v)
			{
				writer.write(byte);
			}
		}

		template <class Writer>//, std::ptrdiff_t Extent>
		inline void write(Writer& writer, const std::vector<uint8_t>& v)
		{
			for (auto& byte : v)
			{
				writer.write(byte);
			}
		}

		template <class Writer>
		void write(Writer& writer, const modbus_exception_code& v)
		{
			write(writer, static_cast<uint8_t>(v));
		}

	}
}

