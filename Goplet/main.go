package main

import (
	"fmt"

	tea "github.com/charmbracelet/bubbletea"
)

func main() {
	clargs := CLI{dbg: false, idict: make([]string, 0)}
	clargs.Parse_args()
	if clargs.dbg {
		fmt.Println(clargs)
	}
	vocab := build_vocab(clargs)
	if clargs.dbg {
		fmt.Println(vocab)
	}
	trie := NewTrie()
	trie.insert_all(vocab)

	p := tea.NewProgram(
		NewModel(trie),
	)

	if _, err := p.Run(); err != nil {
		panic(err)
	}

}
