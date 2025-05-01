package main

import (
	"fmt"
	"log"
	"os"
	"os/exec"
	"path/filepath"
	"strings"
)

func main() {
	features := "Features"
	libs := "Libs"
	mainSrc := "main.cpp"
	buildDir := "build"
	objFiles := []string{}

	_ = os.MkdirAll(buildDir, 0755)
	_ = os.MkdirAll(filepath.Join(buildDir, "Features"), 0755)
	_ = os.MkdirAll(filepath.Join(buildDir, "Libs"), 0755)

	featureFiles, _ := filepath.Glob(filepath.Join(features, "*.cpp"))
	libFiles, _ := filepath.Glob(filepath.Join(libs, "*.cpp"))

	for _, src := range featureFiles {
		base := strings.TrimSuffix(filepath.Base(src), ".cpp")
		objName := filepath.Join(buildDir, "Features", base+".o")
		depName := filepath.Join(buildDir, "Features", base+".d")
		runCommand("g++",
			"-std=c++17", "-c", src,
			"-o", objName,
			"-MMD", "-MF", depName,
			"-I"+features,
			"-I"+libs,
		)
		objFiles = append(objFiles, objName)
	}

	for _, src := range libFiles {
		base := strings.TrimSuffix(filepath.Base(src), ".cpp")
		objName := filepath.Join(buildDir, "Libs", base+".o")
		depName := filepath.Join(buildDir, "Libs", base+".d")
		runCommand("g++",
			"-std=c++17", "-c", src,
			"-o", objName,
			"-MMD", "-MF", depName,
			"-I"+features,
			"-I"+libs,
		)
		objFiles = append(objFiles, objName)
	}

	mainObj := filepath.Join(buildDir, "main.o")
	mainDep := filepath.Join(buildDir, "main.d")
	runCommand("g++",
		"-std=c++17", "-c", mainSrc,
		"-o", mainObj,
		"-MMD", "-MF", mainDep,
		"-I"+features,
		"-I"+libs,
	)
	objFiles = append(objFiles, mainObj)

	output := "main"
	args := append(objFiles, "-o", output)
	runCommand("g++", args...)
	fmt.Println("Build completed successfully.")
}

func runCommand(name string, args ...string) {
	cmd := exec.Command(name, args...)
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr
	if err := cmd.Run(); err != nil {
		log.Fatalf("Command failed: %v", err)
	}
}
