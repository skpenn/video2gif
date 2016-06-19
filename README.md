# video2gif

A program used to convert video to gif with openCV.

## Summary

[video2gif](https://github.com/skpenn/video2gif) is programmed to convert some most popular video format files into GIF.
For now you can convert video files with its format in: .mp4, .avi, .rm, .rmvb, .mkv, to GIF files. You can also convert a picture to 
a static GIF image, supported file format including .jpg, .bmp. Transparent color is not supported for the moment.

## Dependency

- Operating System: Windows only, suggesting Windows 8 or higher
- Compiler: Visual Studio 2010 or higher
- OpenCV: OpenCV 3+

## Usage

The simplest way to use this program is:
```shell
video2gif yourvideofile
```
This command will generate a GIF file with original image size, a same FPS as video, a same name as video file.
You can add some options to make your generated GIF file better.

### Decide output filename
The ouput file can be given a name with the follow option:
```shell
-o yourname.gif
```

### Resize image
Your can resize your output image with the command like:
```shell
-r 0.5
```
This will generate a GIF file with half width and height as original video.
Notice that Enlarging the image is not supported now so a parameter larger then 1 will be ignored.

### Setting FPS
You can set FPS of your file. Command:
```shell
-f 12
```
Given FPS should not be great than the original one.

### Reset playing speed
Maybe you want to speed up or slow down your GIF file's playing speed, then you can use the command:
```shell
-s 1.1
```
This command will generate a GIF with its playing speed 1.1 times faster than original one.
Notice that this will change your FPS setting, the real FPS is Speed Ratio multiple set FPS.

### Cut a chapter
Maybe you just want to generate a GIF with just a part of your video instead of the whole one.
The cut command gives you the option:
```shell
-c 120.5-121.5
```
you should provide 2 time points with their units  second, connected by symbol '-'. 
The example command will generate GIF with input video start from 120.5s to 121.5s. 
Time point can be decimals. Notice that the time format 'hh:mm:ss' is not supported for now.

### Use local color table
When the command
```shell
-l
```
is added, every frame will be quantized dividedly. This will make your file more colorful but larger.

## Use key frame
The command
```shell
-k
```
will generate animated GIF with the first frame as its reference frame. 
Any other frame will cover over it while playing.

### Help info
If you don't give any parameters(including input filename), then you'll get the full notice.
