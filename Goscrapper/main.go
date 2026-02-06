package main

import (
	"os"
)

var data Data
var JobQueue chan Job

func fn(s string) {
}

func main() {
	KEYWORD, flag := parse_args(os.Args)
	if flag == 0 {
		return
	}
	init_pool(flag)
	init_wlist()
	boost_wlist()
	for i, url := range website_list {
		if i == 50 {
			break
		}
		add_job(url)
	}
	start_workers()
	wg.Wait()
	close(JobQueue)
	fn(KEYWORD)
}
