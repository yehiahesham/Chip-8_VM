#pragma once
#ifndef CHIP8CPU_H_INCLUDED
#define CHIP8CPU_H_INCLUDED

#include <string>
#include <vector>

typedef unsigned char BYTE ;
typedef unsigned short int WORD ;

const int ROMSIZE = 0xFFF ;

class Chip8CPU
{
  public:
                        ~Chip8CPU			( ) ;

    static  Chip8CPU*   CreateSingleton		( ) ;

            bool        LoadRom				(const std::string& romname) ;
			void		ExecuteNextOpcode	( ) ;
			void		DecreaseTimers		( ) ;
			void		KeyPressed			( int key ) ;
			void		KeyReleased			( int key ) ;


			// for some strange reason glDrawPixels needs to be set up as y axis then x axis
			BYTE		m_ScreenData[320][640][3] ; 
  private:
                        Chip8CPU			( ) ;

			WORD		GetNextOpcode		( ) ;
			void		PlayBeep			( ) ;
			int			GetKeyPressed		( ) ;

            void        CPUReset			( ) ;
			void		ClearScreen			( ) ;

			void		DecodeOpcode0		( WORD opcode ) ;
			void		DecodeOpcode8		( WORD opcode ) ;
			void		DecodeOpcodeE		( WORD opcode ) ;
			void		DecodeOpcodeF		( WORD opcode ) ;

			void		Opcode00EE			( ) ;
			void		Opcode1NNN			( WORD opcode ) ;
			void		Opcode2NNN			( WORD opcode ) ;
			void		Opcode3XNN			( WORD opcode ) ;
			void		Opcode4XNN			( WORD opcode ) ;
			void		Opcode5XY0			( WORD opcode ) ;
			void		Opcode6XNN			( WORD opcode ) ;
			void		Opcode7XNN			( WORD opcode ) ;
			void		Opcode8XY0			( WORD opcode ) ;
			void		Opcode8XY1			( WORD opcode ) ;
			void		Opcode8XY2			( WORD opcode ) ;
			void		Opcode8XY3			( WORD opcode ) ;
			void		Opcode8XY4			( WORD opcode ) ;
			void		Opcode8XY5			( WORD opcode ) ;
			void		Opcode8XY6			( WORD opcode ) ;
			void		Opcode8XY7			( WORD opcode ) ;
			void		Opcode8XYE			( WORD opcode ) ;
			void		Opcode9XY0			( WORD opcode ) ;
			void		OpcodeANNN			( WORD opcode ) ;
			void		OpcodeBNNN			( WORD opcode ) ;
			void		OpcodeCXNN			( WORD opcode ) ;
			void		OpcodeDXYN			( WORD opcode ) ;
			void		OpcodeEX9E			( WORD opcode ) ;
			void		OpcodeEXA1			( WORD opcode ) ;
			void		OpcodeFX07			( WORD opcode ) ;
			void		OpcodeFX0A			( WORD opcode ) ;
			void		OpcodeFX15			( WORD opcode ) ;
			void		OpcodeFX18			( WORD opcode ) ;
			void		OpcodeFX1E			( WORD opcode ) ;
			void		OpcodeFX29			( WORD opcode ) ;
			void		OpcodeFX33			( WORD opcode ) ;
			void		OpcodeFX55			( WORD opcode ) ;
			void		OpcodeFX65			( WORD opcode ) ;

    static  Chip8CPU*   m_Instance ;

            BYTE        m_GameMemory[ROMSIZE] ;
            BYTE        m_Registers[16] ;
            WORD        m_AddressI ;
            WORD        m_ProgramCounter ;

			std::vector<WORD> m_Stack ;
			BYTE		m_KeyState[16] ;
			BYTE		m_DelayTimer ;
			BYTE		m_SoundTimer ;
};


#endif // CHIP8CPU_H_INCLUDED
