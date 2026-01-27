package main

import (
	"fmt"
	"os"
	"strings"
)

var ret map[string][]string

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

func search(str string, flag i8) {

	for _, v := range website_list {
		parse_html(v)
	}

}

func parse_args(args []string) (string, i8) {
	var keyword string = ""
	var deep_flag i8 = 0
	l := len(args)
	if l < 2 || l > 3 {
		usage(args[0])
		return "", 0
	} else if l == 2 {
		keyword = args[1]
	} else {
		if args[1] == "-d" {
			deep_flag = 1
		} else if args[1] == "-D" {
			deep_flag = 2
		} else {
			usage(args[0])
			return "", 0
		}
		keyword = args[2]
	}
	return keyword, deep_flag
}

func main() {
	args := os.Args
	keyword, flag := parse_args(args)
	if keyword == "" {
		return
	}
	search(keyword, flag)
	print_map(ret)
	return
}
