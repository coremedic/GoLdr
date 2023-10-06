package types

import (
	"debug/pe"
)

/*
* Define Windows types
 */
type (
	// Win32 BOOL
	BOOL uint32
	// Win32 BOOLEAN
	BOOLEAN byte
	// Win32 BYTE
	BYTE byte
	// 32 bit WORD
	DWORD uint32
	// 64 bit WORD
	DWORD64 uint64
	// Win32 HANDLE
	HANDLE uintptr
	// Win32 LocalHandle
	HLOCAL uintptr
	// int64
	LARGE_INTEGER int64
	// int32
	LONG int32
	// LPVOID pointer
	LPVOID uintptr
	// Win32 SIZE_T
	SIZE_T uintptr
	// Unsigned int32
	UINT uint32
	// Unsigned int pointer
	ULONG_PTR uintptr
	// Unsigned in64
	ULONGLONG uint64
	// 16 bit word
	WORD uint16
)

/*
* IMAGE_FILE_HEADER structure (winnt.h)
* Represents the COFF header format
 */
type ImageFileHeader struct {
	Machine              uint16
	NumberOfSections     uint16
	TimeDateStamp        uint32
	PointerToSymbolTable uint32
	NumberOfSymbols      uint32
	SizeOfOptionalHeader uint16
	Characteristics      uint16
}

/*
* IMAGE_NT_HEADERS64 structure (winnt.h)
* Represents the PE header format (64bit)
 */
type ImageNTHeaders64 struct {
	Signature      uint32
	FileHeader     ImageFileHeader
	OptionalHeader pe.OptionalHeader64
}

/*
* IMAGE_DOS_HEADER structure (winnt.h)
* Header at start of PE file
 */
type ImageDosHeader struct {
	EMagic    uint16
	ECblp     uint16
	ECp       uint16
	ECrlc     uint16
	ECparhdr  uint16
	EMinalloc uint16
	EMaxalloc uint16
	ESs       uint16
	ESp       uint16
	ECsum     uint16
	EIp       uint16
	ECs       uint16
	ELfarlc   uint16
	EOvno     uint16
	ERes      [4]uint16
	EOemid    uint16
	EOeminfo  uint16
	ERes2     [10]uint16
	ELfanew   uint32
}

/*
* IMAGE_EXPORT_DIRECTORY (winnt.h)
* Export directory of PE file
 */
type ImageExportDirectory struct {
	Characteristics       uint32
	TimeDateStamp         uint32
	MajorVersion          uint16
	MinorVersion          uint16
	Name                  uint32
	Base                  uint32
	NumberOfFunctions     uint32
	NumberOfNames         uint32
	AddressOfFunctions    uint32
	AddressOfNames        uint32
	AddressOfNameOrdinals uint32
}

/*
* IMAGE_OPTIONAL_HEADER64 (winnt.h)
* 64 bit optional header
 */
type ImageOptionalHeader64 struct {
	Magic                WORD
	MajorLinkerVersion   BYTE
	MinorLinkerVersion   BYTE
	SizeOfCode           DWORD
	SizeOfInitalizedData DWORD
}