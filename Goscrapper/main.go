package main

import (
	"fmt"
	"io"
	"net/http"
	"os"
	"strings"
)

// Placeholder
var website_list []string = []string{"a", "b", "c"}

func usage(str string) {
	fmt.Println(str, " [-d] <KEYWORD>")
	return
}

func print_map(s map[string][]string) {
	for k, v := range s {
		fmt.Print("[%v]:", k)
		z := len(k)
		for i := 0; i < len(v); i++ {
			fmt.Println(strings.Repeat(" ", z+2), v[i])
		}
	}
}

func search(str string, flag bool) map[string][]string {
	ret := make(map[string][]string)
	for _, v := range website_list {
		res, err := http.Get(v)
		html_ret, err := io.ReadAll(res.Body)
		res.Body.Close()
		if err != nil {
			ret[v] = append(ret[v], html_ret)
		}

	}
	return ret
}

func main() {
	args := os.Args
	keyword, deep_flag := "", false
	l := len(args)
	if l < 2 || l > 3 {
		usage(args[0])
		return
	} else if l == 2 {
		keyword = args[1]
	} else {
		if args[1] != "-d" {
			usage(args[0])
			return
		}
		deep_flag = true
		keyword = args[2]
	}

	ret := search(keyword, deep_flag)
	print_map(ret)
	return
}
