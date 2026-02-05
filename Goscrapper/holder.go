package main

import "fmt"

type Data struct {
	link  map[string]string
	text  map[string]string
	image map[string]string
	video map[string]string
	audio map[string]string
}

type Data_interface interface {
	print()
	print_image()
	print_audio()
	print_video()
	print_text()
}

func (s *Data) print_image() {
	fmt.Println("Image Files: ")
	for k, v := range s.image {
		fmt.Printf("\t%v : %v\n", k, v)
	}
}

func (s *Data) print_audio() {
	fmt.Println("Audio Files: ")
	for k, v := range s.audio {
		fmt.Printf("\t%v : %v\n", k, v)
	}
}

func (s *Data) print_text() {
	fmt.Println("Text Snippets: ")
	for k, v := range s.text {
		fmt.Printf("\t%v : %v\n", k, v)
	}
}

func (s *Data) print_video() {
	fmt.Println("Video Files: ")
	for k, v := range s.video {
		fmt.Printf("\t%v : %v\n", k, v)
	}
}
