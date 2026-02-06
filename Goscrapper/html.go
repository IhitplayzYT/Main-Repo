package main

import (
	"crypto/sha1"
	"encoding/hex"
	"fmt"
	"io"
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

var KEYWORD string
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
		MaxIdleConnsPerHost: 10,
	},
}

// Returns the entire html for a link
func get_html(link string) string {
	response, err := http.Get(link)
	if err != nil {
		return ""
	}
	defer response.Body.Close()
	res, err := io.ReadAll(response.Body)
	if err != nil {
		return ""
	}
	return string(res)
}

func parse_html(link string) {
	html := get_html(link)
	if len(html) == 0 {
		return
	}
	parse_html_helper(link, html)
}

func parse_html_helper(link string, html string) {
	if len(html) == 0 || !strings.Contains(html, KEYWORD) {
		return
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
		install(resolved, Image_T)
	})

	doc.Find("video source").Each(func(i int, s *goquery.Selection) {
		src, _ := s.Attr("src")
		ref, err := url.Parse(src)
		if err != nil {
			return
		}
		resolved := base.ResolveReference(ref).String()
		append_to_lmap(link, src)
		install(resolved, Video_T)
	})
	doc.Find("audio source").Each(func(i int, s *goquery.Selection) {
		src, _ := s.Attr("src")
		ref, err := url.Parse(src)
		if err != nil {
			return
		}
		resolved := base.ResolveReference(ref).String()
		append_to_lmap(link, src)
		install(resolved, Audio_T)
	})

	install_html(link, html)
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

	name, err := htmlfname(link)
	if err != nil {
		return err
	}
	home, err := os.UserHomeDir()
	if err != nil {
		return err
	}
	path := filepath.Join(home, "Goscrapper", "output", "Websites_html", name)
	return os.WriteFile(path, []byte(html), 0755)
}

func install(src string, M_type Media_Type) {
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
		fmt.Printf("Failed to create directory %s: %v", dwnld_dir, err)
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

}
