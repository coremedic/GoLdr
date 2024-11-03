package GoLdr

import (
	"GoLdr/pack"
	_ "embed"
	"github.com/spf13/pflag"
)

func main() {
	var (
		payload   string
		output    string
		shellcode bool
		dll       bool
		exe       bool
	)

	// Define flags
	pflag.StringVarP(&payload, "payload", "p", "", "Path to payload")
	pflag.StringVarP(&output, "output", "o", "", "Path to output")
	pflag.BoolVar(&shellcode, "shellcode", false, "Shellcode payload input type")
	pflag.BoolVar(&dll, "dll", false, "DLL payload input type")
	pflag.BoolVar(&exe, "exe", false, "EXE payload input type")

	// Parse flags
	pflag.Parse()

	// Init options
	var opts *pack.Options

	switch {
	case shellcode:
		{
			// Pack shellcode
		}

	case dll:
		{
			// Pack DLL
		}

	case exe:
		{
			// Pack EXE
		}

	default:
		{
			// Display error
		}

	}
}
