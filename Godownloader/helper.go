package main

import (
	"crypto/sha256"
	"encoding/base64"
	"encoding/hex"
	"errors"
	"fmt"
	"io"
	"mime"
	"net/http"
	"net/url"
	"os"
	"path"
	"path/filepath"
	"strings"
)

func get_fname(resp *http.Response, raw string) string {
	if cd := resp.Header.Get("Content-Disposition"); cd != "" {
		_, params, err := mime.ParseMediaType(cd)
		if err == nil {
			if name, ok := params["filename*"]; ok {
				if decoded, err := url.QueryUnescape(strings.TrimPrefix(name, "UTF-8''")); err == nil {
					return decoded
				}
			}
		}
	}
	u, err := url.Parse(raw)
	if err == nil {
		name := path.Base(u.Path)
		if name != "/" && name != "." {
			return name
		}
	}

	return ""
}

func downloadDataURL(url string, dwnld_path string) error {
	parts := strings.SplitN(url, ",", 2)
	if len(parts) != 2 {
		return errors.New("Unsupported data field")
	}
	metadata, img := parts[0], parts[1]
	ext := ".bin"
	if strings.Contains(metadata, "image/png") {
		ext = ".png"
	} else if strings.Contains(metadata, "image/jpeg") {
		ext = ".jpg"
	} else if strings.Contains(metadata, "image/svg+xml") {
		ext = ".svg"
	}
	decode, err := base64.StdEncoding.DecodeString(img)
	if err != nil {
		return err
	}
	os.MkdirAll(dwnld_path, 0755)
	hash := sha256.Sum256([]byte(img))
	hashStr := hex.EncodeToString(hash[:])
	fname := fmt.Sprintf("%s%s", hashStr, ext)
	fpath := filepath.Join(dwnld_path, fname)
	return os.WriteFile(fpath, decode, 0755)
}

func download(url string, dwnld_path string) error {
	if strings.HasPrefix(url, "data:") {
		return downloadDataURL(url, dwnld_path)
	}
	res, err := http.Get(url)
	if err != nil {
		return err
	}
	defer res.Body.Close()
	if res.StatusCode != http.StatusOK {
		return fmt.Errorf("Bad Status Code %d", res.StatusCode)
	}

	fname := get_fname(res, url)
	if fname == "" {
		return errors.New("Unable to parse fname")
	}
	fpath := dwnld_path

	if err = os.MkdirAll(fpath, 0755); err != nil {
		return fmt.Errorf("Failed to create download directory: %w", err)
	}

	fpath = filepath.Join(fpath, fname)
	fmt.Println(fpath)
	out, err := os.Create(fpath)
	if err != nil {
		return errors.New("Failed to create file" + fpath)
	}
	defer out.Close()
	_, err = io.Copy(out, res.Body)
	if err != nil {
		fmt.Println(err)
	}
	return err
}

func parse_args(args []string) map[string]string {
	ret := make(map[string]string)
	prev := ""
	for i, val := range args {
		if i&1 != 0 {
			ret[prev] = val
		} else {
			prev = val
			ret[val] = ""
		}
	}
	return ret
}

func usage(argv string) {
	fmt.Println("Usage: ", argv, " [URL DEST]...")
}
