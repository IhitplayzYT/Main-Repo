package main

import (
	"encoding/gob"
	"os"
	"sort"
)

type Trie struct {
	child  map[rune]*Trie
	isword bool
	freq   int
}

func NewTrie() *Trie {
	return &Trie{
		child: make(map[rune]*Trie),
	}
}

func (t *Trie) insert_all(words []string) {
	for _, word := range words {
		cur := t
		for _, ch := range word {
			if cur.child[ch] == nil {
				cur.child[ch] = NewTrie()
			}
			cur = cur.child[ch]
		}

		cur.isword = true
	}
}

func (t *Trie) insert(word string) {
	cur := t
	for _, ch := range word {
		if cur.child[ch] == nil {
			cur.child[ch] = NewTrie()
		}
		cur = cur.child[ch]
	}

	cur.isword = true
}

func (t *Trie) search(word string) bool {
	cur := t
	for _, ch := range word {
		next := cur.child[ch]
		if next == nil {
			return false
		}
		cur = next
	}
	return cur.isword
}

func (t *Trie) starts_with(prefix string) bool {
	return t.find_node(prefix) != nil
}

func (t *Trie) find_node(prefix string) *Trie {
	cur := t
	for _, ch := range prefix {
		next := cur.child[ch]
		if next == nil {
			return nil
		}
		cur = next
	}
	return cur
}

func (t *Trie) delete(word string) bool {
	return delete_helper(t, []rune(word), 0)
}

func delete_helper(node *Trie, word []rune, idx int) bool {
	if node == nil {
		return false
	}
	if idx == len(word) {
		if !node.isword {
			return false
		}
		node.isword = false
		node.freq = 0
		return len(node.child) == 0
	}
	ch := word[idx]
	next := node.child[ch]
	if next == nil {
		return false
	}
	shouldDelete := delete_helper(next, word, idx+1)
	if shouldDelete {
		delete(node.child, ch)
	}
	return !node.isword && len(node.child) == 0
}

func (t *Trie) get_prefix_words(prefix string) []string {
	var result []string
	node := t.find_node(prefix)

	if node == nil {
		return result
	}

	node.collect(prefix, &result)

	return result
}

func (t *Trie) collect(cur string, result *[]string) {
	if t.isword {
		*result = append(*result, cur)
	}

	for ch, next := range t.child {
		next.collect(cur+string(ch), result)
	}
}

func (t *Trie) Autocomplete(prefix string) []string {
	return t.get_prefix_words(prefix)
}

func (t *Trie) incr_freq(word string) {
	cur := t
	for _, ch := range word {
		next := cur.child[ch]
		if next == nil {
			return
		}
		cur = next
	}
	if cur.isword {
		cur.freq++
	}
}

func (t *Trie) set_freq(word string, freq int) {
	cur := t
	for _, ch := range word {
		next := cur.child[ch]
		if next == nil {
			return
		}
		cur = next
	}
	if cur.isword {
		cur.freq = freq
	}
}

type Suggestion struct {
	Word string
	Freq int
}

func (t *Trie) Autocomplete_top_N(prefix string, k int) []string {
	node := t.find_node(prefix)
	if node == nil {
		return nil
	}
	var suggestions []Suggestion
	node.collectSuggestions(
		prefix,
		&suggestions,
	)
	sort.Slice(
		suggestions,
		func(i, j int) bool {
			return suggestions[i].Freq >
				suggestions[j].Freq
		},
	)
	if k > len(suggestions) {
		k = len(suggestions)
	}
	result := make([]string, k)
	for i := 0; i < k; i++ {
		result[i] = suggestions[i].Word
	}
	return result
}

func (t *Trie) collectSuggestions(
	cur string,
	out *[]Suggestion,
) {
	if t.isword {
		*out = append(
			*out,
			Suggestion{
				Word: cur,
				Freq: t.freq,
			},
		)
	}

	for ch, next := range t.child {
		next.collectSuggestions(
			cur+string(ch),
			out,
		)
	}
}

func (t *Trie) CountWords() int {
	count := 0

	if t.isword {
		count++
	}

	for _, child := range t.child {
		count += child.CountWords()
	}

	return count
}

func (t *Trie) CountPrefix(prefix string) int {
	node := t.find_node(prefix)

	if node == nil {
		return 0
	}

	return node.CountWords()
}

func (t *Trie) LongestPrefix(s string) string {
	cur := t

	lastMatch := 0
	runes := []rune(s)

	for i, ch := range runes {
		next := cur.child[ch]

		if next == nil {
			break
		}

		cur = next

		if cur.isword {
			lastMatch = i + 1
		}
	}

	return string(runes[:lastMatch])
}

func (t *Trie) InsertBatch(words []string) {
	for _, word := range words {
		t.insert(word)
	}
}

func (t *Trie) Save(path string) error {
	f, err := os.Create(path)
	if err != nil {
		return err
	}
	defer f.Close()

	enc := gob.NewEncoder(f)

	return enc.Encode(t)
}

func LoadTrie(path string) (*Trie, error) {
	f, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	defer f.Close()

	var trie Trie

	dec := gob.NewDecoder(f)

	if err := dec.Decode(&trie); err != nil {
		return nil, err
	}

	return &trie, nil
}
