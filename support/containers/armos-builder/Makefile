all: build

build:
	docker build -t ghcr.io/tristanred/armos-builder:latest .

rebuild:
	docker build -t ghcr.io/tristanred/armos-builder:latest --no-cache .

publish:
	docker push ghcr.io/tristanred/armos-builder:latest

clean:
	docker image rm ghcr.io/tristanred/armos-builder:latest

pull:
	docker pull ghcr.io/tristanred/armos-builder:latest