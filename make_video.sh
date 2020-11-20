#ffmpeg -i $1/%d.png -vcodec h264 -filter:v fps=fps=60 $1/out.mp4
ffmpeg -framerate 60 -i $1/%d.png -pix_fmt yuv420p -vcodec h264 -vf "pad=ceil(iw/2)*2:ceil(ih/2)*2" -r 60 $1/out.mp4
