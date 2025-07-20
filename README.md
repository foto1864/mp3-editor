# MP3 Metadata Editor 🎵

A simple GTK-based desktop app for Linux that lets you **edit MP3 metadata** (title, artist, album) and **add cover art** — all in style.

Built entirely in **C** using the **GTK+3** toolkit and powered by Python’s `eyeD3` behind the scenes for metadata editing.

## ✨ Features

- 📁 Select a folder and view all `.mp3` files inside
- ✍️ Edit artist and album for all tracks
- 🔤 Inline renaming of track titles
- 🖼️ Add or update album cover images
- 🧼 Automatically sorts the tracklist alphabetically
- 🧠 Dark-mode friendly interface

## 🖥️ Screenshots
TODO

## 🛠️ Dependencies

### Required:
- `gtk+3`
- `eyeD3` (installed in a Python virtual environment or system-wide)
- A C compiler (e.g., `gcc`)

### On Debian/Ubuntu:

#### bash:
sudo apt install libgtk-3-dev python3-pip
pip install --user eyed3
Or install eyeD3 inside a virtualenv and point the app to its eyeD3 binary.

🧪 Building from Source
Clone the repo and run:
#### bash:
make

Then run the app:
#### bash:
make run

To clean build files:
#### bash:
make clean

## 🔌 Connect Python virtualenv (optional)
If eyeD3 is installed inside a virtualenv, make sure to update the hardcoded path in ui.c:
#define EYED3_PATH "/path/to/your/venv/bin/eyeD3"

## 🚀 Install as a native app (optional)
Copy the compiled binary to /usr/local/bin/

Place the .desktop launcher file in ~/.local/share/applications/

Copy your icon to ~/.local/share/icons/

Refresh your app menu and enjoy 🎧