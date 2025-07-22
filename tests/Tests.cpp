// CppBinaryStream - binary stream c++ library implemention.
//
// Copyright (C) 2025  vp817
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <BMLib/BinaryStream.hpp>

using namespace BMLib;

int main()
{
	printf("Big Endian:\n");

	BinaryStream *stream = new BinaryStream(Buffer::allocate(true,0), 0);
	printf("Bit Related Stuff:\n");
	stream->writeBit(true);
	stream->writeBit(true);
	stream->writeBit(true);
	stream->writeBit(false);
	stream->writeBit(true);
	stream->writeBit(true);
	stream->writeBit(true);
	stream->writeBit(true);

	stream->writeBit(false);
	stream->writeBit(true,true);

	// 0 0 1 1 0 1 1 1 0 -> MSB
	// 0 1 1 1 0 1 1 0 0 -> LSB
	stream->writeBit(false,false,false);
	stream->writeBit(false,false,false);
	stream->writeBit(true,false,false);
	stream->writeBit(false,false,false);
	stream->writeBit(true,false,false);
	stream->writeBit(true,false,false);
	stream->writeBit(true,false,false);
	stream->writeBit(false,false,false);

	printf("Bit value: %d\n", stream->readBit());
	printf("Bit value: %d\n", stream->readBit());
	printf("Bit value: %d\n", stream->readBit());
	printf("Bit value: %d\n", stream->readBit());
	printf("Bit value: %d\n", stream->readBit());
	printf("Bit value: %d\n", stream->readBit());
	printf("Bit value: %d\n", stream->readBit());
	printf("Bit value: %d\n", stream->readBit());

	printf("Bit 2 value: %d\n", stream->readBit());
	printf("Bit 2 value: %d\n", stream->readBit());
	stream->resetBitReader();

	printf("Bit 3 value: %d\n", stream->readBit(false,false));
	printf("Bit 3 value: %d\n", stream->readBit(false,false));
	printf("Bit 3 value: %d\n", stream->readBit(false,false));
	printf("Bit 3 value: %d\n", stream->readBit(false,false));
	printf("Bit 3 value: %d\n", stream->readBit(false,false));
	printf("Bit 3 value: %d\n", stream->readBit(false,false));
	printf("Bit 3 value: %d\n", stream->readBit(false,false));
	printf("Bit 3 value: %d\n", stream->readBit(false,false));

	stream->write<std::uint8_t>(1);
	stream->write<std::uint8_t>(3);
	stream->write<std::uint8_t>(2);
	stream->write<std::uint8_t>(4);
	stream->write<std::uint8_t>(5);
	stream->write<std::uint8_t>(6);
	stream->write<std::uint8_t>(7);
	stream->write<std::uint8_t>(8);
	stream->write<std::uint8_t>(9);
	stream->write<std::int8_t>(-9);
	stream->write<std::uint16_t>(19132);
	stream->write<std::uint16_t>(0xffff);
	stream->write<bool>(true);
	stream->write<bool>(false);
	stream->writeBit(true);
	stream->writeBit(false);
	stream->writeBit(true, true);
	stream->writeBits<std::uint16_t>(16, 8);
	stream->writeBit(true, true);
	stream->write<uint24_t>(0xffffff);
	stream->write<int24_t>(0xffffff);
	stream->write<std::uint32_t>(0xffffffff);
	stream->write<std::int32_t>(0xffffffff);
	stream->write<std::uint64_t>(0xffffffffffffffff);
	stream->write<std::int64_t>(0xffffffffffffffff);
	stream->writeFloat<float>(1.113);
	stream->writeFloat<double>(1.119911);
	stream->writeVarInt<std::uint32_t>(100);
	stream->writeVarInt<std::uint64_t>(1000);
	stream->writeZigZag(100);
	stream->writeZigZag<std::int64_t>(1000);
	stream->writeString<std::uint32_t>("String Test (Not varint)");
	stream->writeStringVarInt<std::uint32_t>("String Test (Varint)");

	stream->writePadding(0, 1024);

	printf("Unsigned: %d\n", stream->read<std::uint8_t>());
	printf("Unsigned: %d\n", stream->read<std::uint8_t>());
	printf("Unsigned: %d\n", stream->read<std::uint8_t>());
	printf("Unsigned: %d\n", stream->read<std::uint8_t>());
	printf("Unsigned: %d\n", stream->read<std::uint8_t>());
	printf("Unsigned: %d\n", stream->read<std::uint8_t>());
	printf("Unsigned: %d\n", stream->read<std::uint8_t>());
	printf("Unsigned: %d\n", stream->read<std::uint8_t>());
	printf("Unsigned: %d\n", stream->read<std::uint8_t>());
	printf("Signed: %d\n", stream->read<std::int8_t>());
	printf("Unsigned: %d\n", stream->read<std::uint16_t>());
	printf("Signed: %d\n", stream->read<std::uint16_t>());
	printf("Bool: %d\n", stream->read<bool>() ? 1 : 0);
	printf("Bool: %d\n", stream->read<bool>() ? 1 : 0);
	printf("Bit: %d\n", stream->readBit() ? 1 : 0);
	printf("Bit: %d\n", stream->readBit() ? 1 : 0);
	printf("Bit: %d\n", stream->readBit() ? 1 : 0);
	stream->resetBitReader();
	printf("UInt16 from bits: %d\n", stream->readBits<std::uint16_t>(8));
	printf("Bit from uint8 is 0x80: %d\n", stream->readBits<std::uint16_t>(8) == 0x80 ? 1 : 0);
	printf("Unsigned: %u\n", static_cast<std::uint32_t>(stream->read<uint24_t>()));
	printf("Signed: %d\n", static_cast<std::int32_t>(stream->read<int24_t>()));
	printf("Unsigned: %u\n", stream->read<std::uint32_t>());
	printf("Signed: %d\n", stream->read<std::int32_t>());
	printf("Unsigned: %lu\n", stream->read<std::uint64_t>());
	printf("Signed: %li\n", stream->read<std::int64_t>());
	printf("Float: %f\n", stream->readFloat<float>());
	printf("Double: %f\n", stream->readFloat<double>());
	printf("VarInt32: %d\n", stream->readVarInt());
	printf("VarInt64: %lu\n", stream->readVarInt<std::uint64_t>());
	printf("ZigZag32: %u\n", stream->readZigZag());
	printf("ZigZag64: %li\n", stream->readZigZag<std::int64_t>());
	printf("String: %s\n", stream->readString<std::uint32_t>().c_str());
	printf("StringVarInt: %s\n", stream->readStringVarInt().c_str());

	Buffer *paddingBuffer = stream->readPadding(0, 1024);
	printf("ZeroPaddingSize: %zu\n", paddingBuffer->getSize());
	printf("RemainingFirstIndex: %d\n", paddingBuffer->at(0));
	printf("RemainingFourthIndex: %d\n", paddingBuffer->at(4));
	printf("RemainingLastIndex: %d\n", paddingBuffer->at(1023));

	printf("Old buffer size: %zu\n", stream->getBuffer()->getSize());

	delete paddingBuffer;

	stream->reset(true);

	printf("Little Endian:\n");

	stream->write<std::uint16_t>(19132, false);
	stream->write<std::uint16_t>(0xffff, false);
	stream->writeBits<std::uint16_t>(16, 8, false);
	stream->write<uint24_t>(0xffffff, false);
	stream->write<int24_t>(0xffffff, false);
	stream->write<std::int32_t>(0xffffffff, false);
	stream->write<std::int32_t>(0xffffffff, false);
	stream->write<std::uint64_t>(0xffffffffffffffff, false);
	stream->write<std::int64_t>(0xffffffffffffffff, false);
	stream->writeFloat<float>(1.113, false);
	stream->writeFloat<double>(1.119911, false);
	stream->writeString<std::uint32_t>("String Test", false);

	printf("Unsigned: %d\n", stream->read<std::uint16_t>(false));
	printf("Signed: %d\n", stream->read<std::uint16_t>(false));
	printf("UInt16 from bits: %d\n", stream->readBits<std::uint16_t>(8, false));
	printf("Unsigned: %u\n", static_cast<std::uint32_t>(stream->read<uint24_t>(false)));
	printf("Signed: %d\n", static_cast<std::int32_t>(stream->read<int24_t>(false)));
	printf("Unsigned: %u\n", stream->read<std::uint32_t>(false));
	printf("Signed: %d\n", stream->read<std::int32_t>(false));
	printf("Unsigned: %lu\n", stream->read<std::uint64_t>(false));
	printf("Signed: %li\n", stream->read<std::int64_t>(false));
	printf("Float: %f\n", stream->readFloat<float>(false));
	printf("Double: %f\n", stream->readFloat<double>(false));
	printf("String: %s\n", stream->readString<std::uint32_t>(false).c_str());

	printf("New buffer size: %zu\n", stream->getBuffer()->getSize());

	stream->reset(true);

	printf("Optional:\n");

	stream->writeOptional([](BinaryStream *stream) {
		stream->writeStringVarInt("Inside of an optional function");
	});

	stream->writeOptional(std::nullopt);

	std::string opt_string;
	stream->readOptional([&](BinaryStream *stream) {
		opt_string = stream->readStringVarInt();
	});

	stream->readOptional([&](BinaryStream *stream) {
		printf("Second optional read (should'nt print)\n");
	});

	printf("OptionalString: %s\n", opt_string.c_str());

	delete stream;

	return 0;
}
