package GoLdr

import (
	"embed"
	"github.com/spf13/pflag"
)

var (
	//go:embed stubs/PE/source/*
	PEStubSrc embed.FS

	//go:embed stubs/PE/include/*
	PEStubInc embed.FS
)

func main() {
	var (
		payload string
		output  string
		dll     bool
		exe     bool
	)

	// Define flags
	pflag.StringVarP(&payload, "payload", "p", "", "Path to payload")
	pflag.StringVarP(&output, "output", "o", "", "Path to output")
	pflag.BoolVar(&exe, "exe", false, "EXE payload output type")
	pflag.BoolVar(&dll, "dll", false, "DLL payload output type")

	// Parse flags
	pflag.Parse()

	if !exe && !dll {
		exe = true
	}

}
