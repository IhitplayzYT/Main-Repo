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

func print_map(s map[string][]string) {
	for k, v := range s {
		fmt.Printf("[%v]:", k)
		z, y := len(k), len(v)
		for i := 0; i < y; i++ {
			fmt.Println(strings.Repeat(" ", z+2), v[i])
		}
		fmt.Println()
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
		deep_flag = 1
	} else {
		if args[1] == "-d" {
			deep_flag = 2
		} else if args[1] == "-D" {
			deep_flag = 3
		} else {
			usage(args[0])
			return "", 0
		}
		keyword = args[2]
	}
	return strings.Trim(keyword, " "), deep_flag
}

func match_longest_any(to_be_found, to_be_searched string) (int, int) {
	lf, ls := len(to_be_found), len(to_be_searched)
	mx_len, ed_idx := 0, -1
	dp := make([][]int, lf+1)
	for i := 0; i <= lf; i++ {
		dp[i] = make([]int, ls+1)
	}

	for i := 1; i <= lf; i++ {
		for j := 1; j <= ls; j++ {
			if to_be_found[i-1] == to_be_searched[j-1] {
				dp[i][j] = dp[i-1][j-1] + 1
				if dp[i][j] > mx_len {
					mx_len = dp[i][j]
					ed_idx = j
				}
			} else {
				dp[i][j] = dp[i-1][j-1]
			}
		}
	}
	return ed_idx - mx_len, mx_len
}

func match_longest_prefix(to_be_found, to_be_searched string) (int, int) {
	cur_idx, mx_len, mx_idx, lf, ls := 0, 0, -1, len(to_be_found), len(to_be_searched)
	for i := 0; i < ls; i++ {
		if cur_idx == lf {
			return i - lf, lf
		}
		if to_be_found[cur_idx] == to_be_searched[i] {
			cur_idx += 1
		} else {
			if mx_len < cur_idx {
				mx_len = cur_idx
				mx_idx = i - cur_idx
			}

			if to_be_found[0] == to_be_searched[i] {
				cur_idx = 1
			} else {
				cur_idx = 0
			}

		}

	}
	return mx_idx, mx_len
}

// CAN FAIL WITH -1
func match_full(to_be_found, to_be_searched string) int {
	ls := len(to_be_searched)
	lf := len(to_be_found)
	idx := 0
	for i := 0; i < ls; i++ {
		if to_be_found[idx] == to_be_searched[i] {
			idx += 1
		}
		if idx == lf {
			return i - lf + 1
		}
	}

	return -1
}

func usage(argv string) {
	sidx := strings.LastIndex(argv, "/")
	s := argv[sidx:]
	fmt.Print("Usage: .", s, " [-d | -D] <KEYWORD>\n")
	fmt.Println("FLAGS:\n  -d -> Deep Unfiltered Search\n  -D -> Download Flag")
	fmt.Println("NOTE: Passing no flag resorts to Safe Search WITHOUT Downloads")
	os.Exit(1)
}

func get_extention(s string) string {
	idx := strings.Index(s, ".")
	ret := s[idx:]
	return ret
}

func get_category(s string) (ret Media_Type) {
	ext := get_extention(s)
	if _, ok := VideoExt[ext]; ok {
		ret = Video_T
	} else if _, ok := ImageExt[ext]; ok {
		ret = Image_T
	} else if _, ok := AudioExt[ext]; ok {
		ret = Audio_T
	} else if _, ok := DocExt[ext]; ok {
		ret = Files_T
	} else {
		ret = Misc_T
	}
	return
}
