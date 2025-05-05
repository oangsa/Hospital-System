package main

import (
	"fmt"
	"log"
	"os"
	"os/exec"
	"path/filepath"
	"strings"
	"sync"
)

func main() {
	features := "Features"
	libs := "Libs"
	mainSrc := "main.cpp"
	buildDir := "builds"
	objFiles := []string{}

	_ = os.MkdirAll(buildDir, 0755)
	_ = os.MkdirAll(filepath.Join(buildDir, "Features"), 0755)
	_ = os.MkdirAll(filepath.Join(buildDir, "Libs"), 0755)

	featureFiles, _ := filepath.Glob(filepath.Join(features, "*.cpp"))
	libFiles, _ := filepath.Glob(filepath.Join(libs, "*.cpp"))

	var wg sync.WaitGroup
	var mu sync.Mutex
	errChan := make(chan error, len(featureFiles)+len(libFiles)+1)

	// Compile Features concurrently
	for _, src := range featureFiles {
		wg.Add(1)
		go func(src string) {
			defer wg.Done()
			base := strings.TrimSuffix(filepath.Base(src), ".cpp")
			objName := filepath.Join(buildDir, "Features", base+".o")
			depName := filepath.Join(buildDir, "Features", base+".d")
			err := runCommand("g++",
				"-std=c++17", "-c", src,
				"-o", objName,
				"-MMD", "-MF", depName,
				"-I"+features,
				"-I"+libs,
			)
			if err == nil {
				mu.Lock()
				objFiles = append(objFiles, objName)
				mu.Unlock()
			} else {
				errChan <- err
			}
		}(src)
	}

	// Compile Libs concurrently
	for _, src := range libFiles {
		wg.Add(1)
		go func(src string) {
			defer wg.Done()
			base := strings.TrimSuffix(filepath.Base(src), ".cpp")
			objName := filepath.Join(buildDir, "Libs", base+".o")
			depName := filepath.Join(buildDir, "Libs", base+".d")
			err := runCommand("g++",
				"-std=c++17", "-c", src,
				"-o", objName,
				"-MMD", "-MF", depName,
				"-I"+features,
				"-I"+libs,
			)
			if err == nil {
				mu.Lock()
				objFiles = append(objFiles, objName)
				mu.Unlock()
			} else {
				errChan <- err
			}
		}(src)
	}

	wg.Wait()
	close(errChan)

	// Check for errors
	for err := range errChan {
		if err != nil {
			log.Fatalf("Build failed: %v", err)
		}
	}

	// Compile main.cpp (not concurrent, must be after others)
	mainObj := filepath.Join(buildDir, "main.o")
	mainDep := filepath.Join(buildDir, "main.d")
	if err := runCommand("g++",
		"-std=c++17", "-c", mainSrc,
		"-o", mainObj,
		"-MMD", "-MF", mainDep,
		"-I"+features,
		"-I"+libs,
	); err != nil {
		log.Fatalf("Build failed: %v", err)
	}
	objFiles = append(objFiles, mainObj)

	output := "main"
	args := append(objFiles, "-o", output)
	if err := runCommand("g++", args...); err != nil {
		log.Fatalf("Link failed: %v", err)
	}
	fmt.Println("Done")
}

func runCommand(name string, args ...string) error {
	cmd := exec.Command(name, args...)
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr
	return cmd.Run()
}
