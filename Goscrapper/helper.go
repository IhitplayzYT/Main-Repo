package main

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
