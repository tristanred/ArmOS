Steps to publish a new version of the docker image


(have to do `docker login ghcr.io/tristanred/armos-builder:latest` first)

docker build -t armos-builder .

docker tag armos-builder ghcr.io/tristanred/armos-builder:latest

(optionally tag the old version)

docker push ghcr.io/tristanred/armos-builder:latest