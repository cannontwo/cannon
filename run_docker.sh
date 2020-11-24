docker run -it \
  --rm \
  --gpus all \
  -v /tmp/.X11-unix:/tmp/.X11-unix \
  -e DISPLAY=$DISPLAY \
  --entrypoint /bin/bash \
  cannon/cannon
