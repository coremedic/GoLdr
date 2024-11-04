package util

import (
	"embed"
	"log/slog"
	"os"
	"os/exec"
	"runtime"
	"strings"
)

// FindMinGWPath checks if x86_64-w64-mingw32-g++ is installed and returns its path
func FindMinGWPath() (string, error) {
	var cmd *exec.Cmd
	switch runtime.GOOS {
	case "windows":
		{
			cmd = exec.Command("where", "x86_64-w64-mingw32-g++.exe")
		}
	case "linux":
		{
			cmd = exec.Command("which", "x86_64-w64-mingw32-g++")
		}
	default:
		{
			cmd = exec.Command("which", "x86_64-w64-mingw32-g++")
		}
	}

	ret, err := cmd.Output()
	if err != nil {
		slog.Error("Failed to find MinGW path", "error", err)
		return "", err
	}

	// Clean and return first line of output
	path := strings.TrimSpace(string(ret))
	return path, nil
}

// ExtractEmbed extracts files from an embed.FS directory
func ExtractEmbed(efs embed.FS, embedPath, extractPath string) error {
	// Check if output dir exists
	if err := os.MkdirAll(extractPath, os.ModePerm); err != nil {
		slog.Error("Failed to create extraction directory", "error", err)
		return err
	}

}
