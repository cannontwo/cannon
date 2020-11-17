ffmpeg -i $1/%d.png -vcodec h264 -filter:v fps=fps=60 $1/out.mp4
