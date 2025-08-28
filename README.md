# miniRT (42)

Small ray tracer written with MiniLibX for 42 school. Written in a group together with https://github.com/michaela811.

<p align="center">
  <img src="assets/miniRT_preview.png" width="400" alt="miniRT preview">
</p>

> Mandatory and bonus share the **same code paths**; bonus behaviour is toggled at runtime by the executable name. We keep a doubled file layout to satisfy 42’s project rules.

## Table of contents
- [About](#about)
- [Submodules](#submodules)
- [Features](#features)
- [Build](#build)
- [Usage](#usage)
- [Scene (`.rt`) quick reference](#scene-rt-quick-reference)
- [Bonus switch (how it works)](#bonus-switch-how-it-works)
- [Troubleshooting](#troubleshooting)
- [Credits](#credits)
- [License](#license)

## About
`miniRT` is a minimal ray tracer that renders basic geometric objects with lighting and shadows. It uses **MiniLibX** for windowing and image output, **libft** for common utilities, and **get_next_line** for streaming scene parsing. The project was developed as part of the **42** curriculum.

- **Language:** C (C99-ish, 42 Norm compliant)
- **Platform:** Linux (MiniLibX X11). macOS can work with the usual MLX flags (not the default here).

---

## 🔌 Submodules

This repo uses three git submodules:

- **libft** → `lib/libft`  
- **get_next_line** → `lib/getnextline` (uses **bonus** sources inside `gnl_bonus/`)  
- **minilibx-linux** → `lib/minilibx_linux` (MiniLibX is a small graphics lib used in 42 projects - an abstraction of the “X Window System” for Unix platforms)

### Clone with submodules

```bash
git clone --recurse-submodules https://github.com/grignetta/miniRT.git miniRT
# or, if you already cloned:
git submodule update --init --recursive
```

### Update submodules later

```bash
git submodule update --remote --merge
```
---

## Features
### Mandatory
- Camera with FOV
- Basic geometric primitives: **sphere**, **plane**, **cylinder**
- Scene parser from a `.rt` file
- Per-pixel ray casting & intersections

### Bonus (runtime toggle)
> Code is shared; bonus effects activate when running the `miniRT_bonus` executable (see [Bonus switch](#bonus-switch-how-it-works)).

- **Material**: matte / glossy (diffuse lightening)
- **Ambient light** support
- **Specular reflection** (mirror highlights)
- **Shadows** (hard shadowing)
- **Light types**: ambient, point, directional



## Build
### Dependencies (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install -y build-essential xorg-dev libbsd-dev
```

### Clone with submodules
```bash
git clone --recurse-submodules <this-repo-url>
# or, if already cloned
git submodule update --init --recursive
```

### Compile
```bash
make         # builds ./miniRT
make bonus   # builds ./miniRT_bonus
```

> The Makefile builds **libft**, **minilibx_linux**, and compiles **get_next_line** from `lib/getnextline/gnl_mandatory/` into the project.

## Usage
Render a scene from a `.rt` file:

```bash
./miniRT scenes/file.rt
# or (bonus runtime features enabled)
./miniRT_bonus scenes/file_bonus.rt
```

Window controls are the usual MLX ones (close the window with the window’s close button; Esc key if mapped in your build).

## Scene (`.rt`) quick reference
The parser follows the classic 42 miniRT subject format. Minimal elements:

- **Ambient light**: `A <ratio> <R,G,B>`
- **Camera**: `C <x,y,z> <nx,ny,nz> <FOV>`
- **Light (point)**: `L <x,y,z> <brightness> <R,G,B>`
- **Sphere**: `sp <x,y,z> <diameter> <R,G,B>`
- **Plane**: `pl <x,y,z> <nx,ny,nz> <R,G,B>`
- **Cylinder**: `cy <x,y,z> <nx,ny,nz> <diameter> <height> <R,G,B>`

**Example:**
```rt
A 0.2 255,255,255
C -50,0,20 0,0,1 70
L -40,0,30 0.7 255,255,255
sp 0,0,20 20 255,0,0
pl 0,0,0 0,1,0 255,255,255
cy 50,0,20 0,0,1 14.2 21.42 10,0,255
```

> Colors are `0-255`. Normals are normalized direction vectors. Values are validated by input checks.

## Bonus switch (how it works)
Mandatory and bonus compile to **two executables**. At runtime, we detect which one is running:

```c
int is_bonus(char **argv)
{
    if (ft_strcmp(argv[0], "./miniRT_bonus") == 0)
        return (1);
    else
        return (0);
}
```

All bonus-only behaviours (specular highlights, shadows, extra light types, etc.) are gated behind this check.

## Troubleshooting
- **Linker errors about X11**: ensure `xorg-dev` is installed.
- **`undefined reference to 'bsd_*'`**: install `libbsd-dev` (some distros require `-lbsd`; our Makefile doesn’t by default).
- **Blank window / no image**: verify your scene path and contents; start with a minimal scene (see example above).
- **Submodules missing**: run `git submodule update --init --recursive`.

## Credits
- **Authors:** Dari and [michaela811](https://github.com/michaela811)
- **Libraries:** [MiniLibX (Linux)](https://github.com/42Paris/minilibx-linux), libft, get_next_line.

