#pragma once
#include "Emu/CPU/CPUDecoder.h"
#include "ARMv7Opcodes.h"


class ARMv7Decoder : public CPUDecoder
{
	ARMv7Opcodes& m_op;
	u8 m_last_instr_size;

public:
	ARMv7Decoder(ARMv7Opcodes& op) : m_op(op)
	{
	}

	virtual u8 DecodeMemory(const u32 address)
	{
		const u32 code0 = vm::psv::read16(address & ~1);
		const u32 code1 = vm::psv::read16(address + 2 & ~1);
		const u32 data = code0 << 16 | code1;

		for (auto& opcode : ARMv7_opcode_table)
		{
			if ((opcode.type >= A1) == ((address & 1) == 0) && (data & opcode.mask) == opcode.code)
			{
				(m_op.*opcode.func)(opcode.length == 2 ? code0 : data, opcode.type);
				return opcode.length;
			}
		}

		m_op.UNK(data);
		return 2;
	}
};
