package main

import (
	"fmt"
	"os"
	"strings"
)

type i8 int8
type i16 int16
type i32 int32
type i64 int64
type s8 uint8
type s16 uint16
type s32 uint32
type s64 uint64
type f32 float32
type f64 float64

const True bool = true
const False bool = false

// Placeholder
func usage(str string) {
	fmt.Println("Usage : ", str, " [-d] <KEYWORD>")
	return
}

func print_map(s map[string][]string) {
	for k, v := range s {
		fmt.Print("[%v]:", k)
		z, y := len(k), len(v)
		for i := 0; i < y; i++ {
			fmt.Println(strings.Repeat(" ", z+2), v[i])
		}
		fmt.Println()
	}
}

// TODO:

func search(str string, flag bool) {

	for _, v := range website_list {
		parse_html(v)
	}

}

func parse_args(args []string) (string, bool) {
	keyword, deep_flag := "", false
	l := len(args)
	if l < 2 || l > 3 {
		usage(args[0])
		return "", false
	} else if l == 2 {
		keyword = args[1]
	} else {
		if args[1] != "-d" {
			usage(args[0])
			return "", false
		}
		deep_flag = true
		keyword = args[2]
	}
	return keyword, deep_flag
}

func main() {
	args := os.Args
	keyword, deep_flag := parse_args(args)
	if keyword == "" {
		return
	}
	search(keyword, deep_flag)
	print_map(ret)
	return
}
