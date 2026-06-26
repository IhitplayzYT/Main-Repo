package main

import (
	"errors"
	"fmt"
	"io/fs"
	"os"
	"path/filepath"
	"strings"
)

type CLI struct {
	dbg   bool
	idict []string
}

func (cli *CLI) Parse_args() {
	args := os.Args[1:]
	for _, v := range args {
		v = strings.ToLower(v)
		if strings.HasPrefix(v, "--idir=") {
			cli.idict = append(cli.idict, v[7:])
		} else if v == "-d" || v == "--debug" {
			cli.dbg = true
		} else {
			cli.idict = append(cli.idict, v)
		}
	}
}

// Map transforms a slice from type T to type U
func Map[T any, U any](slice []T, f func(T) U) []U {
	result := make([]U, len(slice))
	for i, v := range slice {
		result[i] = f(v)
	}
	return result
}

// Filter extracts elements matching a predicate
func Filter[T any](slice []T, predicate func(T) bool) []T {
	var result []T
	for _, v := range slice {
		if predicate(v) {
			result = append(result, v)
		}
	}
	return result
}

func If[T any](cond bool, if_b T, else_b T) T {
	if cond {
		return if_b
	} else {
		return else_b
	}
}

func f_vocab(path string) []string {
	fmt.Println("Reading:", path)
	if cont, err := os.ReadFile(path); err == nil {
		return strings.Split(strings.ToLower(string(cont)), "\n")
	} else {
		return []string{}
	}
}

func d_vocab(path string) []string {
	ret := make([]string, 0)
	if members, err := os.ReadDir(path); err == nil {
		for _, v := range members {
			fullpath := filepath.Join(path, v.Name())
			ret = append(ret, If(v.IsDir(), d_vocab(fullpath), f_vocab(fullpath))...)
		}
	}
	return ret
}

func normalizePath(p string) (string, error) {
	if strings.HasPrefix(p, "~") {
		home, err := os.UserHomeDir()
		if err != nil {
			return "", err
		}

		p = filepath.Join(
			home,
			strings.TrimPrefix(p, "~"),
		)
	}

	return filepath.Abs(p)
}

func build_vocab(cli CLI) []string {
	ret := make([]string, 0)
	for _, v := range cli.idict {

		path, err := normalizePath(v)
		if err != nil {
			fmt.Printf("bad path %q: %v\n", v, err)
			continue
		}
		if info, err := os.Stat(path); err == nil {
			ret = append(ret, If(info.IsDir(), d_vocab(path), f_vocab(path))...)
		} else if errors.Is(err, fs.ErrNotExist) {
			fmt.Println("Path does not exist")
		} else {
			fmt.Printf("Other Error: %v\n", err)
		}

	}
	return ret
}
