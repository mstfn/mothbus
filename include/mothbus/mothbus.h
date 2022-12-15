#pragma once

#include <boost/variant.hpp>
#ifdef USE_GSL
//#include <gsl/span>
#include <gsl/gsl>
#else // USE_GSL
#include <cstddef>
#include <tcb/span.hpp>
#endif // USE_GSL
#include <boost/system/system_error.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

namespace mothbus
{
	template<class ...T>
	using variant = boost::variant<T...>;

#ifdef USE_GSL
	template<class ...T>
	using span = gsl::span<T...>;

	using byte = gsl::byte;

	template<class ...T>
	auto to_integer = &gsl::to_integer<T...>;

	template <typename T>
	constexpr byte to_byte(T t) noexcept
	{
		return gsl::to_byte<T>(t);
	}

	template <int I>
	constexpr byte to_byte() noexcept
	{
		return gsl::to_byte<I>();
	}
#else // USE_GSL
	template<class ...T>
	using span = tcb::span<T...>;

	using byte = std::byte;

	template<class ...T>
	auto to_integer = &std::to_integer<T...>;

	template <typename T>
	constexpr byte to_byte(T t) noexcept
	{
		return byte{t};
	}

	template <int I>
	constexpr byte to_byte() noexcept
	{
		return byte{I};
	}
#endif // USE_GSL

	template <typename SyncWriteStream, typename ConstBufferSequence>
	inline std::size_t write(SyncWriteStream& s, const ConstBufferSequence& buffers)
	{
		return boost::asio::write(s, buffers);
	}

	template <typename SyncWriteStream, typename ConstBufferSequence, typename ErrorCode>
	inline std::size_t write(SyncWriteStream& s, const ConstBufferSequence& buffers, ErrorCode& ec)
	{
		return boost::asio::write(s, buffers, ec);
	}

	template <typename SyncReadStream, typename MutableBufferSequence>
	inline std::size_t read(SyncReadStream& s, const MutableBufferSequence& buffers)
	{
		return boost::asio::read(s, buffers);
	}

	template <typename SyncReadStream, typename MutableBufferSequence, typename ErrorCode>
	inline std::size_t read(SyncReadStream& s, const MutableBufferSequence& buffers,  ErrorCode& ec)
	{
		return boost::asio::read(s, buffers, ec);
	}

	template <typename SyncReadStream, typename MutableBufferSequence, typename ReadHandler>
	inline void async_read(SyncReadStream& s, const MutableBufferSequence& buffers, ReadHandler&& handler)
	{
		boost::asio::async_read(s, buffers, std::forward<ReadHandler>(handler));
	}

	class modbus_exception// : public std::runtime_error
	{
	public:
		modbus_exception(int error_code):
		error_code(error_code)
		{}

		int error_code;
	};
}
