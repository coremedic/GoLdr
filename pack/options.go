package pack

// Options struct
//
// Represents various options for the packer
type Options struct {
	Payload    string     // Path to payload
	OutputType OutputType // Packed payload output type
	Sgn        bool       // Use Shikata ga nai encoder
}

// OutputType enum
type OutputType int

const (
	EXE OutputType = iota // EXE output type
	DLL                   // DLL output type
	PIC                   // Position independent code output type
)
