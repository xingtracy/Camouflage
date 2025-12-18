# Face Filter Application

## Overview

The Face Filter Application is an interactive real-time face processing application built with openFrameworks. It uses computer vision techniques to detect faces in a live camera feed and applies various visual effects to them in real-time. The application provides an intuitive interface for experimenting with different face filters and capturing screenshots of the processed video feed.

## Features

### 1. Real-Time Face Detection
- **Haar Cascade Classifier**: Utilizes OpenCV's pre-trained Haar Cascade classifier (`haarcascade_frontalface_default.xml`) to detect frontal faces in the video stream
- **Smooth Tracking**: Implements object tracking with persistence (15 frames) to maintain stable face detection across frames
- **Multiple Face Support**: Can detect and process multiple faces simultaneously in the camera feed
- **Fast Detection**: Uses the "Fast" preset for optimized performance

### 2. Visual Effects
The application provides four distinct visual effects that can be applied to detected faces:

- **Blur Effect**: Applies a box blur filter with a 15-pixel radius, creating a privacy-focused blurring effect
- **Pixelate Effect**: Converts the face region into a pixelated mosaic with 10x10 pixel blocks
- **Grayscale Effect**: Converts the face region to grayscale using brightness-based conversion
- **Invert Effect**: Inverts the RGB color values of the face region, creating a negative image effect
- **No Effect**: Option to disable all effects and view the original video feed

### 3. User Interface
- **Real-Time Face Count Display**: Shows the number of faces currently detected in the video feed
- **Current Effect Indicator**: Displays which effect is currently active
- **Face Rectangle Overlay**: Optional green rectangle overlay to visualize detected face boundaries
- **On-Screen Instructions**: Displays keyboard shortcuts and usage instructions directly on the screen

### 4. Interactive Controls
- **Effect Selection**: Switch between different effects using number keys (1-5)
- **Face Rectangle Toggle**: Show/hide face detection rectangles with the 'F' key
- **Screenshot Capture**: Save the current frame as a PNG image with timestamp using the spacebar

## How to Use

### Initial Setup

1. **Prerequisites**: Ensure you have openFrameworks installed with the following addons:
   - `ofxOpenCv`
   - `ofxCv`
   - `ofxGui`

2. **Cascade File**: The application requires the Haar Cascade XML file (`haarcascade_frontalface_default.xml`) in the `bin/data/` directory. The application will attempt to copy it automatically if not found.

3. **Camera Access**: Ensure your camera is connected and accessible. The application uses device ID 0 by default.

### Running the Application

1. **Launch**: Run the compiled application. The window will open with a 1024x768 resolution (or adjust based on your settings).

2. **Camera Initialization**: The application will automatically initialize the camera at 640x480 resolution and begin capturing video.

3. **Face Detection**: Position yourself (or others) in front of the camera. Detected faces will be automatically processed with the current effect.

### Keyboard Controls

| Key | Action |
|-----|--------|
| `1` | Apply Blur effect |
| `2` | Apply Pixelate effect |
| `3` | Apply Grayscale effect |
| `4` | Apply Invert effect |
| `5` | Disable all effects (No effect) |
| `F` or `f` | Toggle face rectangle overlay |
| `SPACE` | Save screenshot with timestamp |

### Usage Tips

- **Lighting**: Ensure adequate lighting for better face detection accuracy
- **Face Position**: Face the camera directly for optimal detection (frontal face detection)
- **Multiple Faces**: The application can handle multiple faces simultaneously
- **Screenshots**: Screenshots are saved in the application's root directory with timestamps

## Technical Implementation

### Architecture

The application follows the standard openFrameworks application structure with the main application logic in the `ofApp` class, which inherits from `ofBaseApp`.

### Core Components

#### 1. Camera Capture (`ofVideoGrabber`)
```cpp
vidGrabber.setDeviceID(0);
vidGrabber.initGrabber(640, 480);
```
- Captures video from the default camera (device ID 0)
- Resolution: 640x480 pixels
- Updates every frame in the `update()` loop

#### 2. Image Processing (`ofxCvColorImage`, `ofxCvGrayscaleImage`)
- **Color Image**: Stores the full-color camera feed
- **Grayscale Image**: Converted from color image for face detection (Haar Cascade works on grayscale)

#### 3. Face Detection (`ofxCv::ObjectFinder`)
```cpp
faceFinder.setup("haarcascade_frontalface_default.xml");
faceFinder.setPreset(ofxCv::ObjectFinder::Fast);
faceFinder.getTracker().setPersistence(15);
```
- Uses OpenCV's Haar Cascade classifier for face detection
- Fast preset for real-time performance
- Tracker persistence of 15 frames for stable detection

### Effect Implementation

All effects operate on pixel-level manipulation of the `ofPixels` object extracted from the color image:

#### Blur Effect
- **Algorithm**: Box blur (averaging neighboring pixels)
- **Radius**: 15 pixels
- **Process**: For each pixel in the face region, averages RGB values from surrounding pixels within the blur radius
- **Performance**: O(n × r²) where n is face pixels and r is blur radius

#### Pixelate Effect
- **Block Size**: 10x10 pixels
- **Process**: Divides face region into blocks and fills each block with the color of the top-left pixel
- **Performance**: O(n/p) where p is pixel block size

#### Grayscale Effect
- **Algorithm**: Brightness-based conversion
- **Process**: Converts each pixel's RGB to grayscale using `color.getBrightness()`
- **Performance**: O(n) linear time

#### Invert Effect
- **Process**: Inverts each RGB channel: `255 - channel_value`
- **Performance**: O(n) linear time

### Rendering Pipeline

1. **Update Loop**:
   - Capture new frame from camera
   - Convert to grayscale for detection
   - Run face detection
   - Apply selected effect to each detected face region
   - Update image buffers

2. **Draw Loop**:
   - Render processed color image
   - Overlay face rectangles (if enabled)
   - Draw UI text (face count, effect name, instructions)

### Performance Optimizations

- **Frame-based Processing**: Only processes new frames (`vidGrabber.isFrameNew()`)
- **Region-based Effects**: Effects are applied only to detected face regions, not the entire image
- **Fast Detection Preset**: Uses optimized detection settings for real-time performance
- **Smooth Tracking**: Reduces jitter in face detection with frame persistence

### Dependencies

- **openFrameworks**: Core framework for multimedia applications
- **ofxOpenCv**: OpenFrameworks wrapper for OpenCV
- **ofxCv**: Additional OpenCV utilities and helpers
- **OpenCV**: Computer vision library (via addons)

### File Structure

```
myFinalProject/
├── src/
│   ├── main.cpp          # Application entry point
│   ├── ofApp.h           # Class declaration
│   └── ofApp.cpp         # Main application logic
├── bin/data/
│   └── haarcascade_frontalface_default.xml  # Face detection classifier
├── addons.make           # Required addons list
└── README.md            # This file
```

## Technical Details

### Image Format
- **Input**: RGB color images from camera (640x480)
- **Processing**: 8-bit per channel color depth
- **Output**: Real-time processed video feed

### Face Detection Parameters
- **Classifier**: Haar Cascade (frontal face)
- **Detection Mode**: Fast preset
- **Tracking Persistence**: 15 frames
- **Coordinate System**: Screen coordinates (0,0 at top-left)

### Effect Parameters
- **Blur Radius**: 15 pixels (configurable in code)
- **Pixelate Block Size**: 10x10 pixels (configurable in code)
- **Grayscale Method**: Brightness-based
- **Invert Method**: RGB channel inversion

## Limitations and Future Enhancements

### Current Limitations
- Only detects frontal faces (not profile views)
- Fixed effect parameters (not user-adjustable)
- Single camera support (device ID 0)
- Fixed resolution (640x480)

### Potential Enhancements
- Adjustable effect parameters (blur radius, pixel size, etc.)
- Additional effects (sepia, edge detection, artistic filters)
- Profile face detection
- Multiple camera support
- Video recording functionality
- Face tracking with history/visualization
- Custom effect presets
- GUI panel for parameter adjustment

## System Requirements

- **Platform**: macOS (iOS release version)
- **openFrameworks**: Version 0.12.1
- **Camera**: Any compatible USB or built-in camera
- **Performance**: Real-time processing requires adequate CPU/GPU

## License and Credits

This application uses:
- **openFrameworks**: Open-source C++ toolkit
- **OpenCV**: Open-source computer vision library
- **Haar Cascade Classifier**: Pre-trained model from OpenCV

---

*Built with openFrameworks and OpenCV for real-time face processing and visual effects.*

