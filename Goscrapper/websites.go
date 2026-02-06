package main

import (
	"encoding/csv"
	"os"
)

func boost_wlist() {
	l := len(website_list)
	for i := 0; i < l; i++ {
		website_list = append(website_list, (website_list[i] + "/search?q=" + KEYWORD))
		website_list = append(website_list, (website_list[i] + "/search?query=" + KEYWORD))
		website_list = append(website_list, (website_list[i] + "/?s=" + KEYWORD))
		website_list = append(website_list, (website_list[i] + "/search?keyword=" + KEYWORD))
	}

}

func init_wlist() {
	f, err := os.Open("majestic_million.csv")
	if err != nil {
		panic(err)
	}
	defer f.Close()
	reader := csv.NewReader(f)
	header, err := reader.Read()
	if err != nil {
		panic(err)
	}
	domainIdx := -1
	for i, h := range header {
		if h == "Domain" {
			domainIdx = i
			break
		}
	}

	if domainIdx == -1 {
		panic("Column \"Domain\" not found")
	}
	for {
		row, err := reader.Read()
		if err != nil {
			break
		}
		if domainIdx < len(row) && len(row[domainIdx]) != 0 {
			website_list = append(website_list, row[domainIdx])
		}
	}

}

var website_list []string = []string{"https://www.instagram.com/"}
