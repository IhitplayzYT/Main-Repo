package main

import (
	"fmt"
	"strings"

	"github.com/charmbracelet/bubbles/textinput"
	tea "github.com/charmbracelet/bubbletea"
)

type model struct {
	input       textinput.Model
	trie        *Trie
	suggestions []string
	selected    int
}

func NewModel(trie *Trie) model {
	ti := textinput.New()
	ti.Placeholder = "Type..."
	ti.Focus()
	return model{
		input: ti,
		trie:  trie,
	}
}

func (m model) Init() tea.Cmd {
	return textinput.Blink
}

func (m model) Update(msg tea.Msg) (tea.Model, tea.Cmd) {

	switch msg := msg.(type) {

	case tea.KeyMsg:

		switch msg.String() {

		case "ctrl+c":
			return m, tea.Quit

		case "up":
			if m.selected > 0 {
				m.selected--
			}
			return m, nil

		case "down":
			if m.selected < len(m.suggestions)-1 {
				m.selected++
			}
			return m, nil

		case "tab":
			if len(m.suggestions) > 0 {
				m.input.SetValue(
					m.suggestions[m.selected],
				)
			}
			return m, nil

		case "enter":

			word := m.input.Value()

			fmt.Println("\nSelected:", word)

			m.trie.incr_freq(word)

			m.input.SetValue("")
			m.suggestions = nil
			m.selected = 0

			return m, nil
		}
	}

	var cmd tea.Cmd

	m.input, cmd = m.input.Update(msg)

	prefix := strings.TrimSpace(
		m.input.Value(),
	)

	if prefix == "" {
		m.suggestions = nil
		m.selected = 0
	} else {
		m.suggestions =
			m.trie.Autocomplete_top_N(
				prefix,
				10,
			)

		if m.selected >= len(m.suggestions) {
			m.selected = 0
		}
	}

	return m, cmd
}

func (m model) View() string {

	s := "Search:\n\n"
	s += m.input.View()
	s += "\n\n"

	for i, word := range m.suggestions {

		cursor := "  "

		if i == m.selected {
			cursor = "> "
		}

		s += cursor + word + "\n"
	}

	s += "\n↑ ↓ navigate"
	s += "\nTAB complete"
	s += "\nENTER submit"
	s += "\nCTRL+C quit"

	return s
}
