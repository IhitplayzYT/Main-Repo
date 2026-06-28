<div align="center">

```text
 _                      ____                                      
| |    __ _ _ __   __ _/ ___|___  _ __ ___  _ __   ___  ___  ___ 
| |   / _` | '_ \ / _` | |   / _ \| '_ ` _ \| '_ \ / _ \/ __|/ _ \
| |__| (_| | | | | (_| | |__| (_) | | | | | | |_) | (_) \__ \  __/
|_____\__,_|_| |_|\__, |\____\___/|_| |_| |_| .__/ \___/|___/\___|
                  |___/                     |_|                    
```

# LangCompose

### Lightning-fast Internationalization Migration for JavaScript Projects

[![Rust](https://img.shields.io/badge/Rust-100%25-orange?style=for-the-badge&logo=rust)](https://www.rust-lang.org/)
[![License](https://img.shields.io/badge/License-MIT-blue?style=for-the-badge)](LICENSE)
[![Platform](https://img.shields.io/badge/Linux-Tested-success?style=for-the-badge&logo=linux)]()
[![JavaScript](https://img.shields.io/badge/JavaScript-Supported-yellow?style=for-the-badge&logo=javascript)]()
[![Status](https://img.shields.io/badge/Status-Production_Ready-brightgreen?style=for-the-badge)]()

*Automatically convert existing JavaScript projects into fully internationalized applications in seconds.*

</div>

---

# Overview

**LangCompose** is a high-performance source-to-source migration tool written in Rust that automatically transforms existing JavaScript projects into localization-ready codebases.

Instead of manually replacing every hardcoded string, LangCompose analyzes your project, wraps strings with your translation helper, generates locale files, and preserves your project structure.

The goal is to reduce an otherwise tedious migration process from hours or days to a single command.

---

# Features

- Fast recursive project scanning
- Automatic string extraction
- Automatic generation of `i18n.js`
- Automatic generation of `locales/`
- Converts hardcoded strings into `t("...")`
- Restorable migration
- Clean rerunnable transformations
- Configurable ignored files/directories
- Configurable output languages
- Zero runtime dependencies
- Written entirely in Rust
- Suitable for medium to very large codebases

---

# Before

```javascript
function Welcome({ name }) {
    return (
        <h1>Hello {name}</h1>
    );
}
```

---

# After

```javascript
function Welcome({ name }) {
    return (
        <h1>{t("Hello ${name}").replace("${name}", name)}</h1>
    );
}
```

Generated locale:

```json
{
    "Hello ${name}": "Hello ${name}"
}
```

---

# Installation

Clone the repository

```bash
git clone https://github.com/IhitplayzYT/LangCompose
cd LangCompose
```

Build

```bash
cargo build --release
```

The executable will be located at

```text
target/release/langcompose
```

---

# Usage

```bash
langcompose [OPTIONS] <PROJECT_DIRECTORY>
```

Example

```bash
langcompose ./MyProject
```

Debug mode

```bash
langcompose --debug ./MyProject
```

Restore project

```bash
langcompose --restore ./MyProject
```

Clean migration

```bash
langcompose --clean-run ./MyProject
```

---

# Command Line Options

| Option | Description |
|---------|-------------|
| `-d`, `--debug` | Enable verbose debugging output |
| `-r`, `--restore` | Restore the project to its original state |
| `-c`, `--clean-run` | Restore then immediately rerun LangCompose |
| `-h`, `--help` | Display help |
| `-m`, `--model` | Change the LLM Model for generating translations
---

# Generated Project Structure

```
MyProject
│
├── i18n.js
├── locales
│   ├── en.json
│   ├── fr.json
│   ├── de.json
│   └── ...
│
├── src
│   ├── ...
│
└── ...
```

---

# Configuration

LangCompose recognizes two optional files.

## LangCompose

Defines which locale files should be generated.

Example

```text
en
fr
de
ja
hi
```

---

## .LangIgnore

Files and directories ignored during scanning.

Example

```text
node_modules
dist
build
coverage

.git

*.min.js
*.bundle.js
```

---

# Migration Pipeline

```
Project
    │
    ▼
Recursive Scan
    │
    ▼
String Detection
    │
    ▼
Translation Wrapping
    │
    ▼
Placeholder Detection
    │
    ▼
Locale Generation
    │
    ▼
Generate i18n.js
    │
    ▼
Localization Ready Project
```

---

# Restore Mode

Running

```bash
langcompose --restore MyProject
```

will

- Remove generated locale files
- Remove generated `i18n.js`
- Restore all modified JavaScript files
- Remove all LangCompose artifacts

---

# Clean Run

```bash
langcompose --clean-run MyProject
```

is equivalent to

```text
Restore

↓

Fresh Migration
```

This guarantees that repeated executions always start from a clean state.

---

# Why LangCompose?

Migrating an existing application to internationalization usually involves

- locating every user-visible string
- replacing it manually
- creating locale files
- keeping placeholders synchronized
- ensuring consistency

LangCompose automates this entire workflow while preserving your project layout.

---

# Built With

- Rust
- Regex
- Serde

---

# Roadmap

- [ ] TypeScript support improvements
- [ ] JSX parser improvements
- [ ] Parallel processing
- [ ] Interactive migration mode

---

# License

MIT License

---

<div align="center">

### Made with Rust

Fast • Safe • Deterministic

</div>
