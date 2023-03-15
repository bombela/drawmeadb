
all: release dist

release:
	cd webui && npm run build
	cd websrv && cargo build --release

dist:
	-rm -rf dist
	mkdir dist
	cp -R webui/dist/ dist/public/
	cp websrv/target/release/websrv dist/
	cp websrv/prompt.txt dist/
	cp websrv/RocketProd.toml dist/Rocket.toml
	cp -R MeriseAcide/MeriseAcide0.4.7/libgv_Linux64 dist/
	cp -R MeriseAcide/MeriseAcide0.4.7/MeriseAcide_Linux64 dist/
	cp -R MeriseAcide/MeriseAcide0.4.7/MeriseAcide dist/

.PHONY: all release dist
