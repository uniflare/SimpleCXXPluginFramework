// Copyright 2023 Jeremy Wade
// 
// Portions of this file are Copyright Alexander Bessonov & Tobias Loew.
// Please refer to the LICENSE.md file distributed with this source code.
#pragma once
#include "Platform.h"

#include <stdexcept>
#include <string>
#include <cassert>
#include <cstdint>
#include <iomanip>

struct SGuid
{
	uint32_t Data1;
	uint16_t Data2;
	uint16_t Data3;
	uint8_t Data4[8];
};

inline bool operator==(const SGuid& left, const SGuid& right)
{
	return std::memcmp(&left, &right, sizeof(SGuid)) == 0;
}

inline bool operator!=(const SGuid& left, const SGuid& right)
{
	return !(left == right);
}

inline bool operator<(const SGuid & left, const SGuid & right)
{
	const uint64_t& llo = *reinterpret_cast<const uint64_t*>(&left);
	const uint64_t& lhi = *(reinterpret_cast<const uint64_t*>(&left) + 1);

	const uint64_t& rlo = *reinterpret_cast<const uint64_t*>(&right);
	const uint64_t& rhi = *(reinterpret_cast<const uint64_t*>(&right) + 1);

	return llo < rlo || (llo == rlo && lhi < rhi);
}

inline SGuid GenerateGuid()
{
	assert(sizeof(SGuid) == sizeof(GUID));

	SGuid guid;
	(void)CoCreateGuid((GUID*)&guid);
	return guid;
}

// overload << so that it's easy to convert to a string
inline std::ostream& operator<<(std::ostream& s, const SGuid& guid)
{
	//const char hex_chars[] = "0123456789ABCDEF";
	std::ios_base::fmtflags f(s.flags()); // politely don't leave the ostream in hex mode

	const char* guid_s = reinterpret_cast<const char*>(&guid);
	s << std::hex << std::setfill('0')
		<< std::setw(8) << (int)guid.Data1
		<< '-'
		<< std::setw(4) << (int)guid.Data2
		<< '-'
		<< std::setw(4) << (int)guid.Data3
		<< '-'
		<< std::setw(2) << (int)guid.Data4[0]
		<< std::setw(2) << (int)guid.Data4[1]
		<< '-'
		<< std::setw(2) << (int)guid.Data4[2]
		<< std::setw(2) << (int)guid.Data4[3]
		<< std::setw(2) << (int)guid.Data4[4]
		<< std::setw(2) << (int)guid.Data4[5]
		<< std::setw(2) << (int)guid.Data4[6]
		<< std::setw(2) << (int)guid.Data4[7];
	s.flags(f);
	return s;
}

namespace guid_parse
{
	namespace details
	{
		constexpr const size_t short_guid_form_length = 36;	// XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
		constexpr const size_t long_guid_form_length = 38;	// {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}

		//
		constexpr uint8_t parse_hex_digit(const char c)
		{
			using namespace std::string_literals;
			return
				('0' <= c && c <= '9')
				? c - '0'
				: ('a' <= c && c <= 'f')
				? 10 + c - 'a'
				: ('A' <= c && c <= 'F')
				? 10 + c - 'A'
				:
				throw std::domain_error{ "invalid character in GUID"s };
		}

		constexpr uint8_t parse_hex_uint8_t(const char* ptr)
		{
			return (parse_hex_digit(ptr[0]) << 4) + parse_hex_digit(ptr[1]);
		}

		constexpr uint16_t parse_hex_uint16_t(const char* ptr)
		{
			return (parse_hex_uint8_t(ptr) << 8) + parse_hex_uint8_t(ptr + 2);
		}

		constexpr uint32_t parse_hex_uint32_t(const char* ptr)
		{
			return (parse_hex_uint16_t(ptr) << 16) + parse_hex_uint16_t(ptr + 4);
		}

		constexpr SGuid parse_guid(const char* begin)
		{
			return SGuid{
				parse_hex_uint32_t(begin),
				parse_hex_uint16_t(begin + 8 + 1),
				parse_hex_uint16_t(begin + 8 + 1 + 4 + 1),
				{
					parse_hex_uint8_t(begin + 8 + 1 + 4 + 1 + 4 + 1),
					parse_hex_uint8_t(begin + 8 + 1 + 4 + 1 + 4 + 1 + 2),
					parse_hex_uint8_t(begin + 8 + 1 + 4 + 1 + 4 + 1 + 2 + 2 + 1),
					parse_hex_uint8_t(begin + 8 + 1 + 4 + 1 + 4 + 1 + 2 + 2 + 1 + 2),
					parse_hex_uint8_t(begin + 8 + 1 + 4 + 1 + 4 + 1 + 2 + 2 + 1 + 2 + 2),
					parse_hex_uint8_t(begin + 8 + 1 + 4 + 1 + 4 + 1 + 2 + 2 + 1 + 2 + 2 + 2),
					parse_hex_uint8_t(begin + 8 + 1 + 4 + 1 + 4 + 1 + 2 + 2 + 1 + 2 + 2 + 2 + 2),
					parse_hex_uint8_t(begin + 8 + 1 + 4 + 1 + 4 + 1 + 2 + 2 + 1 + 2 + 2 + 2 + 2 + 2)
				}

			};
		}

		constexpr SGuid make_guid_helper(const char* str, size_t N)
		{
			using namespace std::string_literals;
			using namespace details;

			return (!(N == long_guid_form_length || N == short_guid_form_length))
				? throw std::domain_error{ "String GUID of the form {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX} or XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX is expected"s }
				: (N == long_guid_form_length && (str[0] != '{' || str[long_guid_form_length - 1] != '}'))
				? throw std::domain_error{ "Missing opening or closing brace"s }

			: parse_guid(str + (N == long_guid_form_length ? 1 : 0));
		}


		template<size_t N>
		constexpr SGuid make_guid(const char(&str)[N])
		{
			return make_guid_helper(str, N - 1);
		}
	}
	using details::make_guid;

	namespace literals
	{
		constexpr SGuid operator "" _guid(const char* str, size_t N)
		{
			using namespace details;
			return make_guid_helper(str, N);
		}
	}
}

using namespace guid_parse::literals;

inline constexpr const SGuid EmptyGuid("{00000000-0000-0000-0000-000000000000}"_guid);