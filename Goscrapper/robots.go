package main

import (
	"fmt"
	"net/http"
	"net/url"
	"sync"
	"time"

	"github.com/temoto/robotstxt"
)

type RobotsManager struct {
	client *http.Client
	agent  string
	mu     sync.RWMutex
	cache  map[string]*robotstxt.RobotsData
}

var RobotsAgent = &RobotsManager{
	client: client,
	agent:  "AmortisedBeing/1.0 (contact: fodder@gmaill.com)",
	cache:  make(map[string]*robotstxt.RobotsData),
}

func (agent *RobotsManager) get_robots(url string) (*robotstxt.RobotsData, error) {
	agent.mu.RLock()
	if data, ok := agent.cache[url]; ok {
		agent.mu.RUnlock()
		return data, nil
	}
	agent.mu.RUnlock()

	r_url := fmt.Sprintf("https://%s/robots.txt", url)
	txt, err := agent.client.Get(r_url)
	if err != nil || txt.StatusCode >= 400 {
		r_url := fmt.Sprintf("http://%s/robots.txt", url)
		txt, err = agent.client.Get(r_url)
		if err != nil || txt.StatusCode >= 400 {
			return nil, nil
		}

	}
	defer txt.Body.Close()
	data, err := robotstxt.FromResponse(txt)
	if err != nil {
		return nil, err
	}
	agent.mu.Lock()
	agent.cache[url] = data
	agent.mu.Unlock()
	return data, nil
}

func (agent *RobotsManager) is_allowed(link string) (bool, time.Duration) {
	u, err := url.Parse(link)
	if err != nil {
		return false, 0
	}
	data, err := agent.get_robots(u.Host)
	if err != nil || data == nil {
		return true, 0
	}

	group := data.FindGroup(agent.agent)
	return group.Test(u.Path), time.Duration(group.CrawlDelay) * time.Second
}
