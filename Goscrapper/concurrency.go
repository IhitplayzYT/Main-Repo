package main

var WORKER_COUNT i32 = 100

type Job struct {
	url string
}

func worker(jobs <-chan Job) {
	for job := range jobs {
		parse_html(job.url)
		wg.Done()
	}

}

func init_pool() {
	JobQueue = make(chan Job, 1000)
}

func add_job(link string) {
	if _, loaded := visited.LoadOrStore(link, true); loaded {
		return
	}
	wg.Add(1)
	JobQueue <- Job{url: link}
}

func start_workers() {
	for i := 0; i < int(WORKER_COUNT); i++ {
		go worker(JobQueue)
	}
}
