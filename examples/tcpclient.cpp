#include <iostream>
#include <iomanip>
#include <boost/program_options.hpp>
#include <boost/asio.hpp>
#include <mothbus/adu/tcp.h>

namespace po = boost::program_options;

using byte_vector = std::vector<mothbus::byte>;
using byte_count_t = byte_vector::size_type;
using reg_count_t = byte_count_t;

std::ostream& operator<<(std::ostream& os, const gsl::span<gsl::byte>& bytes) 
{
	for (auto byte : bytes)
		os << std::hex << std::setw(2) << std::setfill('0') << gsl::to_integer<int>(byte);
	return os;
}

int main(int argc, char** argv)
{
	po::options_description cmdline_options;
	po::options_description desc("Allowed options");

	int port;
	reg_count_t length;
	std::string host = "localhost";
	uint16_t register_address;
	uint16_t slave;

	desc.add_options()
		("help", "produce help message")
		("port,p", po::value<int>(&port)->default_value(502), "tcp port (default 502)")
		("length,l", po::value<reg_count_t>(&length)->default_value(1), "number of registers (default 1)")
		("address,a", po::value<uint16_t>(&register_address)->default_value(1), "register address")
		("slave,s", po::value<uint16_t>(&slave)->default_value(255), "slave id")
		("host", po::value<std::string>(), "host")
		;
	cmdline_options.add(desc);

	po::positional_options_description p;
	p.add("host", -1);
	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(cmdline_options).positional(p).run(), vm);
	po::notify(vm);

	if (vm.count("help") || vm.count("host") == 0) {
		std::cout << cmdline_options;
		return 0;
	}

	if (vm.count("host"))
		host = vm["host"].as<std::string>();

	using boost::asio::ip::tcp;
	boost::asio::io_service io_service;
	tcp::resolver resolver(io_service);
	tcp::resolver::query query(host, std::to_string(port));
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	tcp::socket socket(io_service);
	boost::asio::connect(socket, endpoint_iterator);

	mothbus::tcp_master<tcp::socket> client(socket);
	byte_count_t byte_length = 2 * length;
	byte_vector registers { byte_length };
	auto ec = client.read_registers(slave, register_address, registers);

	std::cout << "Host: " << host << ":" << port << "\n";
	std::cout << "register address: " << register_address << " amount: " << length << "\n";
	std::cout << "---------------\n";
	if (!ec)
		std::cout << "    value: " << registers << "\n";
	else
		std::cout << "error: " << ec.message() << "\n";
	
	return 0;
}