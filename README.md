# GMatrix  
![Test](https://github.com/gducpm/gmatrix/actions/workflows/test.yml/badge.svg)![Release](https://github.com/gducpm/gmatrix/actions/workflows/release.yml/badge.svg)![Platform](https://img.shields.io/badge/platform-GNU+Linux | macOS | Windows-brightgreen)![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)![Repo Size](https://img.shields.io/github/repo-size/gducpm/gmatrix)
A fast and lightweight terminal entertainment program for Matrix rain  
![GMatrix](./.github/assets/gmatrix_preview.png)
## General Info  
**Platforms**: GNU+Linux, Windows, MacOS
**Version**: 1.0.0  
**Author**: GDucpm  
**Contributors**: none for the time being  
**License**: GNU General Public License 3+  
## Quickstart  
1. Go to the latest release. On GitHub, search for the Releases section on the panel on the right side  
2. Search for executables on your platform.  
    - Windows: .msi (OFFICIAL INSTALLER), .zip (portable file)  
    - GNU+Linux in general: .tar.xz, .tar.gz, .tar.zst (general portables)  
    - Debian-based (Debian, Ubuntu, Pop...): .deb (OFFICIAL INSTALLER)  
    - Red Hat-based (Red Hat, Fedora...): .rpm (OFFICIAL INSTALLER)  
    - MacOS: .pkg (OFFICIAL INSTALLER), .dmg  
3. Download and enjoy!  
## Compilation for UNIX (MacOS, GNU/Linux)  
1. Clone this repo  
2. Run:  
```sh
cmake -B build -DCMAKE-BUILD-TYPE=Release
cmake --build build --config Release
sudo cmake --install build
```
If you want to avoid running the last command as root or avoid installing to the system, use `cmake --install build --prefix ~/.local` instead.  
## Compilation for Windows
1. Open PowerShell in ADMINISTRATOR MODE (Unless you are installing as user)  
2. Clone this repo  
3. Install some dependencies using VCPKG:  
```pwsh
vcpkg install pdcurses:x64-windows getopt-win32:x64-windows
```
4. If installing for all users (be careful not to wipe the system PATH):  
```pwsh
New-Item -ItemType Directory -Force -Path "C:\Program Files\GMatrix"
cmake --install build --config Release --prefix "C:\Program Files\GMatrix"
$p = [Environment]::GetEnvironmentVariable("Path", "Machine")
[Environment]::SetEnvironmentVariable("Path", "$p;C:\Program Files\GMatrix", "Machine")
```
5. If installing for you only (be careful not to wipe your PATH):  
```pwsh
$installDir = "$env:LOCALAPPDATA\GMatrix"
if (!(Test-Path $installDir)) { New-Item -ItemType Directory -Path $installDir }
cmake --install build --config Release --prefix $installDir
$p = [Environment]::GetEnvironmentVariable("Path", "User")
[Environment]::SetEnvironmentVariable("Path", "$p;$env:LOCALAPPDATA\GMatrix", "User")
```
## Warranty  
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.  

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.  

You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.  
