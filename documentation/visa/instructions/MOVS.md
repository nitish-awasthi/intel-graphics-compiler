 

## Opcode

  MOVS = 0x2d

## Format

| | | |
| --- | --- | --- |
| 0x2d(MOVS) | Exec_size | Dst | Src0 |


## Semantics




                    for (i = 0; < exec_size; ++i) {
                      if (ChEn[i]) {
                        dst[i] = src0[i];
                      }
                    }

## Description


    Move a state variable from <src0> to <dst>

- **Exec_size(ub):** Execution size
 
  - Bit[2..0]: size of the region for source and destination operands
 
    - 0b000:  1 element (scalar) 
    - 0b001:  2 elements 
    - 0b010:  4 elements 
    - 0b011:  8 elements 
    - 0b100:  16 elements 
    - 0b101:  32 elements 
  - Bit[7..4]: execution mask (explicit control over the enabled channels)
 
    - 0b0000:  M1 
    - 0b0001:  M2 
    - 0b0010:  M3 
    - 0b0011:  M4 
    - 0b0100:  M5 
    - 0b0101:  M6 
    - 0b0110:  M7 
    - 0b0111:  M8 
    - 0b1000:  M1_NM 
    - 0b1001:  M2_NM 
    - 0b1010:  M3_NM 
    - 0b1011:  M4_NM 
    - 0b1100:  M5_NM 
    - 0b1101:  M6_NM 
    - 0b1110:  M7_NM 
    - 0b1111:  M8_NM
- **Dst(vec_operand):** The destination operand. Operand class: state,general

- **Src0(vec_operand):** The first source operand. Operand class: general,indirect,immediate

#### Properties


## Text
```
    

		MOVS (<exec_size>) <dst> <src0>
```



## Notes



    - **Supported Type:** UD (for general and immediate operand).
        - If Dst and Src0 are both state operands, they must be variables of the same storage class.
        - If Dst is a general operand, Src0 must be a state operand; the index value representing the state variable will be copied to the Dst.
        - If Src0 is a general, indirect, or an immediate operand, Dst must be a state operand; the value in Src0 will be interpreted as the index value for the state variable in Dst.
        - In each of the above scenario, <exec_size> number of contiguous elements will be copied from Src0 to Dst starting from their respective starting offsets.
        - Neither saturation nor predication is supported for this instruction.