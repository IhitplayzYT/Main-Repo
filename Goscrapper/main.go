package main

import (
	"os"
)

var KEYWORD string
var FLAG i8
var data Data
var JobQueue chan Job
var Levanshtein_MIN_THreshold int

func main() {
	KEYWORD, FLAG = parse_args(os.Args)
	if FLAG == 0 {
		return
	}
	Levanshtein_MIN_THreshold = int(30 * len(KEYWORD) / 100)

	init_pool()
	// Some day get a smaller csv or use mutliple threads to process the csv
	//init_wlist()
	boost_wlist()
	start_workers()
	for _, url := range website_list {
		add_job(url)
	}
	go func() {
		wg.Wait()
		close(JobQueue)
	}()
	wg.Wait()
}
