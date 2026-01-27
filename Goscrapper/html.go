package main

import (
	"io"
	"net/http"
	"strings"

	"github.com/PuerkitoBio/goquery"
)

var link_map map[string][]string = make(map[string][]string)

type Media_Type int

const (
	Invalid_T Media_Type = iota
	Image_T
	Video_T
	Audio_T
	Files_T
	Misc_T
)

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

	go parse_html_helper(link, html)
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
		install(src, Image_T)
	})

	doc.Find("video, source").Each(func(i int, s *goquery.Selection) {
		if src, ok := s.Attr("src"); ok {
			link_map[link] = src
			install(src, Video_T)
		}
	})
	doc.Find("audio, source").Each(func(i int, s *goquery.Selection) {
		if src, ok := s.Attr("src"); ok {
			link_map[link] = src
			install(src, Audio_T)
		}
	})

}

func install(src string, M_type Media_Type) {
	dwnld_dir := "output/"
	switch M_type {

	case Image_T:
		{
			dwnld_dir += "images/"
			break
		}
	case Audio_T:
		{
			dwnld_dir += "Audio/"
			break
		}
	case Video_T:
		{
			dwnld_dir += "videos/"
			break
		}
	case Files_T:
		{
			dwnld_dir += "files/"
			break
		}
	default:
		{
			dwnld_dir += "misc/"
			return
		}

		/* Main Download logic alongside a completion abr for each thread */

	}

}
