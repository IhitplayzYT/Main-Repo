package main

import "fmt"

var assetExt = map[string]bool{
	".jpg": true, ".jpeg": true, ".png": true, ".gif": true, ".webp": true,
	".mp4": true, ".webm": true, ".mp3": true, ".wav": true, ".ogg": true,
	".pdf": true, ".zip": true, ".tar": true, ".gz": true,
}

type Data struct {
	link  [][]string
	text  [][]string
	image [][]string
	video [][]string
	audio [][]string
}

type Data_interface interface {
	print()
	print_image()
	print_audio()
	print_video()
	print_text()
}

func (s *Data) print_image() {
	l := len(s.image)
	fmt.Println("Image Files: ")
	for i := 0; i < l; i++ {
		fmt.Println("\t%v : %v", s.image[i][0], s.image[i][1])
	}
}

func (s *Data) print_audio() {
	l := len(s.audio)
	fmt.Println("Audio Files: ")
	for i := 0; i < l; i++ {
		fmt.Println("\t%v : %v", s.audio[i][0], s.audio[i][1])
	}
}

func (s *Data) print_text() {
	l := len(s.text)
	fmt.Println("Text Snippets: ")
	for i := 0; i < l; i++ {
		fmt.Println("\t%v : %v", s.text[i][0], s.audio[i][1])
	}
}

func (s *Data) print_video() {
	l := len(s.video)
	fmt.Println("Video Files: ")
	for i := 0; i < l; i++ {
		fmt.Println("\t%v : %v", s.video[i][0], s.audio[i][1])
	}
}
