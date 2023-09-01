// CppBinaryStream - binarystream made in cpp
//
// Copyright (C) 2023  vp817
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

#include <BinaryStream/BinaryStream.h>

using namespace Binary;

int main()
{
	printf("Big Endian:\n");

	auto *stream = new BinaryStream(Buffer::allocateZero(), 0);
	stream->writeUInt8(1);
	stream->writeUInt8(3);
	stream->writeUInt8(2);
	stream->writeUInt8(4);
	stream->writeUInt8(5);
	stream->writeUInt8(6);
	stream->writeUInt8(7);
	stream->writeUInt8(8);
	stream->writeUInt8(9);
	stream->writeInt8(-9);
	stream->writeUInt16(19132);
	stream->writeInt16(0xfffff);
	stream->writeBool(true);
	stream->writeBool(false);
	stream->writeBit(true);
	stream->writeBit(false);
	stream->writeBit(true, true);
	stream->writeBits<uint16_t>(16, 8);
	stream->writeBit(true, true);
	stream->writeUInt24(0xffffff);
	stream->writeInt24(0xffffff);
	stream->writeUInt32(0xffffffff);
	stream->writeInt32(0xffffffff);
	stream->writeUInt64(0xffffffffffffffff);
	stream->writeInt64(0xffffffffffffffff);
	stream->writeFloat(1.113);
	stream->writeDouble(1.119911);
	stream->writeVarInt32(1000);
	stream->writeVarInt64(1000);
	stream->writeZigZag32(1000);
	stream->writeZigZag32(1000);

	stream->padBufferWithZero(1024);

	printf("Unsigned: %d\n", stream->readUInt8());
	printf("Unsigned: %d\n", stream->readUInt8());
	printf("Unsigned: %d\n", stream->readUInt8());
	printf("Unsigned: %d\n", stream->readUInt8());
	printf("Unsigned: %d\n", stream->readUInt8());
	printf("Unsigned: %d\n", stream->readUInt8());
	printf("Unsigned: %d\n", stream->readUInt8());
	printf("Unsigned: %d\n", stream->readUInt8());
	printf("Unsigned: %d\n", stream->readUInt8());
	printf("Signed: %d\n", stream->readInt8());
	printf("Unsigned: %d\n", stream->readUInt16());
	printf("Signed: %d\n", stream->readInt16());
	printf("Bool: %d\n", stream->readBool() ? 1 : 0);
	printf("Bool: %d\n", stream->readBool() ? 1 : 0);
	printf("Bit: %d\n", stream->readBit() ? 1 : 0);
	printf("Bit: %d\n", stream->readBit() ? 1 : 0);
	printf("Bit: %d\n", stream->readBit() ? 1 : 0);
	stream->nullifyBit();
	printf("UInt16 from bits: %d\n", stream->readBits<uint16_t>(8));
	printf("Bit from uint8: %d\n", stream->readBits<uint16_t>(8) == 0x80 ? 1 : 0);
	printf("Unsigned: %u\n", stream->readUInt24());
	printf("Signed: %d\n", stream->readInt24());
	printf("Unsigned: %u\n", stream->readUInt32());
	printf("Signed: %d\n", stream->readInt32());
	printf("Unsigned: %llu\n", stream->readUInt64());
	printf("Signed: %lli\n", stream->readInt64());
	printf("Float: %f\n", stream->readFloat());
	printf("Double: %f\n", stream->readDouble());
	printf("VarInt32: %d\n", stream->readVarInt32());
	printf("VarInt64: %llu\n", stream->readVarInt64());
	printf("ZigZag32: %u\n", stream->readZigZag32());
	printf("ZigZag64: %lli\n", stream->readZigZag64());

	Buffer *remaining = stream->readRemaining();
	printf("PadWithZeroSize: %zu\n", remaining->getSize());
	printf("RemainingFirstIndex: %d\n", remaining->at(0));
	printf("RemainingFourthIndex: %d\n", remaining->at(4));
	printf("RemainingLastIndex: %d\n", remaining->at(1023));

	printf("Old buffer size: %zu\n", stream->getBuffer()->getSize());

	stream->reset(true);

	printf("Little Endian:\n");

	stream->writeUInt16(19132, false);
	stream->writeInt16(0xfffff, false);
	stream->writeBits<uint16_t>(16, 8, false);
	stream->writeUInt24(0xffffff, false);
	stream->writeInt24(0xffffff, false);
	stream->writeUInt32(0xffffffff, false);
	stream->writeInt32(0xffffffff, false);
	stream->writeUInt64(0xffffffffffffffff, false);
	stream->writeInt64(0xffffffffffffffff, false);
	stream->writeFloat(1.113, false);
	stream->writeDouble(1.119911, false);

	printf("Unsigned: %d\n", stream->readUInt16(false));
	printf("Signed: %d\n", stream->readInt16(false));
	printf("UInt16 from bits: %d\n", stream->readBits<uint16_t>(8, false));
	printf("Unsigned: %u\n", stream->readUInt24(false));
	printf("Signed: %d\n", stream->readInt24(false));
	printf("Unsigned: %u\n", stream->readUInt32(false));
	printf("Signed: %d\n", stream->readInt32(false));
	printf("Unsigned: %llu\n", stream->readUInt64(false));
	printf("Signed: %lli\n", stream->readInt64(false));
	printf("Float: %f\n", stream->readFloat(false));
	printf("Double: %f\n", stream->readDouble(false));

	printf("New buffer size: %zu\n", stream->getBuffer()->getSize());

	delete stream;

	return 0;
}
