package main

import "sync"

var WORKER_COUNT i32 = 100

type Job struct {
	url string
}

func worker(id int, jobs <-chan Job, wg *sync.WaitGroup) {
	defer wg.Done()
	for job := range jobs {
		parse_html(job.url)
	}

}

func init_pool(flag bool) chan Job {
	jobs := make(chan Job, 1000)
	var wg sync.WaitGroup
	for i := 0; i < int(WORKER_COUNT); i++ {
		wg.Add(1)
		go worker(i, jobs, &wg)
	}
	return jobs
}
