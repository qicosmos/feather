#pragma once
#include <array>
#include <iterator>
#include <cstdint>

namespace feather {

	class md5 {
	private:
		std::uint32_t a0_;
		std::uint32_t b0_;
		std::uint32_t c0_;
		std::uint32_t d0_;

		std::array<std::uint32_t, 16> m_array_;
		std::array<std::uint32_t, 16>::iterator m_array_first_;

		static const std::array<std::uint32_t, 64> k_array_;
		static const std::array<std::uint32_t, 64> s_array_;

	private:
		static std::uint32_t left_rotate(std::uint32_t x, std::uint32_t c) {
			return (x << c) | (x >> (32 - c));
		}

		template <class OutputIterator>
		static void uint32_to_byte(std::uint32_t n, OutputIterator & first) {

			*first++ = n & 0xff;
			*first++ = (n >> 8) & 0xff;
			*first++ = (n >> 16) & 0xff;
			*first++ = (n >> 24) & 0xff;
		}

		template <class OutputIterator>
		static void uint32_to_hex(std::uint32_t n, OutputIterator & first) {
			const char * hex_chars = "0123456789abcdef";

			std::uint32_t b;

			b = n & 0xff;
			*first++ = hex_chars[b >> 4];
			*first++ = hex_chars[b & 0xf];

			b = (n >> 8) & 0xff;
			*first++ = hex_chars[b >> 4];
			*first++ = hex_chars[b & 0xf];

			b = (n >> 16) & 0xff;
			*first++ = hex_chars[b >> 4];
			*first++ = hex_chars[b & 0xf];

			b = (n >> 24) & 0xff;
			*first++ = hex_chars[b >> 4];
			*first++ = hex_chars[b & 0xf];
		}

	private:
		void reset_m_array() {
			m_array_first_ = m_array_.begin();
		}

		template <class InputIterator>
		void bytes_to_m_array(InputIterator & first, std::array<std::uint32_t, 16>::iterator m_array_last) {
			for (; m_array_first_ != m_array_last; ++m_array_first_) {
				*m_array_first_ = *first++;
				*m_array_first_ |= *first++ << 8;
				*m_array_first_ |= *first++ << 16;
				*m_array_first_ |= *first++ << 24;
			}
		}

		template <class InputIterator>
		void true_bit_to_m_array(InputIterator & first, std::ptrdiff_t chunk_length) {
			switch (chunk_length % 4) {
			case 0:
				*m_array_first_++ = 0x00000080;
				break;
			case 1:
				*m_array_first_++ = *first++;
				*m_array_first_ |= 0x00008000;
				break;
			case 2:
				*m_array_first_++ = *first++;
				*m_array_first_ |= *first++ << 8;
				*m_array_first_ |= 0x00800000;
				break;
			case 3:
				*m_array_first_++ = *first++;
				*m_array_first_ |= *first++ << 8;
				*m_array_first_ |= *first++ << 16;
				*m_array_first_ |= 0x80000000;
				break;
			}
		}

		void zeros_to_m_array(std::array<std::uint32_t, 16>::iterator m_array_last) {
			for (; m_array_first_ != m_array_last; ++m_array_first_) {
				*m_array_first_ = 0;
			}
		}

		void original_length_bits_to_m_array(std::uint64_t original_length_bits) {
			original_length_bits &= 0xffffffffffffffff;
			*m_array_first_++ = (original_length_bits) & 0x00000000ffffffff;
			*m_array_first_++ = (original_length_bits & 0xffffffff00000000) >> 32;
		}

		void hash_chunk() {
			std::uint32_t A = a0_;
			std::uint32_t B = b0_;
			std::uint32_t C = c0_;
			std::uint32_t D = d0_;

			std::uint32_t F;
			unsigned int g;

			for (unsigned int i = 0; i < 64; ++i) {
				if (i < 16) {
					F = (B & C) | ((~B) & D);
					g = i;
				}
				else if (i < 32) {
					F = (D & B) | ((~D) & C);
					g = (5 * i + 1) & 0xf;
				}
				else if (i < 48) {
					F = B ^ C ^ D;
					g = (3 * i + 5) & 0xf;
				}
				else {
					F = C ^ (B | (~D));
					g = (7 * i) & 0xf;
				}

				std::uint32_t D_temp = D;
				D = C;
				C = B;
				B += left_rotate(A + F + k_array_[i] + m_array_[g], s_array_[i]);
				A = D_temp;
			}

			a0_ += A;
			b0_ += B;
			c0_ += C;
			d0_ += D;
		}

	public:
		template <class InputIterator>
		void update(InputIterator first, InputIterator last) {

			std::uint64_t original_length_bits = std::distance(first, last) * 8;

			std::ptrdiff_t chunk_length;
			while ((chunk_length = std::distance(first, last)) >= 64) {
				reset_m_array();
				bytes_to_m_array(first, m_array_.end());
				hash_chunk();
			}

			reset_m_array();
			bytes_to_m_array(first, m_array_.begin() + chunk_length / 4);
			true_bit_to_m_array(first, chunk_length);

			if (chunk_length >= 56) {
				zeros_to_m_array(m_array_.end());
				hash_chunk();

				reset_m_array();
				zeros_to_m_array(m_array_.end() - 2);
				original_length_bits_to_m_array(original_length_bits);
				hash_chunk();
			}
			else {
				zeros_to_m_array(m_array_.end() - 2);
				original_length_bits_to_m_array(original_length_bits);
				hash_chunk();
			}
		}

	public:
		md5()
			: a0_(0x67452301),
			b0_(0xefcdab89),
			c0_(0x98badcfe),
			d0_(0x10325476)
		{}

		template <class Container>
		void digest(Container & container) {
			container.resize(16);
			auto it = container.begin();

			uint32_to_byte(a0_, it);
			uint32_to_byte(b0_, it);
			uint32_to_byte(c0_, it);
			uint32_to_byte(d0_, it);
		}

		template <class Container>
		void hex_digest(Container & container) {
			container.resize(32);
			auto it = container.begin();

			uint32_to_hex(a0_, it);
			uint32_to_hex(b0_, it);
			uint32_to_hex(c0_, it);
			uint32_to_hex(d0_, it);
		}
	};

	const std::array<std::uint32_t, 64> md5::k_array_ = {
		0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
		0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
		0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
		0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
		0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
		0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
		0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
		0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
		0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
		0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
		0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
		0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
		0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
		0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
		0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
		0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
	};

	const std::array<std::uint32_t, 64> md5::s_array_ = {
		7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
		5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
		4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
		6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
	};
}