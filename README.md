# 🎮 cub3D - 3D Maze Game

<div align="center">
  <img src="[Reserved for Project Banner]" alt="cub3D Banner" width="800"/>
</div>

## 📖 Overview

cub3D is a 3D maze game built using raycasting technology, inspired by the legendary Wolfenstein 3D game. This project is part of the 42 School curriculum, demonstrating advanced graphics programming concepts and game development fundamentals.

<div align="center">
  <img src="[Reserved for Gameplay GIF]" alt="Gameplay Demo" width="600"/>
</div>

## ✨ Features

### Core Features
- 🎥 First-person 3D visualization using raycasting
- 🏃‍♂️ Smooth player movement (W, A, S, D keys)
- 🔄 View rotation (arrow keys)
- 🎨 Textured walls with different textures for each direction (N, S, E, W)
- 🎪 Customizable floor and ceiling colors
- 🗺️ Custom map format support (.cub files)
- 🖥️ Clean window management

<div align="center">
  <img src="[Reserved for Features Showcase GIF]" alt="Features Demo" width="400"/>
</div>

### Bonus Features
- 💥 Wall collision detection
- 🗺️ Dynamic minimap system
- 🚪 Interactive doors
- 🎬 Animated sprites
- 🖱️ Mouse-controlled camera rotation

<div align="center">
  <img src="[Reserved for Bonus Features GIF]" alt="Bonus Features Demo" width="400"/>
</div>

## 🎯 Installation

```bash
# Clone this repository
git clone https://github.com/yourusername/cub3d.git

# Navigate to the project directory
cd cub3d

# Compile the project
make

# Run with a map
./cub3D maps/example.cub
```

## 🎮 Controls

| Key | Action |
|-----|--------|
| W | Move forward |
| S | Move backward |
| A | Strafe left |
| D | Strafe right |
| ← | Rotate camera left |
| → | Rotate camera right |
| ESC | Exit game |
| Mouse | Camera rotation (bonus) |
| E | Interact with doors (bonus) |

## 🗺️ Map Format

Maps use the `.cub` extension and follow this format:

```
NO ./path_to_north_texture
SO ./path_to_south_texture
WE ./path_to_west_texture
EA ./path_to_east_texture
F 220,100,0
C 225,30,0

111111111111
100000000001
100000000001
100000N00001
111111111111
```

### Map Elements
- `0`: Empty space
- `1`: Wall
- `N/S/E/W`: Player start position and orientation
- `D`: Door (bonus feature)

<div align="center">
  <img src="[Reserved for Map Examples]" alt="Map Examples" width="600"/>
</div>

## 🛠️ Technical Details

### Technologies Used
- C programming language
- MinilibX graphics library
- Math library
- Raycasting technique

### Architecture
```
cub3D/
├── src/          # Source files
├── includes/     # Header files
├── assets/       # Textures and maps
├── libft/        # Custom library
└── maps/         # Game maps
```

## 📊 Performance

<div align="center">
  <img src="[Reserved for Performance Metrics]" alt="Performance Metrics" width="400"/>
</div>

- Average FPS: XX
- Memory Usage: XX MB
- Load Time: XX ms

## 🎥 Showcase

### Basic Navigation
<div align="center">
  <img src="[Reserved for Navigation Demo]" alt="Navigation Demo" width="600"/>
</div>

### Texture Mapping
<div align="center">
  <img src="[Reserved for Texture Demo]" alt="Texture Mapping Demo" width="600"/>
</div>

### Bonus Features
<div align="center">
  <img src="[Reserved for Bonus Demo]" alt="Bonus Features Demo" width="600"/>
</div>

## 🤝 Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📝 License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

## 👏 Acknowledgments

- Wolfenstein 3D for inspiration
- 42 School for the project specification
- [MLX Library Documentation](https://harm-smits.github.io/42docs/libs/minilibx)
- [Raycasting Tutorial](https://lodev.org/cgtutor/raycasting.html)

## 📬 Contact

Your Name - [@yourusername](https://twitter.com/yourusername)

Project Link: [https://github.com/yourusername/cub3d](https://github.com/yourusername/cub3d)

---
<div align="center">
  Made with ❤️ by @hluiz-ma and @marsoare
</div>

