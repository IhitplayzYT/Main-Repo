package main

// Video file extensions
var VideoExt = map[string]struct{}{
	".mp4": {}, ".mkv": {}, ".avi": {}, ".mov": {}, ".webm": {},
	".flv": {}, ".wmv": {}, ".mpeg": {}, ".mpg": {}, ".3gp": {},
}

// Image file extensions
var ImageExt = map[string]struct{}{
	".jpg": {}, ".jpeg": {}, ".png": {}, ".gif": {}, ".bmp": {},
	".webp": {}, ".svg": {}, ".tiff": {}, ".ico": {}, ".avif": {},
}

// Audio file extensions
var AudioExt = map[string]struct{}{
	".mp3": {}, ".wav": {}, ".aac": {}, ".ogg": {}, ".flac": {},
	".m4a": {}, ".wma": {}, ".opus": {}, ".aiff": {},
}

// Document / file extensions
var DocExt = map[string]struct{}{
	".pdf": {}, ".doc": {}, ".docx": {}, ".xls": {}, ".xlsx": {},
	".ppt": {}, ".pptx": {}, ".txt": {}, ".csv": {}, ".rtf": {},
	".odt": {}, ".ods": {}, ".odp": {}, ".md": {},
}

// Archive / compressed files
var ArchiveExt = map[string]struct{}{
	".zip": {}, ".rar": {}, ".7z": {}, ".tar": {}, ".gz": {},
	".bz2": {}, ".xz": {}, ".tgz": {},
}

// Web-related files (often scraped)
var WebExt = map[string]struct{}{
	".html": {}, ".htm": {}, ".css": {}, ".js": {}, ".json": {},
	".xml": {}, ".wasm": {},
}

// Fonts (useful for asset scraping)
var FontExt = map[string]struct{}{
	".woff": {}, ".woff2": {}, ".ttf": {}, ".otf": {}, ".eot": {},
}

// Misc useful extensions
var MiscExt = map[string]struct{}{
	".apk": {}, ".exe": {}, ".dmg": {}, ".iso": {},
	".torrent": {}, ".log": {},
}

var assetExt = map[string]bool{
	".jpg": true, ".jpeg": true, ".png": true, ".gif": true, ".webp": true,
	".mp4": true, ".webm": true, ".mp3": true, ".wav": true, ".ogg": true,
	".pdf": true, ".zip": true, ".tar": true, ".gz": true,
}

var SupportedExts = map[string]bool{
	// Video
	".mp4": true, ".mkv": true, ".avi": true, ".mov": true, ".webm": true,
	".flv": true, ".wmv": true, ".mpeg": true, ".mpg": true, ".3gp": true,

	// Image
	".jpg": true, ".jpeg": true, ".png": true, ".gif": true, ".bmp": true,
	".webp": true, ".svg": true, ".tiff": true, ".ico": true, ".avif": true,

	// Audio
	".mp3": true, ".wav": true, ".aac": true, ".ogg": true, ".flac": true,
	".m4a": true, ".wma": true, ".opus": true, ".aiff": true,

	// Documents
	".pdf": true, ".doc": true, ".docx": true, ".xls": true, ".xlsx": true,
	".ppt": true, ".pptx": true, ".txt": true, ".csv": true, ".rtf": true,
	".odt": true, ".ods": true, ".odp": true, ".md": true,

	// Archives
	".zip": true, ".rar": true, ".7z": true, ".tar": true, ".gz": true,
	".bz2": true, ".xz": true, ".tgz": true,

	// Web
	".html": true, ".htm": true, ".css": true, ".js": true,
	".json": true, ".xml": true, ".wasm": true,

	// Fonts
	".woff": true, ".woff2": true, ".ttf": true, ".otf": true, ".eot": true,

	// Misc
	".apk": true, ".exe": true, ".dmg": true, ".iso": true,
	".torrent": true, ".log": true,
}
