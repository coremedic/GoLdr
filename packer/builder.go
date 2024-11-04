package packer

import (
	"embed"
	"log/slog"
	"os"
	"path/filepath"
)

// OutputType enum
type outputType int

const (
	EXE outputType = iota // EXE output type
	DLL                   // DLL output type
	PIC                   // Position independent code output type
)

// Builder
//
// packer.Builder represents configuration options for the packer builder
type Builder struct {
	sourcePath string // Source code path
	buildPath  string
	OutputPath string
	OutputType outputType

	compiler struct {
		MinGW64Path string // Path to MinGW-w64 compiler
		MinGW86Path string // Reserved for future use
		JWasmPath   string // Path to JWasm assembler

		SourcePaths    []string // Paths to C source files
		IncludePaths   []string // Paths to C header files
		AsmSourcePaths []string // Paths to asm source files
		Flags          []string // Flags to pass to MinGW
		Defines        []string // Defines to pass to MinGW
	}

	packer struct {
		PayloadPath string
		Sgn         bool
		Crypt       bool
		LZMA        bool
	}
}

// NewBuilder creates a new builder instance
func NewBuilder(embeddedSrc, embeddedInc embed.FS) (*Builder, error) {
	var (
		builder = new(Builder)
		err     error
	)

	// Make temporary build directory
	builder.buildPath, err = os.MkdirTemp("", "build")
	if err != nil {
		slog.Error("Failed to create build directory", "error", err)
		return nil, err
	}

	// Extract embedded source files to build directory
	if err := os.MkdirAll(filepath.Join(builder.buildPath, "source"), os.ModePerm); err != nil {

	}
}

// Close cleans up builder files on disk
func (b *Builder) Close() error {
	return os.RemoveAll(b.buildPath)
}
