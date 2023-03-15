
all: release dist docker

release:
	cd webui && npm run build
	cd websrv && cargo build --release

dist:
	-rm -rf dist
	mkdir dist
	cp -R webui/dist/ dist/public/
	cp websrv/target/release/websrv dist/
	cp websrv/prompt.txt dist/
	cp websrv/welcome.html dist/
	cp websrv/RocketProd.toml dist/Rocket.toml
	cp -R MeriseAcide/MeriseAcide0.4.7/libgv_Linux64 dist/
	cp -R MeriseAcide/MeriseAcide0.4.7/MeriseAcide_Linux64 dist/
	cp -R MeriseAcide/MeriseAcide0.4.7/MeriseAcide dist/

docker:
	cd dist && docker build -t drawmeadb -f ../Dockerfile .

deploy: all
	docker tag drawmeadb:latest registry.fly.io/drawmeadb
	docker push registry.fly.io/drawmeadb
	flyctl deploy

.PHONY: all release dist docker deploy
