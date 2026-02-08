package main

import (
	"context"
	"crypto/sha1"
	"encoding/hex"
	"fmt"
	"io"
	"math/rand"
	"net/http"
	"net/url"
	"os"
	"os/exec"
	"path/filepath"
	"strings"
	"sync"
	"time"

	"github.com/PuerkitoBio/goquery"
)

var visited, link_map sync.Map
var wg sync.WaitGroup

type Media_Type int

const (
	Invalid_T Media_Type = iota
	Image_T
	Video_T
	Audio_T
	Files_T
	Misc_T
)

var client = &http.Client{
	Timeout: 10 * time.Second,
	Transport: &http.Transport{
		Proxy:                 http.ProxyFromEnvironment,
		MaxIdleConns:          100,
		MaxIdleConnsPerHost:   20,
		IdleConnTimeout:       30 * time.Second,
		ForceAttemptHTTP2:     true,
		TLSHandshakeTimeout:   10 * time.Second,
		ExpectContinueTimeout: 1 * time.Second,
		DisableCompression:    false,
	},
}

func rand_delay() {
	time.Sleep(time.Duration(500+rand.Intn(1500)) * time.Millisecond)
}

func get_html(link string) (string, error) {
	rand_delay()
	context, cancel := context.WithTimeout(context.Background(), 30*time.Second)
	defer cancel()
	request, err := http.NewRequestWithContext(context, "GET", link, nil)
	if err != nil {
		return "", err
	}
	request.Header.Set("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 Chrome/121 Safari/537.36")
	request.Header.Set("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8")
	request.Header.Set("Accept-Language", "en-US,en;q=0.5")
	request.Header.Set("Accept-Encoding", "gzip, deflate, br")
	request.Header.Set("Connection", "keep-alive")
	request.Header.Set("Upgrade-Insecure-Requests", "1")
	response, err := client.Do(request)
	if err != nil {
		return "", err
	}
	if response.StatusCode < 200 || response.StatusCode >= 300 {
		return "", err
	}
	defer response.Body.Close()
	res, err := io.ReadAll(response.Body)
	if err != nil {
		return "", err
	}
	return string(res), nil
}

func parse_html(link string) {
	html, err := get_html(link)
	if err != nil {
		fmt.Println("Error : ", err)
		return
	}
	if len(html) == 0 {
		return
	}
	parse_html_helper(link, html)
}

func parse_html_helper(link string, html string) {
	if FLAG == 2 || FLAG == 5 {
		if len(html) == 0 || !strings.Contains(html, KEYWORD) {
			return
		}
	}
	if FLAG == 4 || FLAG == 6 {
		if len(html) == 0 || !in_thresh(KEYWORD, html) {
			return
		}
	}

	doc, err := goquery.NewDocumentFromReader(strings.NewReader(html))
	if err != nil {
		panic(err)
	}
	base, err := url.Parse(link)
	if err != nil {
		fmt.Printf("Error parsing base URL %s: %v", link, err)
		return
	}
	doc.Find("a[href]").Each(func(i int, s *goquery.Selection) {
		href, _ := s.Attr("href")
		ref, err := url.Parse(href)
		if err != nil {
			return
		}
		urlpath := base.ResolveReference(ref).String()
		for k, _ := range SupportedExts {
			if strings.Contains(href, k) {
				append_to_lmap(link, urlpath)
			}
		}
		add_job(urlpath)
	})

	doc.Find("img[src]").Each(func(i int, s *goquery.Selection) {
		src, _ := s.Attr("src")
		ref, err := url.Parse(src)
		if err != nil {
			return
		}
		resolved := base.ResolveReference(ref).String()
		append_to_lmap(link, src)
		if FLAG == 3 || FLAG == 5 || FLAG == 6 {
			install(resolved, Image_T)
		}
	})

	doc.Find("video source").Each(func(i int, s *goquery.Selection) {
		src, _ := s.Attr("src")
		ref, err := url.Parse(src)
		if err != nil {
			return
		}
		resolved := base.ResolveReference(ref).String()
		append_to_lmap(link, src)
		if FLAG == 3 || FLAG == 5 || FLAG == 6 {
			install(resolved, Video_T)
		}
	})
	doc.Find("audio source").Each(func(i int, s *goquery.Selection) {
		src, _ := s.Attr("src")
		ref, err := url.Parse(src)
		if err != nil {
			return
		}
		resolved := base.ResolveReference(ref).String()
		append_to_lmap(link, src)
		if FLAG == 3 || FLAG == 5 || FLAG == 6 {
			install(resolved, Audio_T)
		}
	})
	if FLAG == 3 || FLAG == 5 || FLAG == 6 {
		install_html(link, html)
	}
}

func append_to_lmap(key string, val string) {

	old, _ := link_map.Load(key)
	var slice []string
	if old != nil {
		slice = old.([]string)
	}
	newslice := append(slice, val)
	if link_map.CompareAndSwap(key, old, newslice) {
		return
	}

}

func htmlfname(name string) (string, error) {
	u, err := url.Parse(name)
	if err != nil {
		return "", err
	}

	base := strings.ReplaceAll(u.Host+u.Path, "/", "-")
	if base == "" {
		base = "index"
	}

	hsh := sha1.Sum([]byte(base))
	hash := hex.EncodeToString(hsh[:0])
	return base + "_" + hash + ".html", nil
}

func install_html(link, html string) error {
	fmt.Println("Installing", link)
	name, err := htmlfname(link)
	if err != nil {
		return err
	}
	home, err := os.UserHomeDir()
	if err != nil {
		return err
	}
	path := filepath.Join(home, "Goscrapper", "output", "Websites_html", name)
	fmt.Println("Installed", link)
	return os.WriteFile(path, []byte(html), 0755)
}

func install(src string, M_type Media_Type) {
	fmt.Println("Installing", src)
	home, _ := os.UserHomeDir()
	dwnld_dir := filepath.Join(home, "Goscrapper", "output")
	switch M_type {

	case Image_T:
		{
			dwnld_dir += "/images"
			break
		}
	case Audio_T:
		{
			dwnld_dir += "/Audio"
			break
		}
	case Video_T:
		{
			dwnld_dir += "/videos"
			break
		}
	case Files_T:
		{
			dwnld_dir += "/files"
			break
		}
	default:
		{
			dwnld_dir += "/misc"
			return
		}

	}
	if err := os.MkdirAll(dwnld_dir, 0755); err != nil {
		fmt.Printf("Failed to create directory %s: %v\n", dwnld_dir, err)
		return
	}
	bin := filepath.Join(home, "Godownloader", "main")
	cmd := exec.Command(bin, src, dwnld_dir)
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr
	err := cmd.Run()
	if err != nil {
		fmt.Println("Download Failed")
	}
	fmt.Println("Installed [", src, "] -> [", dwnld_dir, "]")
}
