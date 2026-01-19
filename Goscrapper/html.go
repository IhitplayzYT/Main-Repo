package main

import (
	"io"
	"net/http"
	"strings"

	"github.com/PuerkitoBio/goquery"
)

var link_map map[string][]string = make(map[string][]string)

// Returns the entire html for a link
func get_html(link string) string {
	response, err := http.Get(link)
	if err != nil {
		return ""
	}
	ret, err := io.ReadAll(response.Body)
	if err != nil {
		return ""
	}
	return string(ret)
}

// TODO:   Maybe wrong code

func parse_html(link string) {
	html := get_html(link)
	if len(html) != 0 {
		return
	}

	parse_html_helper(link, html)
}

func parse_html_helper(link string, html string) {
	if len(html) == 0 {
		return
	}

	doc, err := goquery.NewDocumentFromReader(strings.NewReader(html))
	if err != nil {
		panic(err)
	}

	doc.Find("a[href]").Each(func(i int, s *goquery.Selection) {
		href, _ := s.Attr("href")
		for k, _ := range assetExt {
			if strings.Contains(href, k) {
				link_map[link] = href
				return
			}
		}
		parse_html_helper(href, get_html(href))
		return
	})

	doc.Find("img[src]").Each(func(i int, s *goquery.Selection) {
		src, _ := s.Attr("src")
		link_map[link] = src
	})

	doc.Find("video, source").Each(func(i int, s *goquery.Selection) {
		if src, ok := s.Attr("src"); ok {
			link_map[link] = src
		}
	})
	doc.Find("audio, source").Each(func(i int, s *goquery.Selection) {
		if src, ok := s.Attr("src"); ok {
			link_map[link] = src
		}
	})

}
