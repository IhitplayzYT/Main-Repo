package main

import (
	"math"
	"regexp"
	"strings"
)

var hexregex = regexp.MustCompile(`^[0-9a-fA-F]+$`)

// To check if a hash if hash we download if not we serach keyword before download
func is_hash(s string) bool {
	before, _, _ := strings.Cut(s, ".")
	l := len(before)
	if l > 100 {
		return true
	}
	if l != 32 && l != 40 && l != 64 && l != 128 {
		return impurity(before) > 3.5
	}
	return hexregex.Match([]byte(before))
}

func impurity(s string) float64 {
	freq := make(map[rune]float64)
	for _, c := range s {
		freq[c] += 1
	}
	l := float64(len(s))
	e := 0.0
	for _, f := range freq {
		p := f / l
		e -= p * math.Log2(p)
	}

	return e
}
